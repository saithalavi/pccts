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
 * \file proto.h
 * function prototypes
 */

#include <stdio.h>
// for Entry
#include "hash.h"

                           /* V a r i a b l e s */

extern int tp;
extern Junction *SynDiag;
extern char Version[];
extern char VersionDef[];
extern void (*fpPrint[])();
extern struct _set (*fpReach[])();
extern struct _tree *(*fpTraverse[])();
extern void (**fpTrans)();
extern void (**fpJTrans)();
extern void (*C_Trans[])();
extern void (*C_JTrans[])();
extern int BlkLevel;
extern int CurFile;
extern char *CurPredName;
extern char *CurRule;
extern int  CurRuleDebug;
extern Junction *CurRuleBlk;
extern RuleEntry *CurRuleNode;
extern ListNode *CurElementLabels;
extern ListNode *CurAstLabelsInActions;
extern ListNode *ContextGuardPredicateList;
extern ListNode *CurActionLabels;
extern int numericActionLabel;
extern ListNode *NumericPredLabels;
extern char *FileStr[];
extern int NumFiles;
extern int EpToken;
extern int WildCardToken;
extern Entry  **Tname,
        **Texpr,
        **Rname,
        **Fcache,
        **Tcache,
        **Elabel,
        **Sname,
        **Pname;
extern ListNode *ExprOrder;
extern ListNode **Cycles;
extern int TokenNum;
extern int LastTokenCounted;
extern ListNode *BeforeActions, *AfterActions, *LexActions;

/* MR1                                              */
/* MR1  11-Apr-97 Provide mechanism for inserting code into DLG class     */
/* MR1        via #lexmember <<....>> & #lexprefix <<...>>              */
/* MR1                      */

extern ListNode *LexMemberActions;
extern ListNode *LexPrefixActions;

extern set *fset;   /* for constrained search */
extern int maxk;    /* for constrained search */
extern int Save_argc;
extern char **Save_argv;
extern ListNode *eclasses, *tclasses;
extern char *HdrAction;
extern char *FirstAction;
extern FILE *ErrFile;
extern char *RemapFileName;
extern char *ErrFileName;
extern char *DlgFileName;
extern char *DefFileName;
extern char *ModeFileName;
extern char *StdMsgName;
extern int NumRules;
extern Junction **RulePtr;
extern int LL_k;
extern int CLL_k;
extern char *decodeJType[];
extern int PrintOut;
extern int PrintAnnotate;
extern int CodeGen;
extern int LexGen;
extern int esetnum;
extern int setnum;
extern int wordnum;
extern int GenAST;
extern int GenANSI;
extern int **FoStack;
extern int **FoTOS;
extern int GenExprSetsOpt;
extern FILE *DefFile;
extern int CannotContinue;
extern int GenCR;
extern int GenLineInfo;
extern int GenLineInfoMS;
extern int action_file, action_line;
extern int TraceGen;
extern int CurAmbigAlt1, CurAmbigAlt2, CurAmbigline, CurAmbigfile;
extern char *CurAmbigbtype;
extern int elevel;
extern int GenEClasseForRules;
extern FILE *input, *output;
extern char **TokenStr, **ExprStr;
extern int CurrentLexClass, NumLexClasses;
extern LClass lclass[];
extern char LexStartSymbol[];
extern char *CurRetDef;
extern char *CurParmDef;
extern int OutputLL_k;
extern int TreeResourceLimit;
extern int DemandLookahead;
extern char *RulePrefix;
extern int GenStdPccts;
extern char *stdpccts;
extern int ParseWithPredicates;
extern int ConstrainSearch;
extern int PURIFY;

extern set MR_CompromisedRules;
extern int MR_AmbSourceSearch;
extern int MR_SuppressSearch;
extern int MR_AmbSourceSearchGroup;
extern int MR_AmbSourceSearchChoice;
extern int MR_AmbSourceSearchLimit;
extern int MR_usingPredNames;
extern int MR_ErrorSetComputationActive;
extern char *MR_AmbAidRule;
extern int   MR_AmbAidLine;
extern int   MR_AmbAidMultiple;
extern int MR_AmbAidDepth;
extern int MR_skipped_e3_report;
extern int MR_matched_AmbAidRule;
extern int MR_Inhibit_Tokens_h_Gen;
extern int NewAST;
extern int tmakeInParser;
extern int AlphaBetaTrace;
extern int MR_BlkErr;
extern int MR_AlphaBetaWarning;
extern int MR_AlphaBetaMessageCount;
extern int MR_MaintainBackTrace;
extern int MR_BadExprSets;
extern int FoundGuessBlk;
extern int FoundException;
extern int FoundAtOperator;
extern int FoundExceptionGroup;
extern int WarningLevel;
extern int UseStdout;
extern int TabWidth;
extern int pLevel;
extern int pAlt1;
extern int pAlt2;
extern int AImode;
extern int HoistPredicateContext;
extern int MRhoisting;
extern int MRhoistingk;
extern int MR_debugGenRule;
extern int GenCC;
extern char *ParserName;
extern char *StandardSymbols[];
extern char *ASTSymbols[];
extern set reserved_positions;
extern set all_tokens;
extern set imag_tokens;
extern set tokclasses;
extern ListNode *ForcedTokens;
extern int *TokenInd;
extern FILE *Parser_h, *Parser_c;
extern char CurrentClassName[];
extern int no_classes_found;
extern char Parser_h_Name[];
extern char Parser_c_Name[];
extern char MRinfoFile_Name[];
extern FILE *MRinfoFile;
extern int MRinfo;
extern int MRinfoSeq;
extern int InfoP;
extern int InfoT;
extern int InfoF;
extern int InfoM;
extern int InfoO;
extern int PotentialSuppression;
extern int PotentialDummy;
extern int TnodesInUse;
extern int TnodesPeak;
extern int TnodesReportThreshold;
extern int TnodesAllocated;
extern char *ClassDeclStuff;
extern char *BaseClassName;
extern ListNode *class_before_actions, *class_after_actions;
extern char *UserTokenDefsFile;
extern int UserDefdTokens;
extern ListNode *MetaTokenNodes;
extern char *OutputDirectory;
extern int DontCopyTokens;
extern int LTinTokenAction;
extern set AST_nodes_refd_in_actions;
extern ListNode *CurExGroups;
extern int CurBlockID;
extern int CurAltNum;
extern Junction *CurAltStart;
extern Junction *OuterAltStart;
extern ExceptionGroup *DefaultExGroup;
extern int NumSignals;
extern int ContextGuardTRAV;
extern Junction *MR_RuleBlkWithHalt;
extern PointerStack MR_BackTraceStack;
extern PointerStack MR_PredRuleRefStack;
extern PointerStack MR_RuleBlkWithHaltStack;

extern int tokenActionActive;

extern char *PRED_OR_LIST;
extern char *PRED_AND_LIST;

#define STRICMP stricmp


extern Tree *tmake(Tree *root, ...);

extern int STRICMP(const char*, const char*);
extern void istackreset(void);
extern int istacksize(void);
extern void pushint(int);
extern int popint( void );
extern int istackempty( void );
extern int topint( void );
extern void NewSetWd( void );
extern void DumpSetWd( void );
extern void DumpSetWdForC( void );
extern void DumpSetWdForCC( void );
extern void NewSet( void );
extern void FillSet( set );
extern void ComputeErrorSets( void );
extern void ComputeTokSets( void );
extern void SubstErrorClass( set * );
extern int DefErrSet( set *, int, char * );
extern int DefErrSetForC( set *, int, char * );
extern int DefErrSetForCC( set *, int, char * );
extern int DefErrSet1(int, set *, int, char *);
extern int DefErrSetForC1(int, set *, int, char *, const char* );
extern int DefErrSetForCC1(int, set *, int, char *, const char* );
extern int DefErrSetWithSuffix(int, set *, int, char *, const char *);
extern void GenErrHdr( void );
extern void dumpExpr( FILE *, char * );
extern void addParm( Node *, char * );
extern Graph buildAction( char *, int, int, int );
extern Graph buildToken( char * );
extern Graph buildWildCard( char * );
extern Graph buildRuleRef( char * );
extern Graph Or( Graph, Graph );
extern Graph Cat( Graph, Graph );
extern Graph makeOpt( Graph, int, char *);
extern Graph makeBlk( Graph, int, char *);
extern Graph makeLoop( Graph, int, char *);
extern Graph makePlus( Graph, int, char *);
extern Graph emptyAlt( void );
extern Graph emptyAlt3( void );
extern TokNode * newTokNode( void );
extern RuleRefNode * newRNode( void );
extern Junction * newJunction( void );
extern ActionNode * newActionNode( void );
extern char * makelocks( void );
extern void preorder( Tree * );
extern Tree * tnode( int );
extern void _Tfree( Tree * );
extern Tree * tdup( Tree * );
extern int is_single_tuple( Tree * );
extern Tree * tappend( Tree *, Tree * );
extern void Tfree( Tree * );
extern Tree * tlink( Tree *, Tree *, int );
extern Tree * tshrink( Tree * );
extern Tree * tflatten( Tree * );
extern Tree * tJunc( Junction *, int, set * );
extern Tree * tRuleRef( RuleRefNode *, int, set * );
extern Tree * tToken( TokNode *, int, set * );
extern Tree * tAction( ActionNode *, int, set * );
extern int tmember( Tree *, Tree * );
extern int tmember_constrained( Tree *, Tree * );
extern Tree * tleft_factor( Tree * );
extern Tree * trm_perm( Tree *, Tree * );
extern void tcvt( set *, Tree * );
extern Tree * permute( int, int );
extern Tree * VerifyAmbig( Junction *, Junction *, unsigned **, set *, Tree **, Tree **, int * );
extern set rJunc( Junction *, int, set * );
extern set rRuleRef( RuleRefNode *, int, set * );
extern set rToken( TokNode *, int, set * );
extern set rAction( ActionNode *, int, set * );
extern void HandleAmbiguity( Junction *, Junction *, Junction *, int );
extern set First( Junction *, int, int, int * );
extern void freeBlkFsets( Junction * );
extern void genAction( ActionNode * );
extern void genRuleRef( RuleRefNode * );
extern void genToken( TokNode * );
extern void genOptBlk( Junction * );
extern void genLoopBlk( Junction *, Junction *, Junction *, int );
extern void genLoopBegin( Junction * );
extern void genPlusBlk( Junction * );
extern void genSubBlk( Junction * );
extern void genRule( Junction * );
extern void genJunction( Junction * );
extern void genEndBlk( Junction * );
extern void genEndRule( Junction * );
extern void genHdr( int );
extern void genHdr1( int );
extern void dumpAction( char *, FILE *, int, int, int, int );
extern void dumpActionPlus(ActionNode*, char *, FILE *, int, int, int, int );
extern Entry * hash_add( Entry **, char *, Entry * );
extern Entry * hash_get( Entry **, char * );
extern void hashStat( Entry ** );
extern void genLexDescr( void );
extern void dumpLexClasses( FILE * );
extern void genDefFile( void );
extern void DumpListOfParmNames( char *, FILE *, int );
extern int DumpNextNameInDef( char **, FILE * );
extern void DumpOldStyleParms( char *, FILE * );
extern void DumpType( char *, FILE * );
extern int strmember( char *, char * );
extern void DumpRetValStruct( FILE *, char *, int );
extern char * StripQuotes( char * );
extern void readDescr( void );
extern FILE * NextFile( void );
extern char * outnameX( char *, char *);
extern char * outname( char * );
extern void fatalFL( char *, char *, int );
extern void fatal_intern( char *, char *, int );
extern void cleanUp( void );
extern char * eMsg3( char *, char *, char *, char * );
extern char * eMsgd( char *, int );
extern char * eMsgd2( char *, int, int );
extern void s_fprT( FILE *, set );
extern char * TerminalString( int );
extern void lexclass( char * );
extern void lexmode( int );
extern int LexClassIndex( char * );
extern int hasAction( char * );
extern void setHasAction( char *, char * );
extern int addTname( char * );
extern int addTexpr( char * );
extern int Tnum( char * );
extern void Tklink( char *, char * );
extern Entry * newEntry( char *, int );
extern void list_add( ListNode **, void * );
extern void list_free( ListNode **, int freeData );
extern void list_apply( ListNode *, void (*)(void *) );
extern int list_search_cstring (ListNode *, char *);
extern char * Fkey( char *, int, int );
extern void FoPush( char *, int );
extern void FoPop( int );
extern void RegisterCycle( char *, int );
extern void ResolveFoCycles( int );
extern void pJunc( Junction * );
extern void pRuleRef( RuleRefNode * );
extern void pToken( TokNode * );
extern void pAction( ActionNode * );
extern void FoLink( Node * );
extern void addFoLink( Node *, char *, Junction * );
extern void GenCrossRef( Junction * );
extern void defErr( char *, long, long, long, long, long, long );
extern void genStdPCCTSIncludeFile(FILE *,char *);
extern char * pcctsBaseName(char *);
extern Predicate *find_predicates(Node *);
extern Predicate *MR_find_predicates_and_supp(Node *);
extern int predicateLookaheadDepth(ActionNode *);
extern void predicate_free(Predicate *);
extern Predicate * predicate_dup(Predicate *);
extern Predicate * predicate_dup_without_context(Predicate *);
extern void GenRulePrototypes(FILE *, Junction *);
extern Junction *first_item_is_guess_block(Junction *);
extern Junction *first_item_is_guess_block_extra(Junction * q);
extern Junction *analysis_point(Junction *);
extern Tree *make_tree_from_sets(set *, set *);
extern Tree *tdup_chain(Tree *);
extern Tree *tdif(Tree *, Predicate *, set *, set *);
extern set covered_set(Predicate *);
extern void AmbiguityDialog(Junction *, int, Junction *, Junction *, int *, int *);
extern void dumpAmbigMsg(set *, FILE *, int);
extern void GenRuleFuncRedefs(FILE *, Junction *);
extern void GenPredefinedSymbolRedefs(FILE *);
extern void GenASTSymbolRedefs(FILE *);
extern void GenRemapFile(void);
extern void GenSetRedefs(FILE *);
extern ForcedToken *newForcedToken(char *, int);
extern void RemapForcedTokens(void);
extern char *TokenOrExpr(int);
extern void setUpperRange(TokNode *, char *);
extern void GenParser_c_Hdr(void);
extern void GenParser_h_Hdr(void);
extern void GenRuleMemberDeclarationsForCC(FILE *, Junction *);
extern int addForcedTname( char *, int );
extern char *OutMetaName(char *);
extern void OutFirstSetSymbol(Junction *q, char *);
extern void warnNoFL(char *err);
extern void warnFL(char *err,char *f,int l);
extern void warn(char *err);
extern void warnNoCR( char *err );
extern void errNoFL(char *err);
extern void errFL(char *err,char *f,int l);
extern void err(char *err);
extern void errNoCR( char *err );
extern void genPredTree( Predicate *p, Node *j, int ,int);
extern UserAction *newUserAction(char *);
extern char *gate_symbol(char *name);
extern char *makeAltID(int blockid, int altnum);
extern void DumpRemainingTokSets(void);
extern void DumpANSIFunctionArgDef(FILE *f, Junction *q, int bInit);
extern void DumpFormals(FILE *, char *, int bInit);
extern char* hideDefaultArgs(const char* pdecl);
extern Predicate *computePredFromContextGuard(Graph,int *msgDone);
extern void recomputeContextGuard(Predicate *);
extern Predicate *new_pred(void);
extern void chkGTFlag(void);
extern void leAdd(LabelEntry *);
extern void leFixup(void);
extern void egAdd(ExceptionGroup *);
extern void egFixup(void);
extern void altAdd(Junction *);
extern void altFixup(void);
extern Predicate * MR_find_in_aSubBlk(Junction *alt);
extern Predicate * MR_predFlatten(Predicate *p);
extern Predicate * MR_predSimplifyALL(Predicate *p);
extern Predicate * MR_predSimplifyALLX(Predicate *p,int skipPass3);
extern int MR_allPredLeaves(Predicate *p);
extern void MR_cleanup_pred_trees(Predicate *p);
extern int MR_predicate_context_completed(Predicate *p);
extern void MR_check_pred_too_long(Predicate *p,set completion);
extern Tree * MR_remove_epsilon_from_tree(Tree *t);
extern Tree * MR_computeTreeAND(Tree *l,Tree *r);
extern int MR_tree_equ(Tree *big, Tree *small);
extern set MR_First(int ck,Junction *j,set *incomplete);
extern set MR_compute_pred_set(Predicate *p);
extern Tree * MR_compute_pred_tree_context(Predicate *p);
extern int MR_pointerStackPush(PointerStack *,void *);
extern void * MR_pointerStackPop(PointerStack *);
extern void * MR_pointerStackTop(PointerStack *);
extern void MR_pointerStackReset(PointerStack *);
extern void MR_backTraceReport(void);
extern void MR_alphaBetaTraceReport(void);
extern void MR_dumpRuleSet(set);
extern void MR_predContextPresent(Predicate *p,int *,int *);
extern void MR_dumpPred(Predicate *p,int withContext);
extern void MR_dumpPred1(int,Predicate *p,int withContext);
extern void MR_xxxIndent(FILE *f,int depth);
extern void MR_outputIndent(int depth);
extern void MR_stderrIndent(int depth);
extern Junction * MR_ruleReferenced(RuleRefNode *rrn);
extern Junction * MR_nameToRuleBlk(char *);
extern void MR_releaseResourcesUsedInRule(Node *);
extern void MR_dumpTreeX(int depth,Tree *t,int across);
extern void MR_dumpTreeF(FILE *f,int depth,Tree *t,int across);
extern void DumpFcache(void);
extern void MR_dumpTokenSet(FILE *f,int depth,set s);
extern void MR_traceAmbSource(set *,Junction *,Junction *);
extern void MR_traceAmbSourceK(Tree *,Junction *a1,Junction *a2);
extern void MR_traceAmbSourceKclient(void);
extern Node *MR_advance(Node *);
extern int  MR_offsetFromRule(Node *);
extern char *MR_ruleNamePlusOffset(Node *);
extern int  MR_max_height_of_tree(Tree *);
extern int  MR_all_leaves_same_height(Tree *,int);
extern void MR_projectTreeOntoSet(Tree *t,int k,set *);
extern Tree *MR_make_tree_from_set(set);
extern Predicate *MR_removeRedundantPredPass3(Predicate *);
extern void MR_pred_depth(Predicate *,int *);
extern int  MR_comparePredicates(Predicate *,Predicate *);
extern Predicate * MR_unfold(Predicate *);
extern void MR_simplifyInverted(Predicate *,int);
extern int  MR_secondPredicateUnreachable
            (Predicate *first,Predicate *second);
extern void MR_clearPredEntry(Predicate *);
extern void MR_orphanRules(FILE *);
extern void MR_merge_contexts(Tree *);
extern int  ci_strequ(char *,char *);
extern void MR_guardPred_plainSet(ActionNode *anode,Predicate *);
extern void MR_suppressSearchReport(void);
extern Predicate * MR_suppressK(Node *,Predicate *);
extern void MR_backTraceDumpItem(FILE *,int skip,Node *n);
extern void MR_backTraceDumpItemReset(void);
extern Junction * MR_junctionWithoutP2(Junction *);
extern void MR_setConstrainPointer(set *);
extern void BlockPreambleOption(Junction *q, char * pSymbol);
extern char* getInitializer(char *);
extern char *endFormal(char *pStart,
             char **ppDataType,
             char **ppSymbol,
             char **ppEqualSign,
             char **ppValue,
             char **ppSeparator,
             int *pNext);
extern char *strBetween(char *pStart,
            char *pNext,
            char *pStop);
extern int hasMultipleOperands(char *);
extern void DumpInitializers(FILE*, RuleEntry*, char*);
extern int isTermEntryTokClass(TermEntry *);
extern int isEmptyAlt(Node *, Node *);

/* MR20 G. Hobbelt  Create proper externs for dlg variables */

extern set attribsRefdFromAction;
extern int inAlt;
extern int UsedOldStyleAttrib;
extern int UsedNewStyleLabel;

#define MAX_BLK_LEVEL 100
extern int     CurBlockID_array[MAX_BLK_LEVEL];
extern int     CurAltNum_array[MAX_BLK_LEVEL];
