/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: dlg_p.g
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

#include <ctype.h>
#include "dlg.h"
#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[46]={
  /* 00 */  "Invalid",
  /* 01 */  "@",
  /* 02 */  "[\\r\\t\\ ]+",
  /* 03 */  "\\n",
  /* 04 */  "L_EOF",
  /* 05 */  "PER_PER",
  /* 06 */  "NAME_PER_PER",
  /* 07 */  "LEXMEMBER",
  /* 08 */  "LEXACTION",
  /* 09 */  "PARSERCLASS",
  /* 10 */  "LEXPREFIX",
  /* 11 */  "ACTION",
  /* 12 */  "GREAT_GREAT",
  /* 13 */  "L_BRACE",
  /* 14 */  "R_BRACE",
  /* 15 */  "L_PAR",
  /* 16 */  "R_PAR",
  /* 17 */  "L_BRACK",
  /* 18 */  "R_BRACK",
  /* 19 */  "ZERO_MORE",
  /* 20 */  "ONE_MORE",
  /* 21 */  "OR",
  /* 22 */  "RANGE",
  /* 23 */  "NOT",
  /* 24 */  "OCTAL_VALUE",
  /* 25 */  "HEX_VALUE",
  /* 26 */  "DEC_VALUE",
  /* 27 */  "TAB",
  /* 28 */  "NL",
  /* 29 */  "CR",
  /* 30 */  "BS",
  /* 31 */  "CONTINUATION",
  /* 32 */  "LIT",
  /* 33 */  "REGCHAR",
  /* 34 */  "\\>",
  /* 35 */  "\\\\>",
  /* 36 */  "\\",
  /* 37 */  "\\n",
  /* 38 */  "/\\*",
  /* 39 */  "//",
  /* 40 */  "~[]",
  /* 41 */  "\\*/",
  /* 42 */  "[\\n\\r]",
  /* 43 */  "~[]",
  /* 44 */  "[\\n\\r]",
  /* 45 */  "~[]"
};
SetWordType zzerr1[8] = {0x80,0xf,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr2[8] = {0x60,0x0,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr3[8] = {0x70,0xa8,0x9a,0x7f, 0x3,0x0,0x0,0x0};
SetWordType setwd1[46] = {0x0,0x6,0x0,0x0,0x30,0xc8,0xc8,
  0x1,0x1,0x1,0x1,0x35,0x0,0x30,0x0,
  0x30,0x0,0x30,0x0,0x30,0x30,0x0,0x0,
  0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
  0x0,0x30,0x30,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr4[8] = {0x10,0xa8,0x9a,0x7f, 0x3,0x0,0x0,0x0};
SetWordType zzerr5[8] = {0x10,0xe8,0xbb,0x7f, 0x3,0x0,0x0,0x0};
SetWordType zzerr6[8] = {0x10,0xa0,0x9a,0x7f, 0x3,0x0,0x0,0x0};
SetWordType setwd2[46] = {0x0,0x0,0x0,0x0,0xeb,0x2,0x2,
  0x0,0x0,0x0,0x0,0xd6,0x0,0xeb,0xd4,
  0xeb,0xd4,0xeb,0x0,0xcb,0xcb,0xd0,0x0,
  0xeb,0xeb,0xeb,0xeb,0xeb,0xeb,0xeb,0xeb,
  0x0,0xeb,0xeb,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0};
SetWordType zzerr7[8] = {0x10,0xa0,0x82,0x7f, 0x3,0x0,0x0,0x0};
SetWordType zzerr8[8] = {0x10,0x0,0x44,0x7f, 0x3,0x0,0x0,0x0};
SetWordType zzerr9[8] = {0x10,0x0,0x0,0x7f, 0x3,0x0,0x0,0x0};
SetWordType setwd3[46] = {0x0,0x0,0x0,0x0,0xf7,0x0,0x0,
  0x0,0x0,0x0,0x0,0xc2,0x0,0xc2,0xc2,
  0xc2,0xc2,0xc2,0xb8,0xc2,0xc2,0xc2,0x80,
  0xc2,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,
  0x0,0xf7,0xf7,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0};
