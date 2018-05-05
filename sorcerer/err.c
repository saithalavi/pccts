/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: sor.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION 13333
#include "pcctscfg.h"
#include <stdio.h>

/*  23-Sep-97   thm     Accomodate user who needs to redefine ZZLEXBUFSIZE  */

#ifndef ZZLEXBUFSIZE
#define ZZLEXBUFSIZE  8000
#endif
#include "constants.h"
#include "charbuf.h"
#include "hash.h"
#include "set.h"
#include "sor.h"
#define AST_FIELDS  \
int token; char text[MaxAtom+1], label[MaxRuleName+1]; \
char *action;   /* if action node, here is ptr to it */ \
char in,out; \
char init_action; /* set if Action and 1st action of alt */ \
int file; int line; /* set for BLOCK, ALT, nonterm nodes */ \
int upper_range;  /* only if T1..T2 found */  \
GLA *start_state; /* ptr into GLA for this block */ \
int no_copy;    /* copy input ptr to output ptr? */ \
ListNode *refvars;  /* any ref vars defined for this rule */ \
unsigned char is_root; /* this token is a root #( A ... ) */
#define zzcr_ast(node, cur, _tok, _text)  \
{(node)->token=_tok; strncpy((node)->text, _text,MaxAtom);}
#define USER_ZZSYN
#define zzAST_DOUBLE
extern int define_num;
#define LL_K 2
#define zzSET_SIZE 16
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[118]={
    /* 00 */    "Invalid",
    /* 01 */    "Eof",
    /* 02 */    "RExpr",
    /* 03 */    "\\n|\\r|\\r\\n",
    /* 04 */    "\\~[]",
    /* 05 */    "~[\\n\\r\"\\]+",
    /* 06 */    "\"",
    /* 07 */    "\\n|\\r|\\r\\n",
    /* 08 */    "\\~[]",
    /* 09 */    "~[\\n\\r\"\\]+",
    /* 10 */    "'",
    /* 11 */    "\\n|\\r|\\r\\n",
    /* 12 */    "\\~[]",
    /* 13 */    "~[\\n\\r'\\]+",
    /* 14 */    "\\*/",
    /* 15 */    "\\*",
    /* 16 */    "\\n|\\r|\\r\\n",
    /* 17 */    "~[\\n\\r\\*]+",
    /* 18 */    "\\n|\\r|\\r\\n",
    /* 19 */    "~[\\n\\r]+",
    /* 20 */    "\\n|\\r|\\r\\n",
    /* 21 */    "~[\\n\\r]+",
    /* 22 */    "\\*/",
    /* 23 */    "\\*",
    /* 24 */    "\\n|\\r|\\r\\n",
    /* 25 */    "~[\\n\\r\\*]+",
    /* 26 */    "~[\\)]+ \\)",
    /* 27 */    "Action",
    /* 28 */    "PassAction",
    /* 29 */    "\\n|\\r|\\r\\n",
    /* 30 */    "\\>",
    /* 31 */    "#[_a-zA-Z][_a-zA-Z0-9]*",
    /* 32 */    "#\\[\\]",
    /* 33 */    "#\\(\\)",
    /* 34 */    "#\\[",
    /* 35 */    "#\\(",
    /* 36 */    "#",
    /* 37 */    "\\)",
    /* 38 */    "\\[",
    /* 39 */    "\\(",
    /* 40 */    "\\\\]",
    /* 41 */    "\\\\)",
    /* 42 */    "\\>",
    /* 43 */    "'",
    /* 44 */    "\"",
    /* 45 */    "\\#",
    /* 46 */    "\\~[\\]\\)>#]",
    /* 47 */    "/",
    /* 48 */    "/\\*",
    /* 49 */    "\\*/",
    /* 50 */    "//",
    /* 51 */    "\\@\\(",
    /* 52 */    "\\@",
    /* 53 */    "[a-zA-Z_]+\\(",
    /* 54 */    "[a-zA-Z_]+",
    /* 55 */    "~[a-zA-Z_\\n\\r\\)\\(\\#\\>\\]\\[\"'/\\@]+",
    /* 56 */    "[\\t\\ ]+",
    /* 57 */    "\\n|\\r|\\n\\r",
    /* 58 */    "\\[",
    /* 59 */    "\\<\\<",
    /* 60 */    "\"",
    /* 61 */    "/\\*",
    /* 62 */    "\\*/",
    /* 63 */    "//",
    /* 64 */    "\\>\\>",
    /* 65 */    "Header",
    /* 66 */    "Tokdef",
    /* 67 */    "BLOCK",
    /* 68 */    "ALT",
    /* 69 */    "LABEL",
    /* 70 */    "OPT",
    /* 71 */    "POS_CLOSURE",
    /* 72 */    "CLOSURE",
    /* 73 */    "WILD",
    /* 74 */    "PRED_OP",
    /* 75 */    "BT",
    /* 76 */    "RULE",
    /* 77 */    "REFVAR",
    /* 78 */    "atomic-element",
    /* 79 */    "rule-header",
    /* 80 */    "\\}",
    /* 81 */    "class",
    /* 82 */    "NonTerm",
    /* 83 */    "Token",
    /* 84 */    "!",
    /* 85 */    "\\<",
    /* 86 */    "\\>",
    /* 87 */    ";",
    /* 88 */    "\\|",
    /* 89 */    "\\(",
    /* 90 */    "\\)",
    /* 91 */    "..",
    /* 92 */    "#[A-Za-z0-9_]*",
    /* 93 */    "\\*/",
    /* 94 */    "\\*",
    /* 95 */    "\\n|\\r|\\r\\n",
    /* 96 */    "~[\\n\\r\\*]+",
    /* 97 */    "\\n|\\r|\\r\\n",
    /* 98 */    "~[\\n\\r]+",
    /* 99 */    "[\\t\\ ]+",
    /* 100 */   "\\n|\\r|\\r\\n",
    /* 101 */   "//",
    /* 102 */   "/\\*",
    /* 103 */   "#ifndef",
    /* 104 */   "#ifdef",
    /* 105 */   "#else",
    /* 106 */   "#define",
    /* 107 */   "#endif",
    /* 108 */   "ID",
    /* 109 */   "INT",
    /* 110 */   "enum",
    /* 111 */   "\\{",
    /* 112 */   "=",
    /* 113 */   ",",
    /* 114 */   "DLGminToken",
    /* 115 */   "DLGmaxToken",
    /* 116 */   "\\}",
    /* 117 */   ";"
};
SetWordType zzerr1[16] = {0x2,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x7,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr2[16] = {0x2,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x0,0x0,0x1,0x0,
    0x0,0x0,0x0,0x0};
SetWordType setwd1[118] = {0x0,0xfe,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0xbe,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x81,0x81,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x9e,0x82,0x8e,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr3[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0xc,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr4[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x80,0x10,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr5[16] = {0x0,0x0,0x0,0x10, 0x0,0x0,0x0,0x0, 0x0,0x0,0x20,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr6[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x80,0x0,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr7[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x20,0x0,0x40,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr8[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x91,0x7,
    0x0,0x0,0x0,0x0};
SetWordType setwd2[118] = {0x0,0x23,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0xa3,0xc,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x1,0x1,0x0,0x0,0x14,0x80,
    0x0,0x0,0x80,0x0,0x80,0x0,0x0,0x0,
    0x0,0xe3,0x1,0xa3,0x80,0x0,0xc,0x14,
    0xc0,0x80,0x80,0xc0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr9[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x81,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr10[16] = {0x2,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0xc0,0xcd,0x91,0xf,
    0x0,0x0,0x0,0x0};
SetWordType zzerr11[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x81,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr12[16] = {0x0,0x0,0x0,0x18, 0x0,0x0,0x0,0x0, 0x40,0x48,0xf1,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr13[16] = {0x0,0x0,0x0,0x10, 0x0,0x0,0x0,0x0, 0x0,0x0,0x20,0x0,
    0x0,0x0,0x0,0x0};
SetWordType setwd3[118] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x4d,0xc8,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4d,
    0x0,0x0,0x4d,0x8,0x49,0x0,0x0,0x0,
    0x0,0x78,0x0,0x4f,0x4f,0x8,0xc8,0x48,
    0x78,0x78,0x4d,0x78,0x8,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr14[16] = {0x0,0x0,0x0,0x18, 0x0,0x0,0x0,0x0, 0x40,0x48,0xe1,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr15[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0xc1,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr16[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x4c,0x81,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr17[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0xc0,0x4d,0x81,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr18[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x91,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr19[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x40,0x0,0x2,
    0x0,0x0,0x0,0x0};
SetWordType zzerr20[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0xc,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr21[16] = {0x0,0x0,0x0,0x18, 0x0,0x0,0x0,0x0, 0x40,0x48,0xf1,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr22[16] = {0x0,0x0,0x0,0x10, 0x0,0x0,0x0,0x0, 0x0,0x0,0x20,0x0,
    0x0,0x0,0x0,0x0};
SetWordType setwd4[118] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x7f,0xc0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,
    0x0,0x0,0x7f,0x0,0x7f,0x0,0x0,0x0,
    0x0,0x7f,0x0,0x7f,0x7f,0x0,0xc0,0x41,
    0x7f,0x7f,0x7f,0x7f,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr23[16] = {0x0,0x0,0x0,0x18, 0x0,0x0,0x0,0x0, 0x40,0x48,0xe1,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr24[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0xc1,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr25[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x91,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr26[16] = {0x2,0x0,0x0,0x18, 0x0,0x0,0x0,0x0, 0x40,0x8,0x81,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr27[16] = {0x2,0x0,0x0,0x18, 0x0,0x0,0x0,0x0, 0x40,0x48,0x81,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr28[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x91,0x7,
    0x0,0x0,0x0,0x0};
SetWordType zzerr29[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x91,0xf,
    0x0,0x0,0x0,0x0};
SetWordType setwd5[118] = {0x0,0x8,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0xff,0x8,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,
    0x0,0x0,0xf7,0x0,0xff,0x0,0x0,0x0,
    0x0,0xff,0x0,0xf7,0xf7,0x0,0x0,0x1,
    0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr30[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x10,0x6,
    0x0,0x0,0x0,0x0};
SetWordType zzerr31[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x10,0xe,
    0x0,0x0,0x0,0x0};
SetWordType zzerr32[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x10,0x6,
    0x0,0x0,0x0,0x0};
SetWordType zzerr33[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0xc,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr34[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x10,0x6,
    0x0,0x0,0x0,0x0};
SetWordType zzerr35[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x10,0xe,
    0x0,0x0,0x0,0x0};
SetWordType zzerr36[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x40,0x48,0x10,0x6,
    0x0,0x0,0x0,0x0};
SetWordType zzerr37[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x2,0x8,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr38[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x40,0x0,0x0,
    0x0,0x0,0x0,0x0};
SetWordType zzerr39[16] = {0x0,0x0,0x0,0x8, 0x0,0x0,0x0,0x0, 0x60,0x48,0x10,0xe,
    0x0,0x0,0x0,0x0};
SetWordType setwd6[118] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0xef,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xef,
    0x0,0x0,0xef,0x0,0xef,0x0,0x0,0x0,
    0x0,0x0,0x0,0xff,0xff,0x1,0x0,0x0,
    0x0,0x0,0xef,0xef,0x1,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr40[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x44,0x0,0x0};
SetWordType zzerr41[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x80,0x40,0x0,0x0};
SetWordType zzerr42[16] = {0x2,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x80,0x44,0x0,0x0};
SetWordType zzerr43[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x90,0x0,0x0};
SetWordType setwd7[118] = {0x0,0xc0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x1d,0x8,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1d,
    0x0,0x0,0x1d,0x8,0x19,0x0,0x0,0x0,
    0x0,0x18,0x0,0x1f,0x1f,0x8,0x8,0x8,
    0x10,0x18,0x1d,0x18,0x8,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x20,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr44[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x0,0x13,0x0};
SetWordType zzerr45[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x0,0x13,0x0};
SetWordType zzerr46[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x0,0x13,0x0};
SetWordType zzerr47[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x0,0x13,0x0};
SetWordType zzerr48[16] = {0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
    0x0,0x10,0x1e,0x0};
SetWordType setwd8[118] = {0x0,0x20,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,
    0x0,0x0,0x1f,0x0,0x0,0x1f,0x0};
