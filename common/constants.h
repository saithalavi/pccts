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
 * \file constants.h (for ANTLR, DLG, and SORCERER)
 */

#ifndef __CONSTANTS_H
#define __CONSTANTS_H


/* largest file name size */
#define MaxFileName   300


#define ErrHdr "%s, line %d:"


#define zzDIE   return 1;
#define zzDONE  return 0;


#define TopDirectory  "."
#define DirectorySymbol "/"
#define CPP_FILE_SUFFIX   ".cpp"


#define ATOKEN_H      "AToken.h"
#define ATOKPTR_H     "ATokPtr.h"
#define ATOKPTR_IMPL_H    "ATokPtrImpl.h"
#define ATOKENBUFFER_H    "ATokenBuffer.h"
#define ATOKENSTREAM_H    "ATokenStream.h"
#define APARSER_H     "AParser.h"
#define ASTBASE_H     "ASTBase.h"
#define DLEXERBASE_H    "DLexerBase.h"
#define DLEXER_H      "DLexer.h"

/* SORCERER Stuff */
#define STPARSER_H      "STreeParser.h"



#endif
