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
 * 1989-2000
 */

/**
 * \file antlr.h
 *
 * Define all of the stack setup and manipulation of $i, #i variables.
 *
 *  Notes:
 *    The type 'Attrib' must be defined before entry into this .h file.
 */


#ifndef ANTLR_H
#define ANTLR_H

#include <string.h>
#include "dlg_p.h" // for Attrib
#include "dlg_a.h"

typedef int ANTLRTokenType;
typedef unsigned char SetWordType;

typedef char ANTLRChar;

            /* G u e s s  S t u f f */

#ifdef ZZCAN_GUESS
#ifndef ZZINF_LOOK
#define ZZINF_LOOK
#endif
#endif

#ifdef ZZCAN_GUESS
typedef struct _zzjmp_buf {
    jmp_buf state;
} zzjmp_buf;
#endif


/* can make this a power of 2 for more efficient lookup */

#ifndef ZZLEXBUFSIZE
#define ZZLEXBUFSIZE  8000 /* MR22 raise from 2k to 8k */
#endif

#define zzOvfChk                            \
    if ( zzasp <= 0 )                                           \
    {                                                           \
        fprintf(stderr, zzStackOvfMsg, __FILE__, __LINE__);   \
        exit(1);                                               \
    }

#ifndef ZZA_STACKSIZE
#define ZZA_STACKSIZE 400
#endif
#ifndef ZZAST_STACKSIZE
#define ZZAST_STACKSIZE 400
#endif

#ifndef zzfailed_pred
#ifdef ZZCAN_GUESS
#define zzfailed_pred(_p,_hasuseraction,_useraction) \
  if (zzguessing) { \
    zzGUESS_FAIL; \
  } else { \
    zzfailed_pred_action(_p,_hasuseraction,_useraction); \
  }
#else
#define zzfailed_pred(_p,_hasuseraction,_useraction) \
    zzfailed_pred_action(_p,_hasuseraction,_useraction);
#endif
#endif

/*  MR23            Provide more control over failed predicate action
                    without any need for user to worry about guessing internals.
                    _hasuseraction == 0 => no user specified error action
                    _hasuseraction == 1 => user specified error action
*/

#ifndef zzfailed_pred_action
#define zzfailed_pred_action(_p,_hasuseraction,_useraction) \
    if (_hasuseraction) { _useraction } \
    else { fprintf(stderr, "semantic error; failed predicate: '%s'\n",_p); }
#endif

#ifndef zzcr_ast
#define zzcr_ast(ast,attr,tok,text)
#endif

#ifdef DEMAND_LOOK
#define DemandLookData  int zzdirty=1;
#else
#define DemandLookData
#endif

#ifndef zzUSER_GUESS_HOOK
#define zzUSER_GUESS_HOOK(seqFrozen,zzrv)
#endif

#ifndef zzUSER_GUESS_DONE_HOOK
#define zzUSER_GUESS_DONE_HOOK(seqFrozen)
#endif

            /* S t a t e  S t u f f */

#ifdef ZZCAN_GUESS
#define zzGUESS_BLOCK   zzantlr_state zzst; int zzrv; int zzGuessSeqFrozen;

/* MR10 change zzGUESS: do zzGUESS_DONE when zzrv==1 after longjmp as in C++ mode */

#define zzGUESS       zzsave_antlr_state(&zzst); \
              zzguessing = 1; \
                            zzGuessSeqFrozen=++zzGuessSeq; \
              zzrv = setjmp(zzguess_start.state); \
                            zzUSER_GUESS_HOOK(zzGuessSeqFrozen,zzrv) \
                            if (zzrv) zzGUESS_DONE;
#ifdef zzTRACE_RULES
#define zzGUESS_FAIL    { zzTraceGuessFail(); longjmp(zzguess_start.state, 1); }
#else
#define zzGUESS_FAIL    longjmp(zzguess_start.state, 1)
#endif

/* MR10 change zzGUESS_DONE: zzrv=1 to simulate longjmp() return value as in C++ mode */

#define zzGUESS_DONE    { zzrestore_antlr_state(&zzst); zzrv=1; zzUSER_GUESS_DONE_HOOK(zzGuessSeqFrozen) }
#define zzNON_GUESS_MODE  if ( !zzguessing )
#define zzGuessData                                     \
            zzjmp_buf zzguess_start;                    \
            int zzguessing;
#else
#define zzGUESS_BLOCK
#define zzGUESS
#define zzGUESS_FAIL
#define zzGUESS_DONE
#define zzNON_GUESS_MODE
#define zzGuessData
#endif

typedef struct _zzantlr_state {
#ifdef ZZCAN_GUESS
      zzjmp_buf guess_start;
      int guessing;
#endif
      int asp;
      int ast_sp;
#ifdef ZZINF_LOOK
      int inf_lap;  /* not sure we need to save this one */
      int inf_labase;
      int inf_last;

/* MR6  Gunnar Rxnning (gunnar@candleweb.no)                                */
/* MR6    Additional state needs to be saved/restored                       */
/* MR6    Matching changes in err.h                                         */

      int *inf_tokens;                                       /* MR6 */
      char **inf_text;                                       /* MR6 */
      char *inf_text_buffer;                                 /* MR6 */
      int *inf_line;                                         /* MR6 */
#endif
#ifdef DEMAND_LOOK
      int dirty;
#endif

#ifdef LL_K
      int tokenLA[LL_K];
      char textLA[LL_K][ZZLEXBUFSIZE];
      int lap;
      int labase;
#else
      int token;
      char text[ZZLEXBUFSIZE];
#endif
#ifdef zzTRACE_RULES
      int     traceOptionValue;       /* MR10 */
      int     traceGuessOptionValue;  /* MR10 */
      char    *traceCurrentRuleName;  /* MR10 */
      int     traceDepth;             /* MR10 */
#endif

} zzantlr_state;

#ifdef zzTRACE_RULES
extern int  zzTraceOptionValueDefault;
extern int  zzTraceOptionValue;
extern int  zzTraceGuessOptionValue;
extern char *zzTraceCurrentRuleName;
extern int  zzTraceDepth;
#endif

extern int zzGuessSeq;                      /* MR10 */
extern int zzSyntaxErrCount;                /* MR11 */
extern int zzLexErrCount;                   /* MR11 */

                 /* I n f i n i t e  L o o k a h e a d */


#ifdef ZZINF_LOOK
#define InfLookData \
  int *zzinf_tokens;  \
  char **zzinf_text;  \
  char *zzinf_text_buffer;  \
  int *zzinf_line;        \
  int zzinf_labase; \
  int zzinf_last;
#else
#define InfLookData
#endif

#ifdef ZZINF_LOOK

#ifndef ZZINF_DEF_TEXT_BUFFER_SIZE
#define ZZINF_DEF_TEXT_BUFFER_SIZE      20000
#endif
#ifndef ZZINF_DEF_TOKEN_BUFFER_SIZE
#define ZZINF_DEF_TOKEN_BUFFER_SIZE     2000
#endif
/* WARNING!!!!!!
 * ZZINF_BUFFER_TEXT_CHUNK_SIZE must be > sizeof(text) largest possible token.
 */
#ifndef ZZINF_BUFFER_TEXT_CHUNK_SIZE
#define ZZINF_BUFFER_TEXT_CHUNK_SIZE  5000
#endif
#ifndef ZZINF_BUFFER_TOKEN_CHUNK_SIZE
#define ZZINF_BUFFER_TOKEN_CHUNK_SIZE 1000
#endif

#if ZZLEXBUFSIZE > ZZINF_BUFFER_TEXT_CHUNK_SIZE
#define ZZINF_BUFFER_TEXT_CHUNK_SIZE  ZZLEXBUFSIZE+5
#endif

/* make inf_look user-access macros */
#ifdef LL_K
#define ZZINF_LA_VALID(i) (((zzinf_labase+i-1)-LL_K+1) <= zzinf_last)
#define ZZINF_LA(i)     zzinf_tokens[(zzinf_labase+i-1)-LL_K+1]
#define ZZINF_LATEXT(i)   zzinf_text[(zzinf_labase+i-1)-LL_K+1]
/* MR6  In 1.33 vanilla the #define ZZINF_LINE(i) is was commented out  */
#define ZZINF_LINE(i)       zzinf_line[(zzinf_labase+i-1)-LL_K+1]
#else
#define ZZINF_LA_VALID(i) (((zzinf_labase+i-1)) <= zzinf_last)
#define ZZINF_LA(i)     zzinf_tokens[(zzinf_labase+i-1)]
#define ZZINF_LATEXT(i)   zzinf_text[(zzinf_labase+i-1)]
#endif

#define inf_zzgettok _inf_zzgettok()
extern void _inf_zzgettok();

#endif  /* ZZINF_LOOK */


#define ANTLR_INFO  \
  Attrib zzempty_attr(void) {static Attrib a; return a;} \
  Attrib zzconstr_attr(int _tok, char *_text) \
    {Attrib a; zzcr_attr((&a),_tok,_text); return a;} \
  int zzasp=ZZA_STACKSIZE; \
  char zzStackOvfMsg[]="fatal: attrib/AST stack overflow %s(%d)!\n"; \
  Attrib zzaStack[ZZA_STACKSIZE]; DemandLookData \
  InfLookData \
    zzGuessData


#ifdef ZZINF_LOOK

#ifdef LL_K
#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  {zzdirty=LL_K; zzlap = zzlabase = 0;}
#else
#define zzPrimeLookAhead  {zzlap = zzlabase = 0; zzfill_inf_look();\
              {int _i;  for(_i=1;_i<=LL_K; _i++)    \
                    {zzCONSUME;} zzlap = zzlabase = 0;}}
#endif

#else /* LL_K */

#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  zzfill_inf_look(); zzdirty=1
#else
#define zzPrimeLookAhead  zzfill_inf_look(); inf_zzgettok

#endif
#endif  /* LL_K */

#else /* ZZINF_LOOK */

#ifdef LL_K
#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  {zzdirty=LL_K; zzlap = zzlabase = 0;}
#else
#define zzPrimeLookAhead  {int _i; zzlap = 0; for(_i=1;_i<=LL_K; _i++)    \
                    {zzCONSUME;} zzlap = 0;}
#endif

#else

#ifdef DEMAND_LOOK
#define zzPrimeLookAhead  zzdirty=1
#else
#define zzPrimeLookAhead  zzgettok()
#endif
#endif  /* LL_K */

#endif  /* ZZINF_LOOK */


#ifdef LL_K
#define zzenterANTLRs(s)                            \
        zzlextext = &(zztextLA[0][0]); zzrdstr( s ); zzPrimeLookAhead;
#define zzenterANTLRf(f)              \
    zzlextext = &(zztextLA[0][0]); zzrdfunc( f ); zzPrimeLookAhead;
#define zzenterANTLR(f)             \
    zzlextext = &(zztextLA[0][0]); zzrdstream( f ); zzPrimeLookAhead;
#ifdef ZZINF_LOOK
#define zzleaveANTLR(f)     free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRf(f)      free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRs(f)    free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#else
#define zzleaveANTLR(f)
#define zzleaveANTLRf(f)
#define zzleaveANTLRs(f)
#endif

#else

#define zzenterANTLRs(s)                            \
        {static char zztoktext[ZZLEXBUFSIZE];   \
        zzlextext = zztoktext; zzrdstr( s ); zzPrimeLookAhead;}
#define zzenterANTLRf(f)              \
    {static char zztoktext[ZZLEXBUFSIZE]; \
    zzlextext = zztoktext; zzrdfunc( f ); zzPrimeLookAhead;}
#define zzenterANTLR(f)             \
    {static char zztoktext[ZZLEXBUFSIZE]; \
    zzlextext = zztoktext; zzrdstream( f ); zzPrimeLookAhead;}
#ifdef ZZINF_LOOK
#define zzleaveANTLR(f)     free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRf(f)      free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#define zzleaveANTLRs(f)    free(zzinf_text_buffer); free(zzinf_text); free(zzinf_tokens); free(zzinf_line);
#else
#define zzleaveANTLR(f)
#define zzleaveANTLRf(f)
#define zzleaveANTLRs(f)
#endif

#endif

/* MR19 Paul D. Smith (psmith@baynetworks.com)
   Need to adjust AST stack pointer at exit.
   Referenced in ANTLRx macros.
*/

#ifdef GENAST
#define ZZAST_ADJUST ++zzast_sp;
#else
#define ZZAST_ADJUST
#endif

#define ANTLR(st, f)  zzbufsize = ZZLEXBUFSIZE; \
            zzenterANTLR(f);      \
            {                                            \
              zzBLOCK(zztasp1);                          \
              st; /* ++zzasp; Removed MR20 G. Hobbelt */     \
                  /* ZZAST_ADJUST Removed MR20 G. Hobbelt */ \
              /* MR20 G. Hobbelt. Kill the top' attribute (+AST stack corr.) */  \
              zzEXIT_ANTLR(zztasp1 + 1);                 \
            }                                            \
            zzleaveANTLR(f);

#define ANTLRm(st, f, _m) zzbufsize = ZZLEXBUFSIZE; \
            zzmode(_m);       \
            zzenterANTLR(f);      \
            {                                            \
              zzBLOCK(zztasp1);                          \
              st; /* ++zzasp; Removed MR20 G. Hobbelt */     \
                  /* ZZAST_ADJUST Removed MR20 G. Hobbelt */ \
              /* MR20 G. Hobbelt. Kill the top' attribute (+AST stack corr.) */  \
              zzEXIT_ANTLR(zztasp1 + 1);                 \
            }                                            \
            zzleaveANTLR(f);

#define ANTLRf(st, f) zzbufsize = ZZLEXBUFSIZE; \
            zzenterANTLRf(f);     \
            {                                            \
              zzBLOCK(zztasp1);                          \
              st; /* ++zzasp; Removed MR20 G. Hobbelt */     \
                  /* ZZAST_ADJUST Removed MR20 G. Hobbelt */ \
              /* MR20 G. Hobbelt. Kill the top' attribute (+AST stack corr.) */  \
              zzEXIT_ANTLR(zztasp1 + 1);                 \
            }                                            \
            zzleaveANTLRf(f);

#define ANTLRs(st, s)   zzbufsize = ZZLEXBUFSIZE; \
                        zzenterANTLRs(s);           \
            {                                            \
              zzBLOCK(zztasp1);                          \
              st; /* ++zzasp; Removed MR20 G. Hobbelt */     \
                  /* ZZAST_ADJUST Removed MR20 G. Hobbelt */ \
              /* MR20 G. Hobbelt. Kill the top' attribute (+AST stack corr.) */  \
              zzEXIT_ANTLR(zztasp1 + 1);                 \
            }                                            \
                        zzleaveANTLRs(s);

#ifdef LL_K
#define zztext    (&(zztextLA[zzlap][0]))
#else
#define zztext    zzlextext
#endif


          /* A r g u m e n t  A c c e s s */

#define zzaCur      (zzaStack[zzasp])
#define zzaRet      (*zzaRetPtr)
#define zzaArg(v,n)   zzaStack[v-n]
#define zzMakeAttr    { zzNON_GUESS_MODE {zzOvfChk; --zzasp; zzcr_attr(&(zzaStack[zzasp]),LA(1),LATEXT(1));}}
#ifdef zzdef0
#define zzMake0     { zzOvfChk; --zzasp; zzdef0(&(zzaStack[zzasp]));}
#else
#define zzMake0     { zzOvfChk; --zzasp;}
#endif
#define zzaPush(_v)   { zzOvfChk; zzaStack[--zzasp] = _v;}
#ifndef zzd_attr
#define zzREL(t)    zzasp=(t);    /* Restore state of stack */
#else
#define zzREL(t)    for (; zzasp<(t); zzasp++)        \
            { zzd_attr(&(zzaStack[zzasp])); }
#endif


#define zzsetmatch(_es,_tokclassErrset)           \
  if ( !_zzsetmatch(_es, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet, _tokclassErrset) ) goto fail; /* MR23 */

#ifdef ZZCAN_GUESS
#define zzsetmatch_wsig(_es, handler)   \
  if ( !_zzsetmatch_wsig(_es) ) if (zzguessing) { zzGUESS_FAIL; } else {_signal=MismatchedToken; goto handler;}
#else
#define zzsetmatch_wsig(_es, handler)   \
  if ( !_zzsetmatch_wsig(_es) ) {_signal=MismatchedToken; goto handler;}
#endif

extern int _zzsetmatch(SetWordType *, char **, char **, int *, int *, SetWordType **, SetWordType *);
extern int _zzsetmatch_wsig(SetWordType *);

#define zzmatch(_t)             \
  if ( !_zzmatch(_t, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet) ) goto fail;

#ifdef ZZCAN_GUESS
#define zzmatch_wsig(_t,handler)      \
  if ( !_zzmatch_wsig(_t) ) if (zzguessing) { zzGUESS_FAIL; } else {_signal=MismatchedToken; goto handler;}
#else
#define zzmatch_wsig(_t,handler)      \
  if ( !_zzmatch_wsig(_t) ) {_signal=MismatchedToken; goto handler;}
#endif

extern int _zzmatch(int, char **, char **, int *, int *, SetWordType **);
extern int _zzmatch_wsig(int);

#define zzmatch_wdfltsig(_t,_f)     \
  if ( !_zzmatch_wdfltsig(_t,_f) ) _signal=MismatchedToken;
#define zzsetmatch_wdfltsig(tw,tt,wf)   \
  if ( !_zzsetmatch_wdfltsig(tw,tt,wf) ) _signal=MismatchedToken;

extern int _zzmatch_wdfltsig(int, SetWordType *);
extern int _zzsetmatch_wdfltsig(SetWordType *tokensWanted,
                int tokenTypeOfSet,
                SetWordType *whatFollows);

#ifdef GENAST
#define zzRULE    Attrib *zzaRetPtr = &(zzaStack[zzasp-1]); \
          SetWordType *zzMissSet=NULL; int zzMissTok=0;   \
          int zzBadTok=0; char *zzBadText="";   \
          int zzErrk=1,zzpf=0;          \
                    zzTRACEdata \
          char *zzMissText=""; zzASTVars
#else
#define zzRULE    Attrib *zzaRetPtr = &(zzaStack[zzasp-1]); \
          int zzBadTok=0; char *zzBadText="";   \
          int zzErrk=1,zzpf=0;                \
                    zzTRACEdata \
          SetWordType *zzMissSet=NULL; int zzMissTok=0; char *zzMissText=""
#endif

#ifdef GENAST
#define zzBLOCK(i)  int i = zzasp - 1; int zztsp = zzast_sp
#define zzEXIT(i) zzREL(i); zzastREL; zzNON_GUESS_MODE { zzastPush(*_root); }
#define zzEXIT_ANTLR(i) zzREL(i); zzastREL /* [i_a] added as we want this for the ANTLRx() macros */
#define zzLOOP(i) zzREL(i); zzastREL
#else
#define zzBLOCK(i)  int i = zzasp - 1
#define zzEXIT(i) zzREL(i)
#define zzEXIT_ANTLR(i) zzREL(i)           /* [i_a] added as we want this for the ANTLRx() macros */
#define zzLOOP(i) zzREL(i)
#endif

#ifdef LL_K

#ifdef DEMAND_LOOK
#define LOOK(_k)  {int i,stop=_k-(LL_K-zzdirty); for (i=1; i<=stop; i++)  \
          zzCONSUME;}
#define zzCONSUME {zzgettok(); zzdirty--;             \
          zzlap = (zzlap+1)&(LL_K-1);           \
          zzlextext = &(zztextLA[zzlap][0]);}
#else
#ifdef ZZINF_LOOK
#define zzCONSUME {inf_zzgettok;                  \
          zzlap = (zzlap+1)&(LL_K-1);           \
          zzlextext = &(zztextLA[zzlap][0]);        \
          }
#else
#define zzCONSUME {zzgettok();                  \
          zzlap = (zzlap+1)&(LL_K-1);           \
          zzlextext = &(zztextLA[zzlap][0]);}
#endif /* ZZINF_LOOK */
#endif /* DEMAND_LOOK */

#else /* LL_K */

#ifdef DEMAND_LOOK
#define LOOK(_k)  if ( zzdirty) zzCONSUME;
#ifdef ZZINF_LOOK
#define zzCONSUME inf_zzgettok; zzdirty=0;
#else
#define zzCONSUME zzgettok(); zzdirty=0;
#endif /* ZZINF_LOOK */

#else  /* DEMAND_LOOK */

#ifdef ZZINF_LOOK
#define zzCONSUME inf_zzgettok
#else
#define zzCONSUME zzgettok();
#endif

#endif /* DEMAND_LOOK */

#endif /* LL_K */

#ifdef LL_K
#define NLA     zztokenLA[zzlap&(LL_K-1)] /* --> next LA */
#define NLATEXT   zztextLA[zzlap&(LL_K-1)]  /* --> next text of LA */
#ifdef DEMAND_LOOK
#define LA(i)       zztokenLA[(zzlabase+(i)-1)&(LL_K-1)]
#define LATEXT(i)   (&(zztextLA[(zzlabase+(i)-1)&(LL_K-1)][0]))
#else
#define LA(i)       zztokenLA[(zzlap+(i)-1)&(LL_K-1)]
#define LATEXT(i)   (&(zztextLA[(zzlap+(i)-1)&(LL_K-1)][0]))
#endif
#else
#define NLA     zztoken
#define NLATEXT   zztext
#define LA(i)       zztoken
#define LATEXT(i)   zztext
#endif


           /* S t a n d a r d  S i g n a l s */

#define NoSignal      0
#define MismatchedToken   1
#define NoViableAlt     2
#define NoSemViableAlt    3

/* MR7  Allow more control over signalling                                  */
/*        by adding "Unwind" and "zzsetSignal"                              */

#define Unwind              4
#define zzsetSignal(newValue) *_retsignal=_signal=(newValue)
#define zzsuppressSignal *_retsignal=_signal=0
#define zzexportSignal    *_retsignal=_signal

           /* F u n c t i o n  T r a c i n g */

#ifndef zzTRACE_RULES
#define zzTRACEdata
#else
#ifndef zzTRACEdata
#define zzTRACEdata     ANTLRChar *zzTracePrevRuleName = NULL;
#endif
#endif

#ifndef zzTRACEIN
#define zzTRACEIN(r)  zzTracePrevRuleName=zzTraceCurrentRuleName;zzTraceIn(r);
#endif
#ifndef zzTRACEOUT
#define zzTRACEOUT(r) zzTraceOut(r);zzTraceCurrentRuleName=zzTracePrevRuleName;
#endif

/* MR19 zzchar_t additions */

#ifndef zzchar_t
#ifdef ZZWCHAR_T
#define zzchar_t wchar_t
#else
#define zzchar_t char
#endif
#endif


        /* E x t e r n  D e f s */

extern Attrib zzempty_attr(void);
extern Attrib zzconstr_attr(int, char *);
extern void zzsyn(char *, int, char *, SetWordType *, int, int, char *);
extern int zzset_el(unsigned, SetWordType *);
extern int zzset_deg(SetWordType *);
extern void zzedecode(SetWordType *);
extern void zzresynch(SetWordType *, SetWordType);
extern void zzsave_antlr_state(zzantlr_state *);
extern void zzrestore_antlr_state(zzantlr_state *);
extern void zzfill_inf_look(void);
extern void zzconsumeUntil(SetWordType *st);                         /* MR7 */
extern void zzconsumeUntilToken(int t);                              /* MR7 */
extern void zzTraceIn(char * ruleName);                              /* MR10 */
extern void zzTraceOut(char * ruleName);                             /* MR10 */
extern int  zzTraceOption(int delta);                                /* MR10 */
extern int  zzTraceGuessOption(int delta);                           /* MR10 */
extern void zzTraceReset(void);                                      /* MR10 */
extern void zzTraceGuessFail(void);                                  /* MR10 */
#ifdef EXCEPTION_HANDLING
extern void zzdflthandlers(int, int *);
#endif

        /* G l o b a l  V a r i a b l e s */

/* Define a parser; user should do a "#parser myname" in their grammar file */
/*extern struct pccts_parser zzparser;*/

#ifdef LL_K
int zztokenLA[LL_K];
zzchar_t zztextLA[LL_K][ZZLEXBUFSIZE];
int zzlap = 0, zzlabase=0; // labase only used for DEMAND_LOOK
#else
int zztoken;
#endif

extern char zzStackOvfMsg[];
extern int zzasp;
extern Attrib zzaStack[];
#ifdef ZZINF_LOOK
extern int *zzinf_tokens;
extern char **zzinf_text;
extern char *zzinf_text_buffer;
extern int *zzinf_line;
extern int zzinf_labase;
extern int zzinf_last;
#endif
#ifdef DEMAND_LOOK
extern int zzdirty;
#endif
#ifdef ZZCAN_GUESS
extern int zzguessing;
extern zzjmp_buf zzguess_start;
#endif

#endif
