/*
 * sor.h
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

#ifndef __SOR_H__
#define __SOR_H__

#include "constants.h"
#include "set.h"
#include "logger.h"

#define MaxNumFiles     20
#define MaxRuleName     100     /* largest rule name size */
#define MaxAtom     300

#define warn(err) warning(err, FileStr[CurFile], zzline);
#define errNoFL(err) {found_error=1; errorNoFL(err);}
#define errFL(err,f,l) {found_error=1; errorFL(err, f, l);}
#define err(err) {found_error=1; errorFL(err, FileStr[CurFile], zzline);}

          /* L i s t  N o d e s */

typedef struct _ListNode {
    void *elem;     /* pointer to any kind of element */
    struct _ListNode *next;
} ListNode;

typedef struct {
    char *token;
    int tnum;
} TokenDef;

typedef struct {
    char decl[MaxAtom+1];
    char var[MaxAtom+1];
    char init[MaxAtom+1];
    int global;
} RefVarRec;

#define newListNode (ListNode *) calloc(1, sizeof(ListNode));
#define newTokenDef (TokenDef *) calloc(1, sizeof(TokenDef));

         /* Grammar Lookahead Automata (GLA) S t u f f */

typedef struct _nfa {
    struct _nfa *p1, *p2;
    int label1, label2;
    struct _nfa *next;  /* used only following rule ref and blkstart nodes
                 * of (...)+ to detect what follows the loop. */
    set lookahead;
    char visited;
    char is_rule_ref;
    char is_rule_entry;
    char *in_rule;    /* which rule am I in */
    int upper_range;  /* used for range operator; this will be non-zero */
} GLA;


        /* S a n i t y  C h e c k i n g */

#ifndef require
#define require(expr, err) {if ( !(expr) ) fatal(err);}
#endif

#endif
