/*
 * main.c
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to SORCERER -- SORCERER is in the public
 * domain.  An individual or company may do whatever they wish with
 * source code distributed with SORCERER or the code generated by
 * SORCERER, including the incorporation of SORCERER, or its output, into
 * commerical software.
 *
 * We encourage users to develop software with SORCERER.  However, we do
 * ask that credit is given to us for developing SORCERER.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like SORCERER and have developed a nice tool with the
 * output, please mention that you developed it using SORCERER.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * SORCERER 1.00B
 * Terence Parr
 * AHPCRC, University of Minnesota
 * 1992-2001
 */


#include "constants.h"
#include "sym.h"
#include "proto.h"
#include "hash.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INT_STACK 50
static int istack[MAX_INT_STACK];       /* Int stack */
static int isp = MAX_INT_STACK;

static int DontAcceptStdin = 0;
static int DontAcceptFiles = 0;

typedef struct _Opt {
    char *option;
    int  arg;
    void (*process)();
    char *descr;
} Opt;


static int cistrequ(char *a,char *b)
{
  for ( ;*a != 0 && *b != 0; a++, b++) {
    if (toupper(*a) != toupper(*b)) return 0;
  }
  return (*a == *b);
}

static void ProcessArgs(int, char **, Opt *);

static void
pOut( char *s, char *t )
{
  OutputDirectory = t;
}

static void
pFile( char *s )
{
  if ( *s=='-' ) { warningNoFL("invalid option: '%s'",s); return; }
  if ( DontAcceptFiles )
  {
    warningNoFL("file '%s' ignored as '-' (stdin option) was specified first",s);
    return;
  }

  require(NumFiles<MaxNumFiles,"exceeded max # of input files");
  FileStr[NumFiles++] = s;
  DontAcceptStdin = 1;
}

static void
pDToks( char *s, char *t )
{
  if ( !GenCPP )
  {
    warningNoFL("-def-tokens valid with C++ interface only; ignored");
    return;
  }
  else {
    def_tokens = 1;
  }
}

static void
pDToksFile( char *s, char *t )
{
  if ( GenCPP )
  {
    warningNoFL("-def-tokens-file not valid with C++ interface; ignored");
    return;
  }
  if ( Inline )
  {
    warningNoFL("-def-tokens-file conflicts with -inline; ignored");
  }
  else {
    def_token_file = t;
    if ( def_token_file == NULL )
    {
      warningNoFL("don't you want filename with that -def-tokens-file?; ignored");
    }
  }
}

static void
pPrefix( char *s, char *t )
{
  Prefix = t;
  if ( GenCPP ) {
    warningNoFL("-prefix conflicts with C++ interface; ignored");
    Prefix = "";
    return;
  }

  if ( Prefix == NULL )
  {
    warningNoFL("don't you want string with that -prefix?; ignored");
    Prefix = "";
  }
}

static void
pProtoFile( char *s, char *t )
{
  if ( GenCPP )
    {warningNoFL("-proto-file not valid with C++ interface; ignored");}
  if ( t==NULL )
    {warningNoFL("don't you want filename with that -proto-file?; ignored");}
  else {
    GenProtoFile = t;
  }
}

static void
pstdin()
{
  if ( DontAcceptStdin )
  {
    warningNoFL("'-' (stdin) ignored as files were specified first");
    return;
  }
  if ( GenCPP )
    {warningNoFL("'-' (stdin) cannot be used with C++ interface; ignored");}

  require(NumFiles<MaxNumFiles,"exceeded max # of input files");
  FileStr[NumFiles++] = "stdin";
  DontAcceptFiles = 1;
}

static void
pGuts()
{
  print_guts = 1;
}

static void
pTrans()
{
  transform = 1;
}

static void
pInline()
{
  if ( def_token_file!=NULL )
  {
    warningNoFL("-inline conflicts with -def-tokens; ignored");
  }
  else if ( GenCPP )
  {
    warningNoFL("-inline conflicts with C++ interface; ignored");
  }
  else Inline = 1;
}

static void
pCPP()
{
  GenCPP = 1;
}

/* MR23 */

static void
pNoCtor()
{
  NoCtor = 1;
  if ( ! GenCPP )
    {warningNoFL("-noctor needs -CPP option; ignored");}
}

Opt options[] = {
    { "-CPP",    0, pCPP,  "Generate C++ output"},
    { "-def-tokens", 0, pDToks,"Define ref'd tokens w/unique integer (C++ interface)"},
    { "-def-tokens-file", 1, pDToksFile,"Define ref'd tokens w/unique integer (put in file)"},
    { "-guts", 0,   pGuts,"Print out a bunch of internal data structures"},
    { "-inline", 0,   pInline, "Gen only actions and functions for given rules"},
    { "-noctor", 0,   pNoCtor, "Do not generate a blank constructor"}, /* MR23 */
    { "-prefix", 1,   pPrefix, "Prefix all globally visible symbols"},
    { "-proto-file", 1, pProtoFile, "Put all prototypes for rule functions in this file"},
    { "-out-dir", 1,  pOut, "Directory where all output files go (default=\".\")"},
    { "-transform", 0,  pTrans, "Transformation mode; read/write to different pointers"},
    { "-",  0,      pstdin, "take input from stdin"},
    { "*",  0,      pFile,  "" }, /* anything else is a file */
    { NULL, 0, NULL, NULL }
};

int main(int argc, char *argv[])
{
  AST *root;
#ifdef THINK_C
#include <console.h>
  argc = ccommand (&argv);
#endif

  fprintf(stderr,
    "Sorcerer -  A simple tree-parser generator  Version %s  1992-2001\n",
    VersionText);

  if ( argc == 1 ) { help(); return EXIT_SUCCESS; }
  ProcessArgs(argc-1, &(argv[1]), options);

  symbols = newHashTable();
  token_dict = (char **) calloc(token_table_size, sizeof(char *));
  require(token_dict != NULL, "main: can't create token_dict");

  output = stdout;
  input = NextFile();
  require(input!=NULL, "No grammar description found (exiting...)");
  root = read_sor_desc(input);

  if ( root!=NULL ) gen(root);

  if ( found_error )
  {
    fprintf(stderr, "no parser generated due to parsing errors; exiting...\n");
    return EXIT_FAILURE;
  }

  if ( GenCPP ) {
    GenCPPClassHeader();
    GenCPPClassCode();
  }

  if ( !UserDefdTokens ) gen_tokens_file();
  else if ( def_token_file!=NULL ) {
    warningNoFL("tokens file not generated; it conflicts with use of #tokdefs");
  }

  if ( GenProtoFile!=NULL )
  {
    FILE *ProtoFILE;
    ProtoFILE = fopen(OutMetaName(GenProtoFile), "w");
    if ( ProtoFILE==NULL ) {warningNoFL("Can't open prototype file '%s'; ignored",GenProtoFile);}
    else {
#ifdef SPECIAL_FOPEN
                        special_fopen_actions(OutMetaName(GenProtoFile));
#endif
      gen_info_hdr( ProtoFILE );
      if ( header_action!=NULL )
        dumpAction(header_action, ProtoFILE, 0, -1, 0, 1);
      GenRulePrototypes( ProtoFILE, 0 );
      fclose(ProtoFILE);
    }
  }
  if ( found_error ) {return EXIT_FAILURE;}
  else return EXIT_SUCCESS;
}

void
help(void)
{
  Opt *p = options;
  static char buf[MaxRuleName+1];

  fprintf(stderr, "sor [options] f1.sor ... fn.sor\n");
  while ( p->option!=NULL && *(p->option) != '*' )
  {
    buf[0]='\0';
    if ( p->arg ) sprintf(buf, "%s ___", p->option);
    else strcpy(buf, p->option);
    fprintf(stderr, "    %-20s   %s\n", buf, p->descr);
    p++;
  }
}

FILE *
NextFile( void )
{
  FILE *f;

  for (;;)
  {
    if ( CurFile+1 >= NumFiles ) return(NULL);
    CurFile++;
    if ( cistrequ(FileStr[CurFile],"stdin")) return stdin;
    f = fopen(FileStr[CurFile], "r");
    if ( f == NULL )
    {
      warningNoFL("file %s doesn't exist; ignored", FileStr[CurFile]);
    }
    else
    {
      return(f);
    }
  }
}

void
pushint( int i )
{
    require(isp>0, "pushint: stack overflow");
    istack[--isp] = i;
}

int
popint( void )
{
    require(isp<MAX_INT_STACK, "popint: stack underflow");
    return istack[isp++];
}

int
istacksize( void )
{
    return MAX_INT_STACK-isp;
}

void
istackreset( void )
{
    isp = MAX_INT_STACK;
}

int istackempty( void )
{
    return isp==MAX_INT_STACK;
}

int topint(void)
{
    require(isp<MAX_INT_STACK, "topint: stack underflow");
    return istack[isp];
}


/**
 * add an element to a list.
 *
 * Any non-empty list has a sentinel node whose 'elem' pointer is really
 * a pointer to the last element.  (i.e. length(list) = #elemIn(list)+1).
 * Elements are appended to the list.
 */
void
list_add( ListNode **list, void *e )
{
  ListNode *p, *tail;
  require(e!=NULL, "list_add: attempting to add NULL list element");

  p = newListNode;
  require(p!=NULL, "list_add: cannot alloc new list node");
  p->elem = e;
  if ( *list == NULL )
  {
    ListNode *sentinel = newListNode;
    require(sentinel!=NULL, "list_add: cannot alloc sentinel node");
    *list=sentinel;
    sentinel->next = p;
    sentinel->elem = (char *)p;   /* set tail pointer */
  }
  else                /* find end of list */
  {
    tail = (ListNode *) (*list)->elem;  /* get tail pointer */
    tail->next = p;
    (*list)->elem = (char *) p;   /* reset tail */
  }
}

static void
ProcessArgs( int argc, char **argv, Opt *options )
{
  Opt *p;
  require(argv!=NULL, "ProcessArgs: command line NULL");

  while ( argc-- > 0 )
  {
    p = options;
    while ( p->option != NULL )
    {
      if ( strcmp(p->option, "*") == 0 ||
         cistrequ(p->option, *argv))
      {
        if ( p->arg )
        {
          (*p->process)( *argv, *(argv+1) );
          argv++;
          argc--;
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

/**
 * Given a string, this function allocates and returns a pointer to a
 * hash table record of size 'sz' whose "str" pointer is reset to a position
 * in the string table.
 */
Entry *
newEntry( char *text, int sz )
{
  Entry *p;
  require(text!=NULL, "new: NULL terminal");

  if ( (p = (Entry *) calloc(1,sz)) == 0 )
  {
    fatal("newEntry: out of memory for terminals\n");
    exit(1);
  }
  p->str = mystrdup(text);

  return(p);
}

void
token_association(int token_type, char *text)
{
  if ( token_type >= token_table_size ) /* overflow? */
  {
    char **p;
    int i, more;

    more = 100;
    token_table_size += more;
    token_dict = (char **) realloc(token_dict, token_table_size*sizeof(char *));
    require(token_dict != NULL, "token_association: can't extend token_dict");
    for (p= &token_dict[token_table_size-more],i=1; i<=more; i++) *p++ = NULL;
  }
  token_dict[token_type] = text;
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
 *    f.e.t --> f.e".c"
 *
 * Where f,e,t are arbitrarily long sequences of characters in a file
 * name.
 *
 * In other words, if a ".x" appears on the end of a file name, make it
 * ".c".  If no ".x" appears, append ".c" to the end of the file name.
 *
 * Use malloc() for new string.
 */
char *
outname( char *fs )
{
  static char buf[MaxFileName+1];
  char *p;

  p = buf;
  strcpy(buf, fs);
  while ( *p != '\0' )  {p++;}      /* Stop on '\0' */
  while ( *p != '.' && p != buf ) {--p;}  /* Find '.' */
  if ( p != buf ) *p = '\0';        /* Found '.' */
  require(strlen(buf) + 2 < (size_t)MaxFileName, "outname: filename too big");
  if ( GenCPP ) strcat(buf, CPP_FILE_SUFFIX);
  else strcat(buf, ".c");
  return( buf );
}

void
ensure_no_C_file_collisions(char *class_c_file)
{
  int i;

  for (i=0; i<NumFiles; i++)
  {
    if ( strcmp(outname(FileStr[i]), class_c_file)==0 )
    {
      fatal(eMsg("class def output file conflicts with parser output file: %s",
            outname(FileStr[i])));
    }
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
    if (strcmp(OutputDirectory, TopDirectory) == 0)
    return n;

  /* p will point to filename without path information */
  if ((p = strrchr(n, *dir_sym)) != NULL)
    p++;
  else
    p = n;

  /* Copy new output directory into newname[] */
  strcpy(newname, OutputDirectory);

  /* if new output directory does not have trailing dir_sym, add it! */
  if (newname[strlen(newname)-1] != *dir_sym)
    strcat(newname, dir_sym);

  /* contatenate FILE NAME ONLY to new output directory */
  strcat(newname, p);

  return newname;
}

void
set_fprint( FILE *f, set e )
{
  int t;

  if ( set_nil(e) ) return;
  fprintf(f, "{");
  while ( !set_nil(e) )
  {
    t = set_int(e);
    set_rm(t, e);
    fprintf(f, " %s", token_dict[t]);
  }
  fprintf(f, " }");
}

/** Given a list of ANSI-style parameter declarations, print out a
 * comma-separated list of the symbols (w/o types).
 * Basically, we look for a comma, then work backwards until start of
 * the symbol name.  Then print it out until 1st non-alnum char.  Now,
 * move on to next parameter.
 */
void
DumpListOfParmNames( char *pdecl, FILE *output )
{
  int firstTime = 1, done = 0;
  require(output!=NULL, "DumpListOfParmNames: NULL parm");

  if ( pdecl == NULL ) return;
  while ( !done )
  {
    if ( !firstTime ) putc(',', output);
    done = DumpNextNameInDef(&pdecl, output);
    firstTime = 0;
  }
}

/** given a list of parameters or return values, dump the next
 * name to output.  Return 1 if last one just printed, 0 if more to go.
 */
int
DumpNextNameInDef( char **q, FILE *output )
{
  char *p = *q;   /* where did we leave off? */
  int done=0;

  while ( *p!='\0' && *p!=',' ) p++;    /* find end of decl */
  if ( *p == '\0' ) done = 1;
  while ( !isalnum(*p) && *p!='_' ) --p;  /* scan back until valid var character */
  while ( isalnum(*p) || *p=='_' ) --p; /* scan back until beginning of variable */
  p++;            /* move to start of variable */
  while ( isalnum(*p) || *p=='_'  ) {putc(*p, output); p++;}
  while ( *p!='\0' && *p!=',' ) p++;    /* find end of decl */
  p++;        /* move past this parameter */

  *q = p;       /* record where we left off */
  return done;
}

/**
 * Given a list of ANSI-style parameter declarations, dump K&R-style
 * declarations, one per line for each parameter.  Basically, convert
 * comma to semi-colon, newline.
 */
void
DumpOldStyleParms( char *pdecl, FILE *output )
{
  require(output!=NULL, "DumpOldStyleParms: NULL parm");

  if ( pdecl == NULL ) return;
  while ( *pdecl != '\0' )
  {
    if ( *pdecl == ',' )
    {
      pdecl++;
      putc(';', output); putc('\n', output);
      while ( *pdecl==' ' || *pdecl=='\t' || *pdecl=='\n' ) pdecl++;
    }
    else {putc(*pdecl, output); pdecl++;}
  }
  putc(';', output);
  putc('\n', output);
}

/** Take in a type definition (type + symbol) and print out type only */
void
DumpType( char *s, FILE *f, char *file, int line )
{
  char *p, *end;
  require(s!=NULL, "DumpType: invalid type string");

  p = &s[strlen(s)-1];    /* start at end of string and work back */
  /* scan back until valid variable character */
  while ( !isalnum(*p) && *p!='_' ) --p;
  /* scan back until beginning of variable */
  while ( isalnum(*p) || *p=='_' ) --p;
  if ( p<=s )
  {
    warning("invalid parameter/return value: '%s'", file, line, s);
    return;
  }
  end = p;          /* here is where we stop printing alnum */
  p = s;
  while ( p!=end ) {putc(*p, f); p++;} /* dump until just before variable */
  while ( *p!='\0' )           /* dump rest w/o variable */
  {
    if ( !isalnum(*p) && *p!='_' ) putc(*p, f);
    p++;
  }
}

char *
actiondup(char *s)
{
  char *p = (char *) malloc(strlen(s)+1);
  require(p!=NULL, "actiondup: no memory");
  strcpy(p, s);
  return p;
}

#ifdef OLD
RefVarRec *
newRefVarRec(char *t,char *lab, char *init)
{
  RefVarRec *p;
  SymEntry *q;

  q = (SymEntry *) hash_get(symbols, lab);
  if ( q==NULL )
  {
    q = (SymEntry *) hash_add(symbols, lab, (Entry *) newSymEntry(lab));
    q->token = REFVAR;
    q->defined = 1;
  }
  else err(eMsg("Reference variable clashes with %s: '%s'", zztokens[q->token], lab));

  p = (RefVarRec *) calloc(1, sizeof(RefVarRec));
  require(p!=NULL, "newRefVarRec: no memory");
  strcpy(p->type, t);
  strcpy(p->label, lab);
  strcpy(p->init, init);
  return p;
}
#endif

/**
 * From one of the following ref var defs, create a record to track
 * the info:
 *
 *      @(int *p);
 *      @(int *p=NULL);   -- initialized
 *      @(static int *p); -- global
 *
 * The 'def' pointer should point to the first char after the '('
 */
RefVarRec *
refVarRec(char *def)
{
  RefVarRec *p;
  char *s,*decl;

  p = (RefVarRec *) calloc(1, sizeof(RefVarRec));
  require(p!=NULL, "newRefVarRec: no memory");

  /* find first word; must be type or "static" */
  s = def;
  while ( isalpha(*s) ) s++;
  if ( strncmp(def, "static", s-def)==0 ) p->global = 1;
  else s = def;

  /* now get type from s position until '=' or ')' */
  decl = s;
  while ( *s!='=' && *s!='\0' ) s++;
  if ( *s=='=' ) {
    /* there is an initializer */
    strcpy(p->init, s+1);
    *s='\0';
  }
  strcpy(p->decl, decl);
  strcpy(p->var, id_in_decl(decl));
  return p;
}

/**
 * given a decl (type + id) return a ptr to the id; nondestructive to 'decl'.
 * Just scan backwards from the end looking for the start of the first id.
 */
char *
id_in_decl( char *decl )
{
  static char id[MaxAtom+1];
  char *p = &(decl[strlen(decl)-1]);
  char *end, *start;
  require(decl!=NULL, "id_in_decl: NULL decl");
  require(strlen(decl)>(size_t)0, "id_in_decl: empty decl");

  /* scan back until valid var character */
    while ( !isalnum(*p) || *p=='_' ) --p;
  end = p+1;
  /* scan back until beginning of variable */
    while ( isalnum(*p) || *p=='_' ) --p;
    p++;                        /* move to start of variable */
  start = p;
  strncpy(id, start, end-start);
  id[end-start] = '\0';
  return id;
}

/** Take in a decl (type + id) and print it out with a prefix on id */
void
dump_prefixed_decl( char *prefix, char *s, FILE *f )
{
    char *p, *id;
    require(s!=NULL, "invalid type string");

    p = &s[strlen(s)-1];        /* start at end of string and work back */
    /* scan back until valid variable character */
    while ( !isalnum(*p) && *p!='_' ) --p;
    /* scan back until beginning of variable */
    while ( isalnum(*p) || *p=='_' ) --p;
  require(p>s, "invalid decl");
  p++;
    id = p;                    /* here is where id is */
    p = s;
    while ( p!=id ) {fputc(*p, f); p++;} /* dump until just before variable */
  fprintf(f, prefix);
    while ( isspace(*p) ) p++;
    while ( *p!='\0' )                   /* dump rest */
    {
        fputc(*p, f);
        p++;
  }
}

/**
 * Convert
 *
 *  0, "#( RangeOp #(Minus %1:IConst %2:Var) #(Plus %3:Var %4:Var) )",
 *           t, &w, &x, &y, &z);
 *
 * to
 *
 *  0, "#( 6 #(5 %1:4 %2:3) #(1 %3:3 %4:3) )",
 *           t, &w, &x, &y, &z);
 */
char *
cvt_token_str(char *prefix, char *s)
{
  SymEntry *q;
  char *b, *e, *p;
  char tname[MaxRuleName+1];
  char duh[MaxRuleName+1];
  static char call[MaxAtom+1];
  require(prefix!=NULL&&s!=NULL, "cvt_token_str: NULL string");
  require(s[0]!='\0', "cvt_token_str: empty string");

/*  printf("cvt incoming: '%s'\n", s);*/

  strcpy(call, prefix);
  p = &call[strlen(prefix)];

  while ( *s!='"' ) *p++ = *s++;  /* ignore until string start */
  *p++ = *s++;

  do
  {
    while ( !isalpha(*s) )
    {
      if ( *s=='"' ) { *p='\0'; goto done; }
      *p++ = *s++;  /* ignore until a token name */
    }
    *p='\0';
    b = s;
    while ( isalpha(*s) ) s++;  /* ignore until end of token name */
    e = s-1;
    strncpy(tname, b, e-b+1);
    tname[e-b+1] = '\0';

/*    printf("looking up %s\n", tname);*/
    q = (SymEntry *) hash_get(symbols, tname);
    if ( q==NULL )
    {
      warningNoFL("call to ast_scan() has reference to unknown token: '%s'",tname);
    }
    else
    {
      sprintf(duh, "%d", q->token_type);
      strcpy(p, duh);
      p += strlen(duh);
      *p='\0';
    }
  } while ( *s!='\0' && *s!='"' );
done:
  strcat(call, s);
/*  printf("cvt: result is '%s'\n", call);*/
  return call;
}
