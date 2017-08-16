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
 * \file main.c
 * Main function for dlg version
 */

#include <libgen.h> /* for basename() */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "dlg.h"
#include "tokens.h"
#include "constants.h"

#ifndef ANTLR_VERSION
#define ANTLR_VERSION 13333
#endif

#define zzSET_SIZE 8
#include "antlr.h"
#include "dlgdef.h"

/* tells what version this is */
#define VERSION "1.33MR33"
/* name of file for mode output */
#define MODE_FILE "mode.h"

int numfiles = 0;
char *file_str[2] = {NULL, NULL};
char *class_name = DEFAULT_CLASSNAME;
char *OutputDirectory = TopDirectory;

/* Option variables */
int comp_level = 0;
int interactive = FALSE;
int case_insensitive = FALSE;
int warn_ambig = FALSE;
int gen_cpp = FALSE;


/* Option List Stuff */

void p_comp0()    {comp_level = 0;}
void p_comp1()    {comp_level = 1;}
void p_comp2()    {comp_level = 2;}
void p_stdio()    { file_str[numfiles++] = NULL;}
void p_file(char *s)  { file_str[numfiles++] = s;}
void p_cl_name(char *s, char *t)
  {
    if ( gen_cpp ) {
      class_name = t;
    }
    else {
      warning("-cl only valid in C++ mode; -cl ignored...",0);
    }
  }
void p_outdir(char *s,char *t) {OutputDirectory=t;}
void p_interactive()  {interactive = TRUE;}
void p_case_s()   { case_insensitive = FALSE; }
void p_case_i()   { case_insensitive = TRUE; }
void p_warn_ambig() { warn_ambig = TRUE; }
void p_cpp()    { gen_cpp = TRUE; }


typedef void (*WildFunc)();

typedef struct {
  char *option;
  int  arg;
  WildFunc process;
  char *descr;
} Opt;

Opt options[] = {
  { "-CC", 0, (WildFunc)p_cpp, "Generate C++ output" },
  { "-C0", 0, (WildFunc)p_comp0, "No compression (default)" },
  { "-C1", 0, (WildFunc)p_comp1, "Compression level 1" },
  { "-C2", 0, (WildFunc)p_comp2, "Compression level 2" },
  { "-Wambiguity", 0, (WildFunc)p_warn_ambig, "Warn if expressions ambiguous"},
  { "-i", 0, (WildFunc)p_interactive, "Build interactive scanner (not valid for C++ mode)"},
  { "-ci", 0, (WildFunc)p_case_i, "Make lexical analyzer case insensitive"},
  { "-cl", 1, (WildFunc)p_cl_name, "Rename lexer class (DLGLexer); only used for -CC"},
  { "-cs", 0, (WildFunc)p_case_s, "Make lexical analyzer case sensitive (default)"},
  { "-o",  1, (WildFunc)p_outdir, OutputDirectoryOption},
  { "-", 0, (WildFunc)p_stdio, "Use standard i/o rather than file"},
  { "*", 0, (WildFunc)p_file, ""}, /* anything else is a file */
  { NULL, 0, NULL }
 };


static int ci_strequ(char *a,char *b)
{
  for ( ;*a != 0 && *b != 0; a++, b++) {
    if (toupper(*a) != toupper(*b)) return 0;
  }
  return (*a == *b);
}


static void ProcessArgs(int argc, char **argv, Opt *options)
{
  Opt *p;

  while ( argc-- > 0 )
  {
    p = options;
    while ( p->option != NULL )
    {
      if ( strcmp(p->option, "*") == 0 ||
         ci_strequ(p->option,*argv) )
      {
        if ( p->arg )
        {
          (*p->process)( *argv, *(argv+1) );
          argv++;
          argc--;
        }
        else
          (*p->process)( *argv );
        break;
      }
      p++;
    }
    argv++;
  }
}


/**
 * Print usage info and quit the program.
 */
static void usage(char *program)
{
  Opt *p = options;
  fprintf(stderr, "%s [options] f1 f2 ... fn\n",program);
  while ( *(p->option) != '*' )
  {
    fprintf(stderr, "\t%s %s\t%s\n",
            p->option,
            (p->arg)?"___":"   ",
            p->descr);
    p++;
  }

  exit(PCCTS_EXIT_FAILURE);
}



int main(int argc, char *argv[])
{
  init();
  fprintf(stderr, "%s  Version %s   1989-2001\n", basename(argv[0]), VERSION);
  if ( argc == 1 )
  {
    usage(argv[0]);
  }

  ProcessArgs(argc-1, &(argv[1]), options);
  if (interactive && gen_cpp) {
    fprintf(stderr,"\n");
  }
  input_stream = read_stream(file_str[0]);
  if (input_stream) {
    /* don't overwrite unless input okay */
    if ( gen_cpp ) {
      output_stream = write_stream(OutputDirectory, ClassName(CPP_FILE_SUFFIX));
      if ( file_str[1]!=NULL ) {
        warning("output file implicit in C++ mode; ignored...",0);
      }
      class_stream = write_stream(OutputDirectory, ClassName(".h"));
      mode_stream = class_stream;
    }
    else {
      output_stream = write_stream(OutputDirectory, file_str[1]);
      mode_stream = write_stream(OutputDirectory, MODE_FILE);
    }
  }
  /* make sure that error reporting routines in grammar
     know what the file really is */
  /* make sure that reading and writing somewhere */
  if (input_stream && output_stream && mode_stream){
    ANTLR(grammar(VERSION, MODE_FILE), input_stream);
  }
  p_class_def2();     /* MR1 */

  if ( output_stream!=NULL ) fclose(output_stream);
  if ( !gen_cpp && mode_stream!=NULL ) fclose(mode_stream);
  if ( class_stream!=NULL ) fclose(class_stream);
  exit(PCCTS_EXIT_SUCCESS);
  return 0;   /* get rid of warning message MR1 */
}

/* initialize all the variables */
void init()
{
  register int i;

#ifdef SPECIAL_INITS
  special_inits();          /* MR1 */
#endif
  used_chars = empty;
  used_classes = empty;
  /* make the valid character set */
  normal_chars = empty;
  /* NOTE: MIN_CHAR is EOF */
  /* NOTE: EOF is not quite a valid char, it is special. Skip it*/
  for (i = 1; i<CHAR_RANGE; ++i){
    set_orel(i,&normal_chars);
  }
  make_nfa_model_node();
  clear_hash();
  /* NOTE: need to set this flag before the lexer starts getting */
  /* tokens */
  func_action = FALSE;
}

/** stuff that needs to be reset when a new automaton is being built */
void new_automaton_mode()
{
  set_free(used_chars);
  clear_hash();
}
