/*
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
 * PCCTS 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1992-2000
 */

/** \file slist.cpp */

#define ANTLR_SUPPORT_CODE

#include "SList.h"
#include "pccts_stdarg.h"

/**
 * Iterate over a list of elements; returns ptr to a new element
 * in list upon every call and NULL when no more are left.
 * Very useful like this:
 *
 *    cursor = mylist;
 *    while ( (p=mylist->iterate(&cursor)) ) {
 *      // place with element p
 *    }
 *
 * The cursor must be initialized to point to the list to iterate over.
 */
void *SList::iterate(SListNode **cursor)
{
  void *e;

  if ( cursor == NULL || *cursor==NULL ) return NULL;
  if ( head == *cursor ) { *cursor = (*cursor)->next(); }
  e = (*cursor)->elem();
  (*cursor) = (*cursor)->next();
  return e;
}

/** add an element to end of list. */
void SList::add(void *e)
{
  SListNode *p, *tail=NULL;
  require(e!=NULL, "slist_add: attempting to add NULL list element");

  p = new SListNode;
  require(p!=NULL, "add: cannot alloc new list node");

  p->setElem(e);
  if ( head == NULL )
  {
    head = tail = p;
  }
  else                /* find end of list */
  {
    tail->setNext(p);
    tail = p;
  }
}

void SList::lfree()
{
  SListNode *p,*q;

  if ( head==NULL ) return; /* empty list */
  for (p = head; p!=NULL; p=q)
  {
    q = p->next();
    free(p);
  }
}

PCCTS_AST *SList::to_ast(SList list)
{
  PCCTS_AST *t=NULL, *last=NULL;
  SListNode *p;

  for (p = head; p!=NULL; p=p->next())
  {
    PCCTS_AST *u = (PCCTS_AST *)p->elem();
    if ( last==NULL ) last = t = u;
    else { last->setRight(u); last = u; }
  }
  return t;
}

int SList::printMessage(FILE* pFile, const char* pFormat, ...)
{
  va_list marker;
  va_start( marker, pFormat );
  int iRet = vfprintf(pFile, pFormat, marker);
  va_end( marker );
  return iRet;
}
