#
# Makefile for ANTLR 1.33
#
# SOFTWARE RIGHTS
#
# We reserve no LEGAL rights to the Purdue Compiler Construction Tool
# Set (PCCTS) -- PCCTS is in the public domain.  An individual or
# company may do whatever they wish with source code distributed with
# PCCTS or the code generated by PCCTS, including the incorporation of
# PCCTS, or its output, into commerical software.
#
# We encourage users to develop software with PCCTS.  However, we do ask
# that credit is given to us for developing PCCTS.  By "credit",
# we mean that if you incorporate our source code into one of your
# programs (commercial product, research project, or otherwise) that you
# acknowledge this fact somewhere in the documentation, research report,
# etc...  If you like PCCTS and have developed a nice tool with the
# output, please mention that you developed it using PCCTS.  In
# addition, we ask that this header remain intact in our source code.
# As long as these guidelines are kept, we expect to continue enhancing
# this system and expect to make other tools available as they are
# completed.
#
# ANTLR 1.33
# Terence Parr
# Parr Research Corporation
# with Purdue University
# and AHPCRC, University of Minnesota
# 1989-1995
#
# Ported to Borland C++, IBM C-Set/2 and Microsoft 6.0 by
# Ed Harfmann
# Micro Data Base Systems
# Lafayette, Indiana
#

COMMON=../common
COMMON_H=../common

CC ?= gcc
CPPFLAGS += -I$(COMMON_H)

#ifndef PREFIX
PREFIX=/usr/local
#endif


SRC=antlr.c scan.c error.c bits.c build.c fset2.c fset.c gen.c globals.c \
	hash.c lex.c main.c misc.c $(COMMON)/set.c pred.c egman.c mrhoist.c \
	fcache.c $(COMMON)/logger.c utils.c antlr_log.c

OBJ=antlr.o scan.o error.o bits.o build.o fset2.o fset.o gen.o \
        globals.o hash.o lex.o main.o misc.o set.o pred.o egman.o mrhoist.o \
        fcache.o logger.o utils.o antlr_log.o

antlr : $(OBJ) $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(COTHER) -o antlr $(OBJ)



antlr.o : antlr.c mode.h tokens.h

scan.o : scan.c mode.h tokens.h


set.o : $(COMMON)/set.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(COTHER) $<

logger.o : $(COMMON)/logger.c $(COMMON)/logger.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(COTHER) $<

#clean up all the intermediate files
clean:
	rm -f *.o core

#remove everything in clean plus binaries
scrub: clean
	rm -f antlr

install: antlr
	install antlr $(PREFIX)/bin
	install antlr.1 $(PREFIX)/man/man1
