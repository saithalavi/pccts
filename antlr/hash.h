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
 * \file hash.h
 * \brief define hash table entries, sizes, hash function...
 */

#ifndef __HASH_H_
#define __HASH_H_

        /* H a s h  T a b l e  S t u f f */

/** Number of 'Entry' in a hashtable, which is a table of 'Entry' */
#ifndef HashTableSize
#define HashTableSize 553
#endif

#ifndef StrTableSize
#define StrTableSize 1000000
#endif

/** Minimum hash table entry -- superclass */
typedef struct _entry {
  char *str;
  struct _entry *next;
} Entry;



Entry *hash_get(Entry **, char *);

Entry **newHashTable();

Entry *hash_add(Entry **table, char *key, Entry *rec);

void  killHashTable(Entry **table);

/**
 * Add a string to the string table and return a pointer to it.
 * Bump the pointer into the string table to next avail position.
 */
char *mystrdup(char *s);

#endif
