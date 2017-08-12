/*
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-2001
 */

/**
 * \file main.c
 * main program for PCCTS ANTLR.
 */


/* To set a breakpoint just before exit look for "cleanUp".    */
/* To set a breakpoint for fatal error look for "fatal_intern" */

#include <stdio.h>
#include "syn.h"
#include "hash.h"
#include "generic.h"
#include "stdpccts.h"

#define MAX_INT_STACK 50
static int istack[MAX_INT_STACK];   /* Int stack */
static int isp = MAX_INT_STACK;

static int DontAcceptFiles = 0;     /* if stdin, don't read files */
static int DontAcceptStdin = 0;     /* if files seen first, don't accept stdin */

static int tnodes_used_in_guard_predicates_etc;     /* MR10 */

    /* C m d - L i n e  O p t i o n  S t r u c t  &  F u n c s */

typedef struct _Opt {
      char *option;
      int  arg;
      void (*process)();
      char *descr;
    } Opt;

extern void ProcessArgs(int, char **, Opt *);

int ci_strequ(char *a,char *b)
{
  for ( ;*a != 0 && *b != 0; a++, b++) {
    if (toupper(*a) != toupper(*b)) return 0;
  }
  return (*a == *b);
}

static void pStdin()
{
  if ( DontAcceptStdin )
  {
    warnNoFL("'-' (stdin) ignored as files were specified first");
    return;
  }

  require(NumFiles<MaxNumFiles,"exceeded max # of input files");
  FileStr[NumFiles++] = "stdin";
  DontAcceptFiles = 1;
}

static void pFile( char *s )
{
  if ( *s=='-' ) { warnNoFL( eMsg1("invalid option: '%s'",s) ); return; }
  if ( DontAcceptFiles )
  {
    warnNoFL(eMsg1("file '%s' ignored as '-' (stdin option) was specified first",s));
    return;
  }

  require(NumFiles<MaxNumFiles,"exceeded max # of input files");
  FileStr[NumFiles++] = s;
  DontAcceptStdin = 1;
}

/* MR14
        Allow input to be a file containing a list of files
        Bernard Giroud (b_giroud@decus.ch)
*/

static void pFileList( char *s, char *t )
{
#define MaxFLArea 1024
  FILE *fl;
  static char Fn_in_Fl[MaxFLArea] = "";
        char one_fn[MaxFileName];
  char *flp = &Fn_in_Fl[0];
  int fnl, left = MaxFLArea, i;

  if ( *t=='-' ) { warnNoFL( eMsg1("invalid option: '%s'",t) ); return; }
  if ( DontAcceptFiles )
  {
    warnNoFL(eMsg1("file '%s' ignored as '-' (stdin option) was specified first",t));
    return;
  }

        if ((fl = fopen(t, "r")) == NULL)
  {
    warnNoFL(eMsg1("file '%s' can't be opened", t));
    return;
  }
        for (;;)
  {
    if (fgets(one_fn, 128 - 1, fl) == NULL)
      break;
    fnl = strlen(one_fn);
    require(fnl<=left, "no more room in File List Area");
    /* drop the trailing LF */
    if (one_fn[fnl - 1] == 0x0a) one_fn[fnl - 1] = ' ';
    strcat(Fn_in_Fl, one_fn);
    left = left - fnl;
    require(NumFiles<MaxNumFiles,"exceeded max # of input files");
    FileStr[NumFiles++] = flp;
    flp = flp + fnl;
  }
  fclose(fl);
  for (i=0;i < MaxFLArea;i++) if (Fn_in_Fl[i] == ' ') Fn_in_Fl[i] = '\0';
  DontAcceptStdin = 1;
}

static void pLLK( char *s, char *t )
{
  LL_k = atoi(t);
  if ( LL_k <= 0 ) {
    warnNoFL("must have at least one token of lookahead (setting to 1)");
    LL_k = 1;
  }
}

static void pCk( char *s, char *t )
{
  CLL_k = atoi(t);
  if ( CLL_k <= 0 ) {
    warnNoFL("must have at least one token of look-ahead (setting to 1)");
    CLL_k = 1;
  }
}

static void pTab( char *s, char *t )                               /* MR6 */
{                                                    /* MR6 */
  TabWidth = atoi(t);                                      /* MR6 */
  if ( TabWidth < 0 || TabWidth > 8 ) {                      /* MR6 */
    warnNoFL("tab width must be between 1 and 8");               /* MR6 */
    TabWidth=0;                                        /* MR6 */
  }                                                  /* MR6 */
}                                                      /* MR6 */

static int ambAidDepthSpecified=0;                                   /* MR11 */

static void pAAd( char *s, char *t )                               /* MR11 */
{                                                    /* MR11 */
    ambAidDepthSpecified=1;                                          /* MR11 */
  MR_AmbAidDepth = atoi(t);                                /* MR11 */
}                                                      /* MR11 */

static void pTreport( char *s, char *t )                             /* MR11 */
{                                                    /* MR11 */
  TnodesReportThreshold = atoi(t);                         /* MR11 */
}                                                      /* MR11 */

void chkGTFlag()                                            /* 7-Apr-97 MR1 */
{
  if ( !GenAST )
    warn("#-variable or other AST item referenced w/o -gt option");
}


static void pInfo(char *s, char *t)                         /* MR10 */
{
  char  *p;
  int   q;
  for (p=t; *p != 0; p++) {
    q=tolower(*p);
    if (q=='t') {
      InfoT=1;
    } else if (q=='p') {
      InfoP=1;
    } else if (q=='m') {
      InfoM=1;
    } else if (q=='o') {
      InfoO=1;
    } else if (q=='0') {
      ; /* nothing */
    } else if (q=='f') {
      InfoF=1;
    } else {
      warnNoFL(eMsgd("unrecognized -info option \"%c\"",(int)*p));
    };
  };
}

static void pCGen(void) { CodeGen = FALSE; LexGen = FALSE; }
static void pLGen(void) { LexGen = FALSE; }
static void pXTGen(void){ MR_Inhibit_Tokens_h_Gen = TRUE; }
static void pTGen(void) { TraceGen = TRUE; }
static void pSGen(void) { GenExprSetsOpt = FALSE; }
static void pPrt(void)  { PrintOut = TRUE; pCGen(); pLGen(); }
static void pPrtA(void) { PrintOut = TRUE; PrintAnnotate = TRUE; pCGen(); pLGen(); }
static void pAst(void)  { GenAST = TRUE; }
static void pANSI(void) { GenANSI = TRUE; }
static void pCr(void) { GenCR = TRUE; }
static void pNOPURIFY(void) { PURIFY = FALSE; }
/*static void pCt(void) { warnNoFL("-ct option is now the default"); }*/
static void pLI(void) { GenLineInfo = TRUE; GenLineInfoMS = FALSE; } /* MR14 */
static void pLIms(void) { GenLineInfo = TRUE; GenLineInfoMS = TRUE; }  /* MR14 */
static void pFr(char *s, char *t) {RemapFileName = t;}
static void pFe(char *s, char *t) {ErrFileName = t;}
static void pFl(char *s, char *t) {DlgFileName = t;}
static void pFm(char *s, char *t) {ModeFileName = t;}
static void pFt(char *s, char *t) {DefFileName = t;}

static void pE1(void) { elevel = 1; }
static void pE2(void) { elevel = 2; }
static void pE3(void) { elevel = 3; }
static void pEGen(void) { GenEClasseForRules = 1; }
static void pDL(void)
{
  DemandLookahead = 1;
  if ( GenCC ) {
    warnNoFL("-gk does not work currently in C++ mode; -gk turned off");
    DemandLookahead = 0;
  }
}

static void pAA(char *s,char *t) {MR_AmbAidRule = t;}               /* MR11 */
static void pAAm(char *s){MR_AmbAidMultiple = 1;}                   /* MR11 */
static void pGHdr(void) { GenStdPccts = 1; }
static void pFHdr(char *s, char *t) { stdpccts = t; pGHdr(); }
static void pW1(void) { WarningLevel = 1; }
static void pNewAST(void) { NewAST = 1; }                           /* MR13 */
static void ptmakeInParser(void) { tmakeInParser = 1; }             /* MR23 */
static void pAlpha(void) { AlphaBetaTrace = 1; }                    /* MR14 */
static void pMR_BlkErr(void) { MR_BlkErr = 1; }                     /* MR21 */
static void pStdout(void) {UseStdout = 1; }                       /* MR6 */
static void pW2(void) { WarningLevel = 2; }
static void pCC(void) { GenCC = TRUE; }


static void pPre( char *s, char *t )
{
  RulePrefix = t;
}

static void pOut( char *s, char *t )
{
  OutputDirectory = t;
}

static void pPred( )
{
  warnNoFL("-pr is no longer used (predicates employed if present); see -prc, -mrhoist, -mrhoistk");
/*
**  if ( DemandLookahead )
**    warnNoFL("-gk conflicts with -pr; -gk turned off");
**  DemandLookahead = 0;
**  HoistPredicateContext = 0;
*/
}

static void pPredCtx( char *s, char *t )
{
  if ( ci_strequ(t,"on")) HoistPredicateContext = 1;
  else if ( ci_strequ(t,"off")) HoistPredicateContext = 0;
  if ( DemandLookahead )
  {
    warnNoFL("-gk incompatible with semantic predicate usage; -gk turned off");
    DemandLookahead = 0;
  }
}

static void pMRhoist( char *s, char *t )
{
  if ( ci_strequ(t,"on")) MRhoisting = 1;
  else if ( ci_strequ(t,"off")==0 ) MRhoisting = 0;
  if (MRhoisting) {
    fprintf(stderr,"Maintenance Release style hoisting enabled for predicates with lookahead depth = 1\n");
    fprintf(stderr,"  No longer considered experimental\n");
    fprintf(stderr,"  Can't consider suppression for predicates with lookahead depth > 1\n");
    fprintf(stderr,"  Implies -prc on but does *not* imply -mrhoistk for k>1 predicates\n");
    fprintf(stderr,"  This is a reminder, not a warning or error.\n");
  };
}

static void pMRhoistk( char *s, char *t )
{
  if ( ci_strequ(t,"on")) MRhoistingk = 1;
  else if ( ci_strequ(t,"off")==0 ) MRhoistingk = 0;
  if (MRhoistingk) {
    fprintf(stderr,"EXPERIMENTAL Maintenance Release style hoisting enabled\n");
    fprintf(stderr,"  Applies to predicates with lookahead depth > 1\n");
    fprintf(stderr,"  Implies -prc on and -mrhoist on\n");
  };
}

static void pTRes(char *s, char *t)
{
  TreeResourceLimit = atoi(t);
  if ( TreeResourceLimit <= 0 )
  {
    warnNoFL("analysis resource limit (# of tree nodes) must be greater than 0");
    TreeResourceLimit = -1; /* set to no limit */
  }
}

Opt options[] = {
    { "-CC", 0, pCC,  "Generate C++ output (default=FALSE)"},
    { "-cr", 0, pCr,  "Generate cross reference (default=FALSE)"},
    { "-ck", 1, pCk,  "Set compressed lookahead depth; fast approximate lookahead"},
    { "-e1", 0, pE1,  "Ambiguities/errors shown in low detail (default)"},
    { "-e2", 0, pE2,  "Ambiguities/errors shown in more detail"},
    { "-e3", 0, pE3,    "Ambiguities for k>1 grammars shown with exact tuples (not lookahead sets)"},
    { "-f",  1, pFileList,"Read names of grammar files from specified file"},   /* MR14 */
    { "-fe", 1, pFe,  "Rename err.c"},
    { "-fh", 1, pFHdr,  "Rename stdpccts.h header (turns on -gh)"},
    { "-fl", 1, pFl,  "Rename lexical output--parser.dlg"},
    { "-fm", 1, pFm,  "Rename mode.h"},
    { "-fr", 1, pFr,  "Rename remap.h"},
    { "-ft", 1, pFt,  "Rename tokens.h"},
    { "-ga", 0, pANSI,  "Generate ANSI-compatible code (default=FALSE)"},
    { "-gc", 0, pCGen,  "Do not generate output parser code (default=FALSE)"},
    { "-gd", 0, pTGen,  "Generate code to trace rule invocation (default=FALSE)"},
    { "-ge", 0, pEGen,  "Generate an error class for each non-terminal (default=FALSE)"},
    { "-gh", 0, pGHdr,  "Generate stdpccts.h for non-ANTLR-generated-files to include"},
    { "-gk", 0, pDL,  "Generate parsers that delay lookahead fetches until needed"},
    { "-gl", 0, pLI,  "Generate line info about grammar actions in C parser"},
    { "-glms", 0, pLIms,"Like -gl but replace '\\' with '/' in #line filenames for MS C/C++ systems"},
    { "-gp", 1, pPre, "Prefix all generated rule functions with a string"},
    { "-gs", 0, pSGen,  "Do not generate sets for token expression lists (default=FALSE)"},
    { "-gt", 0, pAst, "Generate code for Abstract-Syntax-Trees (default=FALSE)"},
    { "-gx", 0, pLGen,  "Do not generate lexical (dlg-related) files (default=FALSE)"},
    { "-gxt",0, pXTGen, "Do not generate tokens.h (default=FALSE)"},
    { "-k",  1, pLLK, "Set full LL(k) lookahead depth (default==1)"},
    { "-o",  1, pOut, OutputDirectoryOption},
    { "-p",  0, pPrt, "Print out the grammar w/o actions (default=no)"},
    { "-pa", 0, pPrtA,  "Print out the grammar w/o actions & w/FIRST sets (default=no)"},
    { "-pr",0, pPred, "no longer used; predicates employed if present"},
    { "-prc", 1, pPredCtx,"Turn on/off computation of context for hoisted predicates"},
    { "-rl", 1, pTRes,  "Limit max # of tree nodes used by grammar analysis"},
    { "-stdout",0, pStdout, "Send grammar.c/grammar.cpp to stdout"},               /* MR6 */
    { "-tab", 1, pTab,  "Width of tabs (1 to 8) for grammar.c/grammar.cpp files"}, /* MR6 */
  { "-w1", 0, pW1,  "Set the warning level to 1 (default)"},
  { "-w2", 0, pW2,  "Ambiguities yield warnings even if predicates or (...)? block"},
    { "-mrhoist",1,pMRhoist,                                                       /* MR9 */
                         "Turn on/off k=1 Maintenance Release style hoisting"},  /* MR9 */
    { "-mrhoistk",1,pMRhoistk,                                                       /* MR13 */
                         "Turn on/off k>1 EXPERIMENTAL Maintenance Release style hoisting"},  /* MR13 */
    { "-aa"  ,1,pAA,     "Ambiguity aid for a rule (rule name or line number)"},          /* MR11 */
    { "-aam" ,0,pAAm,
                         "Lookahead token may appear multiple times in -aa listing"},     /* MR11 */
    { "-aad" ,1,pAAd,
                         "Limits exp growth of -aa listing - default=1 (max=ck value)"},  /* MR11 */
  { "-info",1,pInfo,
      "Extra info: p=pred t=tnodes f=first/follow m=monitor o=orphans 0=noop"},           /* MR11 */
    { "-treport",1,pTreport,
                        "Report when tnode usage exceeds value during ambiguity resolution"},   /* MR11 */
  { "-newAST", 0, pNewAST,
                 "In C++ mode use \"newAST(...)\" rather than \"new AST(...)\""},         /* MR13 */
  { "-tmake", 0, ptmakeInParser,
                 "In C++ mode use parser's tmake method rather than \"ASTBase::tmake(...)\""},   /* MR23 */
    { "-alpha",0, pAlpha,
                 "Provide additional information for \"(alpha)? beta\" error messages"},  /* MR14 */
    { "-mrblkerr",0,pMR_BlkErr,                                                           /* MR21 */
                 "EXPERIMENTAL change to (...)* and (...)+ syntax error sets"},           /* MR21 */
  { "-nopurify",0,pNOPURIFY,
    "Don't use the notorious PURIFY macro (replaced by MR23 initial value syntax) to zero return arguments of rules"},   /* MR23 */
  { "-",   0, pStdin, "Read grammar from stdin" },
  { "*",   0, pFile,  "" }, /* anything else is a file */
  { NULL,  0, NULL }
 };

void readDescr();
void cleanUp();

static void buildRulePtr( void );
static void help( void );
static void init( void );
static void CompleteTokenSetRefs( void );
static void ensure_no_C_file_collisions(char *);
static void CompleteContextGuards(void);


static void report_numericPredLabels(ActionNode *a)
{                                                                           /* MR10 */
  warnFL("numeric references to attributes (e.g. $i or $i.j) in semantic pred will be null during guess mode",  /* MR10 */
            FileStr[a->file],a->line);                                      /* MR10 */
}                                                                           /* MR10 */

                /* M a i n */

int main( int argc, char *argv[] )
{
  int i;
  static char EPSTR[] = "[Ep]";

    Save_argc=argc;                                                  /* MR10 */
    Save_argv=argv;                                                  /* MR10 */

/*  malloc_debug(8);*/

#ifdef SPECIAL_INITS
    special_inits();                                                 /* MR1 */
#endif
  fprintf(stderr, "Antlr parser generator   Version %s   1989-2001\n", Version);
  if ( argc == 1 ) { help(); zzDIE; }
  ProcessArgs(argc-1, &(argv[1]), options);

/* MR14 */    if (MR_AmbAidRule && AlphaBetaTrace) {
/* MR14 */       fatal("Can't specify both -aa (ambiguity aid) and -alpha (\"(alpha)? beta\" aid)");
/* MR14 */    }

    if (MRhoistingk) {              /* MR13 */
      HoistPredicateContext=1;      /* MR13 */
      MRhoisting=1;                 /* MR13 */
    };                              /* MR13 */
    if (MRhoisting && ! HoistPredicateContext) {
/***      warnNoFL("Using \"-mrhoist\" forces \"-prc on\"");    ***/
      HoistPredicateContext=1;
    };
    if (HoistPredicateContext && ! MRhoisting) {
        warnNoFL("When using predicate context (-prc on) -mrhoist on is recommended");
    }
  /* Fix lookahead depth */
  /* Compressed lookahead must always be larger than or equal to full lookahead */
  if ( CLL_k < LL_k  && CLL_k>0 )
  {
    warnNoFL("must have compressed lookahead >= full LL(k) lookahead (setting -ck to -k)");
    CLL_k = LL_k;
  }
  if ( CLL_k == -1 ) CLL_k = LL_k;
  OutputLL_k = CLL_k;
  if ( ((CLL_k-1)&CLL_k)!=0 ) { /* output ll(k) must be power of 2 */
    int n;
    for(n=1; n<CLL_k; n<<=1) {;}
    OutputLL_k = n;
  };

  if (MR_BlkErr) {
    warnNoFL("The -mrblkerr option is EXPERIMENTAL");
        if (LL_k > 1) {
        warnNoFL("The -mrblkerr option is designed only for k=1 ck=1 grammars");
        }
  };

    if ( ! ambAidDepthSpecified) {
      MR_AmbAidDepth=1;
    } else {
      if (MR_AmbAidDepth > CLL_k || MR_AmbAidDepth <= 0) {
        warnNoFL(eMsgd(
            "Ambiguity aid depth (\"-aad ...\") must be a number between 1 and max(k,ck)=%d",CLL_k));
        MR_AmbAidDepth=1;
      };
      if (MR_AmbAidDepth == 0) {
        MR_AmbAidDepth=2;
      };
    };

    if (MR_AmbAidRule != NULL) MR_AmbAidLine=atoi(MR_AmbAidRule);

  fpTrans = &(C_Trans[0]);    /* Translate to C Language */
  fpJTrans = &(C_JTrans[0]);
  init();
  lexclass(LexStartSymbol);

  readDescr();
  LastTokenCounted = TokenNum;
  RemapForcedTokens();
  if ( CannotContinue ) {cleanUp(); zzDIE;}
  if ( GenCC && no_classes_found ) fatal("required grammar class not found (exiting...)");
  if ( WarningLevel>1 && HdrAction == NULL )
     warnNoFL("no #header action was found");
  if ( FoundAtOperator && ! FoundExceptionGroup) {
     warnNoFL("found the exception operator '@' - but no exception group was found");
  };
  EpToken = addTname(EPSTR);    /* add imaginary token epsilon */
  set_orel(EpToken, &imag_tokens);

  /* this won't work for hand-built scanners since EofToken is not
   * known.  Forces EOF to be token type 1.
   */
  set_orel(EofToken, &imag_tokens);

  set_size(NumWords(TokenNum-1));

  /* compute the set of all known token types
   * It represents the set of tokens from 1 to last_token_num + the
   * reserved positions above that (if any).  Don't include the set of
   * imaginary tokens such as the token/error classes or EOF.
   */
  {
    set a;
    a = set_dup(reserved_positions);
    for (i=1; i<TokenNum; i++) { set_orel(i, &a); }
    all_tokens = set_dif(a, imag_tokens);
    set_free(a);
  }

  ComputeTokSets();       /* Compute #tokclass sets */
  CompleteTokenSetRefs();     /* Change complex nodes in syn diag */
    CompleteContextGuards();        /* MR13 */

  if ( CodeGen ) genDefFile();  /* create tokens.h */
  if ( LexGen ) genLexDescr();  /* create parser.dlg */

  if ( GenStdPccts )
  {
    FILE *f = fopen(OutMetaName(stdpccts), "w");
    if ( f==NULL ) {warnNoFL( eMsg1("can't create %s",OutMetaName(stdpccts)) );}
    else
    {
#ifdef SPECIAL_FOPEN
      special_fopen_actions(OutMetaName(stdpccts));            /* MR1 */
#endif
            if (strcmp(stdpccts,"stdpccts.h") == 0) {                /* MR10 */
            genStdPCCTSIncludeFile(f,NULL);                        /* MR10 */
            } else {                                                 /* MR10 */
            genStdPCCTSIncludeFile(f,pcctsBaseName(stdpccts));     /* MR32 */
            };
      fclose(f);
    }
  }

  buildRulePtr();         /* create mapping from rule # to RuleBlk junction */
  ComputeErrorSets();
  FoLink( (Node *)SynDiag );    /* add follow links to end of all rules */

  if ( GenCR ) GenCrossRef( SynDiag );

  if ( CodeGen )
  {
    if ( SynDiag == NULL )
    {
      warnNoFL("no grammar description recognized");
      cleanUp();
      zzDIE;
    }
    else if ( !GenCC ) {
      ErrFile = fopen(OutMetaName(ErrFileName), "w");
      require(ErrFile != NULL, "main: can't open err file");
#ifdef SPECIAL_FOPEN
                special_fopen_actions(OutMetaName(ErrFileName));   /* MR1 */
#endif
      NewSetWd();
      GenErrHdr();
      TRANS(SynDiag);     /* Translate to the target language */
      DumpSetWd();
      DumpRemainingTokSets();
      fclose( ErrFile );
    }
    else {
      strcpy(Parser_h_Name, CurrentClassName);
      strcat(Parser_h_Name, ".h");
      strcpy(Parser_c_Name, CurrentClassName);
      strcat(Parser_c_Name, CPP_FILE_SUFFIX);
      ensure_no_C_file_collisions(Parser_c_Name);
      Parser_h = fopen(OutMetaName(Parser_h_Name), "w");
      require(Parser_h != NULL, "main: can't open class Parserx.h file");
#ifdef SPECIAL_FOPEN
        special_fopen_actions(OutMetaName(Parser_h_Name));       /* MR1 */
#endif
      Parser_c = fopen(OutMetaName(Parser_c_Name), "w");
      require(Parser_c != NULL, "main: can't open class Parserx.c file");
#ifdef SPECIAL_FOPEN
        special_fopen_actions(OutMetaName(Parser_c_Name));       /* MR1 */
#endif
      GenParser_h_Hdr();
      if ( class_before_actions != NULL )
      {
        ListNode *p;
        for (p = class_before_actions->next; p!=NULL; p=p->next)
        {
          UserAction *ua = (UserAction *)p->elem;
          dumpAction( ua->action, Parser_h, 0, ua->file, ua->line, 1);
        }
      }
      GenParser_c_Hdr();
      fprintf(Parser_h, "protected:\n");  /* MR20 */
      NewSetWd();
      TRANS(SynDiag);     /* Translate to the target language */
      DumpSetWd();
      GenRuleMemberDeclarationsForCC(Parser_h, SynDiag);
      if ( class_after_actions != NULL )
      {
        ListNode *p;
        for (p = class_after_actions->next; p!=NULL; p=p->next)
        {
          UserAction *ua = (UserAction *)p->elem;
          dumpAction( ua->action, Parser_h, 0, ua->file, ua->line, 1);
        }
      }
      DumpRemainingTokSets();
      fprintf(Parser_h, "};\n");
      fprintf(Parser_h, "\n#endif /* %s_h */\n", CurrentClassName);
      fclose( Parser_h );
      fclose( Parser_c );
    }
  }

    MR_orphanRules(stderr);
    if (LTinTokenAction && WarningLevel >= 2) {
    if (GenCC) {
      warnNoFL("At least one <<action>> following a token match contains a reference to LT(...)\n      this will reference the immediately preceding token,\n      not the one which follows as is the case with semantic predicates.");
    }
      warnNoFL("At least one <<action>> following a token match contains a reference to LA(...) or LATEXT(...)\n      this will reference the immediately preceding token,\n      not the one which follows as is the case with semantic predicates.");
  }

  if ( PrintOut )
  {
    if ( SynDiag == NULL ) {warnNoFL("no grammar description recognized");}
    else PRINT(SynDiag);
  }

#ifdef DBG_LL1
#endif
  GenRemapFile();         /* create remap.h */
/* MR10 */    if (FoundGuessBlk) {
/* MR10 */      list_apply(NumericPredLabels, (void (*)(void *))report_numericPredLabels);
/* MR10 */    };

    if (InfoT && TnodesAllocated > 0) {
      if (TnodesPeak > 10000) {
        fprintf(stdout,"\nTree Nodes:  peak %dk  created %dk  lost %d\n",
                        (TnodesPeak/1000),
                        (TnodesAllocated/1000),
                        TnodesInUse-tnodes_used_in_guard_predicates_etc);
       } else {
         fprintf(stdout,"\nTree Nodes:  peak %d  created %d  lost %d\n",
                        TnodesPeak,
                        TnodesAllocated,
                        TnodesInUse-tnodes_used_in_guard_predicates_etc);
       };
    };
    if (InfoF) {
      DumpFcache();
    };
    if (MR_skipped_e3_report) {
      fprintf(stderr,"note: use -e3 to get exact information on ambiguous tuples\n");
    };
    if (MR_BadExprSets != 0) {
      fprintf(stderr,"note: Unreachable C or C++ code was generated for empty expression sets,\n");
      fprintf(stderr,"        probably due to undefined rules or infinite left recursion.\n");
      fprintf(stderr,"      To locate: search the generated code for \"empty set expression\"\n");
    };
    if (MR_AmbAidRule != NULL && MR_matched_AmbAidRule==0) {
      RuleEntry *q = (RuleEntry *) hash_get(Rname,MR_AmbAidRule);
      if (MR_AmbAidLine == 0 && q == NULL) {
         warnNoFL(eMsg2("there is no rule \"%s\" so \"-aa %s\" will never match",
                                                        MR_AmbAidRule,MR_AmbAidRule));
      } else {
        warnNoFL(eMsg1("there was no ambiguity that matched \"-aa %s\"",MR_AmbAidRule));
      };
    };
    if (AlphaBetaTrace) {

      if (MR_AlphaBetaMessageCount == 0) {
         fprintf(stderr,"note: there were no messages about \"(alpha)? beta\" blocks added to the generated code\n");
      } else {
         fprintf(stderr,"note: there were %d messages about \"(alpha)? beta\" blocks added to the generated code\n",
                    MR_AlphaBetaMessageCount);
      }

      if (set_null(MR_CompromisedRules)) {
         fprintf(stderr,"note: the list of rules with compromised follow sets is empty\n");
      } else {
         fprintf(stderr,"note: the following is a list of rules which *may* have incorrect\n");
         fprintf(stderr,"      follow sets computed as a result of an \"(alpha)? beta\" block\n");
         fprintf(stderr,"\n");
         MR_dumpRuleSet(MR_CompromisedRules);
         fprintf(stderr,"\n");
      }
    }
  cleanUp();
  exit(PCCTS_EXIT_SUCCESS);
    return 0;           /* MR11 make compilers happy */
}

static void init( )
{
  SignalEntry *q;

  Tname = newHashTable();
  Rname = newHashTable();
  Fcache = newHashTable();
  Tcache = newHashTable();
  Sname = newHashTable();
    Pname = newHashTable();     /* MR11 */

  /* Add default signal names */
  q = (SignalEntry *)hash_add(Sname,
                "NoViableAlt",
                (Entry *)newSignalEntry("NoViableAlt"));
  require(q!=NULL, "cannot alloc signal entry");
  q->signum = sigNoViableAlt;
  q = (SignalEntry *)hash_add(Sname,
                "MismatchedToken",
                (Entry *)newSignalEntry("MismatchedToken"));
  require(q!=NULL, "cannot alloc signal entry");
  q->signum = sigMismatchedToken;
  q = (SignalEntry *)hash_add(Sname,
                "NoSemViableAlt",
                (Entry *)newSignalEntry("NoSemViableAlt"));
  require(q!=NULL, "cannot alloc signal entry");
  q->signum = sigNoSemViableAlt;

  reserved_positions = empty;
  all_tokens = empty;
  imag_tokens = empty;
  tokclasses = empty;
  TokenStr = (char **) calloc(TSChunk, sizeof(char *));
  require(TokenStr!=NULL, "main: cannot allocate TokenStr");
  FoStack = (int **) calloc(CLL_k+1, sizeof(int *));
  require(FoStack!=NULL, "main: cannot allocate FoStack");
  FoTOS = (int **) calloc(CLL_k+1, sizeof(int *));
  require(FoTOS!=NULL, "main: cannot allocate FoTOS");
  Cycles = (ListNode **) calloc(CLL_k+1, sizeof(ListNode *));
  require(Cycles!=NULL, "main: cannot allocate Cycles List");
    MR_CompromisedRules=empty;  /* MR14 */
}

static void help( )
{
  Opt *p = options;
  fprintf(stderr, "antlr [options] f1 f2 ... fn\n");
  while ( *(p->option) != '*' )
  {
    fprintf(stderr, "    %-9s%s   %s\n",
            p->option,
            (p->arg)?"___":"   ",
            p->descr);
    p++;
  }
}

/**
 * The RulePtr array is filled in here.  RulePtr exists primarily
 * so that sets of rules can be maintained for the FOLLOW caching
 * mechanism found in rJunc().  RulePtr maps a rule num from 1 to n
 * to a pointer to its RuleBlk junction where n is the number of rules.
 */
static void buildRulePtr( )
{
  int r=1;
  Junction *p  = SynDiag;
  RulePtr = (Junction **) calloc(NumRules+1, sizeof(Junction *));
  require(RulePtr!=NULL, "cannot allocate RulePtr array");

  while ( p!=NULL )
  {
    require(r<=NumRules, "too many rules???");
    RulePtr[r++] = p;
    p = (Junction *)p->p2;
  }
}

void dlgerror(const char *s)
{
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " lexical error: %s (text was '%s')\n",
          ((s == NULL) ? "Lexical error" : s), zzlextext);
}

void readDescr()
{
  zzerr = dlgerror;
  input = NextFile();
  if ( input==NULL ) fatal("No grammar description found (exiting...)");
  ANTLR(grammar(), input);
    tnodes_used_in_guard_predicates_etc=TnodesInUse;    /* MR10 */
}

FILE *NextFile()
{
  FILE *f;

  for (;;)
  {
    CurFile++;
    if ( CurFile >= NumFiles ) return(NULL);
    if ( ci_strequ(FileStr[CurFile],"stdin")) return stdin;
    f = fopen(FileStr[CurFile], "r");
    if ( f == NULL )
    {
      warnNoFL( eMsg1("file %s doesn't exist; ignored", FileStr[CurFile]) );
    }
    else
    {
      return(f);
    }
  }
}

/**
 * Return a string corresponding to the output file name associated
 * with the input file name passed in.
 *
 * Observe the following rules:
 *
 *    f.e   --> f".c"
 *    f   --> f".c"
 *    f.    --> f".c"
 *    f.e.g --> f.e".c"
 *
 * Where f,e,g are arbitrarily long sequences of characters in a file
 * name.
 *
 * In other words, if a ".x" appears on the end of a file name, make it
 * ".c".  If no ".x" appears, append ".c" to the end of the file name.
 *
 * C++ mode using .cpp not .c.
 *
 * Use malloc() for new string.
 */

char *outname(char *fs)
{
    if ( GenCC) {
      return outnameX(fs,CPP_FILE_SUFFIX);
    } else {
      return outnameX(fs,".c");
    };
}

char *outnameX( char *fs ,char *suffix)
{
  static char buf[MaxFileName+1];
  char *p;
  require(fs!=NULL&&*fs!='\0', "outname: NULL filename");

  p = buf;
  strcpy(buf, fs);
  while ( *p != '\0' )  {p++;}      /* Stop on '\0' */
  while ( *p != '.' && p != buf ) {--p;}  /* Find '.' */
  if ( p != buf ) *p = '\0';        /* Found '.' */
  require(strlen(buf) + 2 < (size_t)MaxFileName, "outname: filename too big");
    strcat(buf,suffix);
  return( buf );
}

void fatalFL( char *err_, char *f, int l )
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " %s\n", err_);
  cleanUp();
  exit(PCCTS_EXIT_FAILURE);
}

void fatal_intern( char *err_, char *f, int l )
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " #$%%*&@# internal error: %s\n", err_);
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " [complain to nearest government official\n");
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, "  or send hate-mail to parrt@parr-research.com;\n");
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, "  please pray to the ``bug'' gods that there is a trival fix.]\n");
  cleanUp();
  exit(PCCTS_EXIT_FAILURE);
}

void cleanUp( void )
{
  if ( DefFile != NULL) fclose( DefFile );
}

/**
 * Sprintf up to 3 strings.
 * \param s format
 * \param a1 first string
 * \param a2 second string
 * \param a3 third string
 * \return a statically allocated string (don't free it). Each call of
 * this function will override previous calls.
 */
char *eMsg3( char *s, char *a1, char *a2, char *a3 )
{
  static char buf[250]; /* DANGEROUS as hell !!!!!! */

  sprintf(buf, s, a1, a2, a3);
  return( buf );
}

/** sprintf a decimal */
char *eMsgd( char *s, int d )
{
  static char buf[250]; /* DANGEROUS as hell !!!!!! */

  sprintf(buf, s, d);
  return( buf );
}

char *eMsgd2( char *s, int d1,int d2)
{
  static char buf[250]; /* DANGEROUS as hell !!!!!! */

  sprintf(buf, s, d1, d2);
  return( buf );
}

void s_fprT(FILE *f, set e)
{
  register unsigned *p;
  unsigned *q;

  if ( set_nil(e) ) return;
  if ( (q=p=set_pdq(e)) == NULL ) fatal_internal("Can't alloc space for set_pdq");
  fprintf(f, "{");
  while ( *p != nil )
  {
    fprintf(f, " %s", TerminalString(*p));
    p++;
  }
  fprintf(f, " }");
  free((char *)q);
}

/** Return the token name or regular expression for a token number. */
char *TerminalString( int token )
{
  int j;
  static char imag_name[20];

  /* look in all lexclasses for the token */
  if ( TokenString(token) != NULL ) return TokenString(token);
  for (j=0; j<NumLexClasses; j++)
  {
    lexmode(j);
    if ( ExprString(token) != NULL ) return ExprString(token);
  }

    if (1) {
      sprintf(imag_name,"UnknownToken#%d",token);           /* MR13 */
      return imag_name;                                     /* MR13 */
    }

  require(j<NumLexClasses, eMsgd("No label or expr for token %d",token));
  return "invalid";
}

                    /* S i m p l e  I n t  S t a c k */

void pushint(int i)
{
  require(isp>0, "pushint: stack overflow");
  istack[--isp] = i;
}

int popint()
{
  require(isp<MAX_INT_STACK, "popint: stack underflow");
  return istack[isp++];
}

int istacksize( )
{
  return MAX_INT_STACK-isp;
}

void istackreset( )
{
  isp = MAX_INT_STACK;
}

int istackempty( )
{
  return isp==MAX_INT_STACK;
}

int topint( )
{
  require(isp<MAX_INT_STACK, "topint: stack underflow");
  return istack[isp];
}

void ProcessArgs( int argc, char **argv, Opt *options )
{
  Opt *p;
  require(argv!=NULL, "ProcessArgs: command line NULL");

  while ( argc-- > 0 )
  {
    p = options;
    while ( p->option != NULL )
    {
      if ( strcmp(p->option, "*") == 0 ||
         ci_strequ(p->option, *argv) == 1 )
      {
        if ( p->arg )
        {
/* MR9  26-Sep-97   Check for argv valid                */
                    if (argc-- > 0) {
                (*p->process)( *argv, *(argv+1) );
              argv++;
                    } else {
                        fprintf(stderr,"error: required argument for option %s omitted\n",*argv);
                        exit(PCCTS_EXIT_FAILURE);
                    };
        }
        else
          (*p->process)( *argv );
        break;
      }
      p++;
    }
    argv++;
  }
}

static void CompleteContextGuards()
{
    ListNode *      p;
    Predicate *     pred;

    if (ContextGuardPredicateList == NULL) return;

    for (p=ContextGuardPredicateList->next; p != NULL; p=p->next) {
      pred=(Predicate *)p->elem;
      recomputeContextGuard(pred);
    }
}

/**
 * Go back into the syntax diagram and compute all meta tokens; i.e.
 * turn all '.', ranges, token class refs etc... into actual token sets
 */
static void CompleteTokenSetRefs()
{
  ListNode *p;

  if ( MetaTokenNodes==NULL ) return;
  for (p = MetaTokenNodes->next; p!=NULL; p=p->next)
  {
    set a,b;

    TokNode *q = (TokNode *)p->elem;
    if ( q->wild_card )
    {
      q->tset = all_tokens;
    }
    else if ( q->tclass!=NULL )
    {
      if ( q->complement ) q->tset = set_dif(all_tokens, q->tclass->tset);
      else q->tset = q->tclass->tset;
    }
    else if ( q->upper_range!=0 )
    {
      /* we have a range on our hands: make a set from q->token .. q->upper_range */
      int i;
      a = empty;
      for (i=q->token; i<=q->upper_range; i++) { set_orel(i, &a); }   /* MR13 */

/* MR13 */    if (q->complement) {
/* MR13 */      q->tset = set_dif(all_tokens, a);
/* MR13 */        set_free(a);
/* MR13 */      } else {
/* MR13 */        q->tset = a;
/* MR13 */      }

        }

    /* at this point, it can only be a complemented single token */
    else if ( q->complement )
    {
      a = set_of(q->token);
      b = set_dif(all_tokens, a);
      set_free(a);
      q->tset=b;
    }
    else fatal("invalid meta token");
  }
}

/* MR10: Jeff Vincent
   MR10: Changed to remove directory information from n only if
   MR10: if OutputDirectory was changed by user (-o option)
*/

char *OutMetaName(char *n)
{
    static char *dir_sym = DirectorySymbol;
    static char newname[MaxFileName+1];
    char *p;

  /* If OutputDirectory is same as TopDirectory (platform default) then leave n alone. */
    if (strcmp(OutputDirectory, TopDirectory) == 0)   /* TopDirectory is "." on Unix. */
    return n;

  /* p will point to filename without path information */
  if ((p = strrchr(n, *dir_sym)) != NULL)       /* Directory symbol is "/" on Unix. */
    p++;
  else
    p = n;

  /* Copy new output directory into newname[] */
  strcpy(newname, OutputDirectory);

  /* if new output directory does not have trailing dir_sym, add it! */
  if (newname[strlen(newname)-1] != *dir_sym) {
    strcat(newname, dir_sym);
  }
  strcat(newname, p);
  return newname;
}

char *pcctsBaseName(char *n) /* MR32 */
{
    static char newname[MaxFileName+1];
    static char* dir_sym = DirectorySymbol;
    int count = 0;
    char *p;

    p = n;

    while ( *p != '\0' )  {p++;}                    /* go to end of string */
    while ( (*p != *dir_sym) && (p != n) ) {--p;}   /* Find last DirectorySymbol */
    while ( *p == *dir_sym) p++;                    /* step forward if we're on a dir symbol */
    while ( *p != '\0' && *p != '.')
    {
        newname[count++] = *p;
        p++;
    }                                               /* create a new name */
    newname[count] = '\0';
    return newname;
}

static void ensure_no_C_file_collisions(char *class_c_file)
{
  int i;

  for (i=0; i<NumFiles; i++)
  {

#ifdef PCCTS_CASE_INSENSITIVE_FILE_NAME
    /* assume that file names are case insensitive */
    if ( STRICMP(outname(FileStr[i]), class_c_file)==0 )
#else
    if ( strcmp(outname(FileStr[i]), class_c_file)==0 )
#endif
    {
      fatal(eMsg1("class def output file conflicts with parser output file: %s",
            outname(FileStr[i])));
    }
  }
}

void warnNoFL(char *err)
{
  fprintf(stderr, "warning: %s\n", err);
}

void warnFL(char *err,char *f,int l)
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " warning: %s\n", err);
}

void warn(char *err)
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " warning: %s\n", err);
}

void warnNoCR( char *err )
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " warning: %s", err);
}

void errNoFL(char *err)
{
  fprintf(stderr, "error: %s\n", err);
}

void errFL(char *err,char *f,int l)
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " error: %s\n", err);
}

void err(char *err)
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " error: %s\n", err);
}

void errNoCR( char *err )
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " error: %s", err);
}

UserAction *newUserAction(char *s)
{
  UserAction *ua = (UserAction *) calloc(1, sizeof(UserAction));
  require(ua!=NULL, "cannot allocate UserAction");

  ua->action = (char *) calloc(strlen(LATEXT(1))+1, sizeof(char));
  strcpy(ua->action, s);
  return ua;
}

/* Added by TJP September 1994 */
/* Take in file.h and return file_h; names w/o '.'s are left alone */
char *gate_symbol(char *name)
{
  static char buf[100];
  char *p;
  sprintf(buf, "%s", name);

  for (p=buf; *p!='\0'; p++)
  {
    if ( *p=='.' ) *p = '_';
  }
  return buf;
}

char *makeAltID(int blockid, int altnum)
{
  static char buf[100];
  char *p;
  sprintf(buf, "_blk%d_alt%d", blockid, altnum);
  p = (char *)malloc(strlen(buf)+1);
  strcpy(p, buf);
  return p;
}
