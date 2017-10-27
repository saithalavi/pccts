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
 * DLG 1.33
 * Will Cohen
 * With mods by Terence Parr; AHPCRC, University of Minnesota
 * 1989-2001
 */

/**
 * \file automata.c
 * Automata conversion functions for DLG
 */

#include <stdio.h>
#include <stdlib.h>

#include "dlg.h"
#include "relabel.h"
#include "automata.h"


int dfa_allocated = 0;  /* keeps track of number of dfa nodes */
dfa_node  **dfa_array;  /* root of binary tree that stores dfa array */
dfa_node  *dfa_model_node;
hash_list   *dfa_hash[HASH_SIZE]; /* used to quickly find */
          /* desired dfa node */

static void close1(nfa_node *node, int o, set *b);


/** adds a new nfa to the binary tree and returns a pointer to it */
static dfa_node *new_dfa_node(set nfa_states)
{
  register int j;
  register dfa_node *t;
  static int dfa_size=0;  /* elements dfa_array[] can hold */

  ++dfa_allocated;
  if (dfa_size<=dfa_allocated){
    /* need to redo array */
    if (!dfa_array){
      /* need some to do inital allocation */
      dfa_size=dfa_allocated+DFA_MIN;
      dfa_array=(dfa_node **) malloc(sizeof(dfa_node*)*
        dfa_size);
    }else{
      /* need more space */
      dfa_size=2*(dfa_allocated+1);
      dfa_array=(dfa_node **) realloc(dfa_array,
        sizeof(dfa_node*)*dfa_size);
    }
  }
  /* fill out entry in array */
  t = (dfa_node*) malloc(sizeof(nfa_node)+sizeof(int)*class_no);
  *t = *dfa_model_node;
  for (j=0; j<class_no; ++j)
    t->trans[j] = NIL_INDEX;
  t->node_no = dfa_allocated;
  t->nfa_states = set_dup(nfa_states);
  dfa_array[dfa_allocated] = t;
  return t;
}


/**
 * Returns a pointer to a dfa node that has the same nfa nodes in it.
 * This may or maynot be a newly created node.
 */
static dfa_node *dfastate(set nfa_states)
{
  register hash_list *p;
  int bin;

  /* hash using set and see if it exists */
  bin = set_hash(nfa_states,HASH_SIZE);
  p = dfa_hash[bin];
  while(p && !set_equ(nfa_states,(p->node)->nfa_states)){
    p = p->next;
  }
  if(!p){
    /* next state to add to hash table */
    p = (hash_list*)malloc(sizeof(hash_list));
    p->node = new_dfa_node(nfa_states);
    p->next = dfa_hash[bin];
    dfa_hash[bin] = p;
  }
  return (p->node);
}


/* this reach assumes the closure has been done already on set */
static int reach(unsigned *nfa_list, register int a, unsigned *reach_list)
{
  register unsigned *e;
  register nfa_node *node;
  int t=0;

  e = nfa_list;
  if (e){
    while (*e != nil){
      node = NFA(*e);
      if (set_el(a,node->label)){
        t=1;
        *reach_list=NFA_NO(node->trans[0]);
        ++reach_list;
      }
      ++e;
    }
  }
  *reach_list=nil;
  return t;
}


/* finds all the nodes that can be reached by epsilon transitions
   from the set of a nodes and returns puts them back in set b */
static set closure(set *b, unsigned *reach_list)
{
  register nfa_node *node,*n; /* current node being examined */
  register unsigned *e;

  ++operation_no;
  e=reach_list;

  while (*e != nil){
    node = NFA(*e);
    set_orel(NFA_NO(node),b);
    /* mark it done */
    node->nfa_set = operation_no;
    if ((n=node->trans[0]) != NIL_INDEX && set_nil(node->label) &&
      (n->nfa_set != operation_no)){
      /* put in b */
      set_orel(NFA_NO(n),b);
      close1(n,operation_no,b);
    }
    if ((n=node->trans[1]) != NIL_INDEX &&
      (n->nfa_set != operation_no)){
      /* put in b */
      set_orel(NFA_NO(node->trans[1]),b);
      close1(n,operation_no,b);
    }
    ++e;
  }
  return *b;
}


/**
 * \param o marker to avoid cycles
 */
static void close1(nfa_node *node, int o, set *b)
{
  register nfa_node *n; /* current node being examined */

  /* mark it done */
  node->nfa_set = o;
  if ((n=node->trans[0]) != NIL_INDEX && set_nil(node->label) &&
    (n->nfa_set != o)){
    /* put in b */
    set_orel(NFA_NO(n),b);
    close1(n,o,b);
  }
  if ((n=node->trans[1]) != NIL_INDEX &&
    (n->nfa_set != o)){
    /* put in b */
    set_orel(NFA_NO(node->trans[1]),b);
    close1(n,o,b);
  }
}


void make_dfa_model_node(int width)
{
  register int i;
  dfa_model_node = (dfa_node*) malloc(sizeof(dfa_node)
       + sizeof(int)*width);
  dfa_model_node->node_no = -1; /* impossible value for real dfa node */
  dfa_model_node->dfa_set = 0;
  dfa_model_node->alternatives = FALSE;
  dfa_model_node->done = FALSE;
  dfa_model_node->nfa_states = empty;
  for(i = 0; i<width; i++){
    dfa_model_node->trans[i] = NIL_INDEX;
  }
}


/**
 * past a pointer to the start start of the nfa graph
 * nfa_to_dfa convers this graph to dfa.  The function returns
 * a pointer to the first dfa state.
 * NOTE:  The function that prints out the table will have to figure out how
 * to find the other dfa states given the first dfa_state and the number of dfa
 * nodes allocated
 */
dfa_node **nfa_to_dfa(nfa_node *start)
{
  register dfa_node *d_state, *trans_d_state;
  register int a;
  set t;
  int last_done;
  unsigned *nfa_list;
  unsigned *reach_list;

  reach_list = (unsigned *) malloc((2+nfa_allocated)*sizeof(unsigned));
  if (!start) return NULL;
  t = set_of(NFA_NO(start));
  _set_pdq(t,reach_list);
  closure(&t,reach_list);
  /* Make t a dfa state */
  d_state = dfastate(t);
  last_done = DFA_NO(d_state);

  do {
    /* Mark dfa state x as "done" */
    d_state->done = TRUE;
    nfa_list = set_pdq(d_state->nfa_states);
    for (a = 0; a<class_no; ++a) {
      /* Add NFA states reached by a from d_state */
      reach(nfa_list,a,reach_list);
      /* Were any states found? */
      if ((*reach_list)!=nil) {
        /* was t=empty; */
        set_free(t);
        /* yes, compute closure */
        closure(&t,reach_list);
        /* Make DFA state of it ... */
        trans_d_state = dfastate(t);
        /* And make transition x->t, labeled with a */
        d_state->trans[a] = DFA_NO(trans_d_state);
        d_state->alternatives = TRUE;
      }
    }
    free(nfa_list);
    ++last_done; /* move forward in queue */
    /* And so forth until nothing isn't done */
    d_state = DFA(last_done);
  } while (last_done<=dfa_allocated);

  free(reach_list);
  set_free(t);

  /* returns pointer to the array that holds the automaton */
  return dfa_array;
}

void clear_hash()
{
  int i;

  for(i=0; i<HASH_SIZE; ++i) {
    dfa_hash[i] = 0;
  }
}
