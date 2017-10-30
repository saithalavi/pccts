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
 */

/**
 * dlg header file
 */

#ifndef DLG_P_H__
#define DLG_P_H__

#include <stdio.h>
#include "set.h"

/***** output related stuff *******************/
#define DEFAULT_CLASSNAME "DLGLexer"


/* indicates that the not an "array" reference */
#define NIL_INDEX 0




#define nfa_node struct _nfa_node
nfa_node {
  int   node_no;
  int   nfa_set;
  int   accept; /* what case to use */
  nfa_node  *trans[2];
  set   label;  /* one arc always labelled with epsilon */
};

/******** antlr attributes *************/
#define zzcr_attr(attr, token, text) {          \
  (attr)->letter = text[0]; (attr)->l = NULL;     \
  (attr)->r = NULL; (attr)->label = empty;      \
}

typedef struct {
  unsigned char letter;
  nfa_node *l,*r;
  set label;
} Attrib;

/******** macros for accessing the NFA nodes ****/
#define NFA(x)  (nfa_array[x])
#define NFA_NO(x) ( (x) ? (x)->node_no : NIL_INDEX)

/* these macros allow the size of the character set to be easily changed */
/* NOTE: do NOT change MIN_CHAR since EOF is the lowest char, -1 */
#define MIN_CHAR (-1) /* lowest possible character possible on input */
#define MAX_CHAR 255  /* highest possible character possible on input */
#define CHAR_RANGE (1+(MAX_CHAR) - (MIN_CHAR))


/******************** Variable ******************************/
extern nfa_node **nfa_array;  /* start of nfa "array" */
extern int  action_no;  /* last action function printed */
extern set  used_chars; /* used to label trans. arcs */
extern set  used_classes; /* classes or chars used to label trans. arcs */
extern set  normal_chars; /* mask off unused portion of set */
extern int  mode_counter; /* keeps track of the number of %%name */
extern int  gen_cpp;
extern int  func_action;  /* should actions be turned into functions?*/ //dlg_a.c
extern int  firstLexMember; /* defined in dlg_a.c */
extern int  comp_level; /* what compression level to use */ // main.c
extern char *file_str[];  /* file names being used */ //main.c



/******************** Functions ******************************/
void make_nfa_model_node();



#endif
