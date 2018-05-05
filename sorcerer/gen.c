/* Code generator for SORCERER
 *
 * Translate sorcerer rules (in intermediate trees) to C / C++
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
 *
 * The latest (October 1994) transformation mode works as follows:
 *
 *    o All elements set _tresult, a function-scoped var
 *    o All rules are passed the address of a SORAST ptr
 *      called '_result' that is set at the end of each
 *      alternative.  The return tree is guaranteed to
 *      be complete only at this point.
 *    o Each alt AND #(...) tree have their own root (_r),
 *      sibling (_s) and tail/end pointers (_e).  #(...)  trees
 *      set _tresult just before exiting their scope.  Hence,
 *      #(...) are just like elements--they set _tresult.
 *    o At the end of a rule, *_result is set to _tresult.
 *    o After each root node ref, a _mkroot(&_r,&_s,&_e,_tresult)
 *      is generated to set the pointers.
 *    o After each child node ref, a _mkchild(&_r,&_s,&_e,_tresult)
 *      is generated to set the pointers.
 *    o After each rule ref, a _mkroot(&_r,&_s,&_e,_tresult)
 *      is generated to set the pointers.
 *    o After each #(...) ref, a _mkroot(&_r,&_s,&_e,_tresult)
 *      is generated to set the pointers.
 *    o token references: gen a _tresult=ast_dup(_t);
 *      (only if labeled, however).
 *    o rules or alts with nocopy set don't construct trees.
 */

#include "stdpccts.h"
#include "constants.h"
#include "sym.h"
#include "proto.h"

/* User may redefine how line information looks */
#ifndef LineInfoFormatStr
#define LineInfoFormatStr "#line %d \"%s\"\n"
#endif

static int outfile = -1;
static char *current_rule;
static ListNode *labels_for_func = NULL;
static AST *whichRule;
static int tabs=0;

static void gen_rule( AST *t );
static void gen_block( AST *q, int block_type );
static void gen_alt( AST *t );
static void gen_tree_pattern( AST *t );
static void gen_element( AST *t );
static void define_labels_in_block( AST *t );
static void define_labels_in_alt( AST *t );
static void define_labels_in_tree( AST *t );
static void define_labels_in_element( AST *t );
static set gen_prediction_expr( AST *alt, GLA *t );
static void GenRulePrototype(FILE *f, AST *p, SymEntry *s, int decl_not_def);
static AST *first_guess_block(AST *q);
static void code_for_guess_block( AST *t, int *guess_block_in_prev_alt );

static void indent(FILE *output) {
    int i;
    for (i=0; i<tabs; i++) {
        putc('\t', output);
    }
}

#define PastWhiteSpace(s) while (*(s) == ' ' || *(s) == '\t') {s++;}


static void go_down(int nl)
{
  fprintf(output, "_DOWN;");
  if ( nl ) fprintf(output, "\n");
}

static void go_right(int nl)
{
  fprintf(output, "_RIGHT;");
  if ( nl ) fprintf(output, "\n");
}

void gen_info_hdr(FILE *f)
{
  fprintf(f, "/*\n");
  fprintf(f, " * S O R C E R E R  T r a n s l a t i o n  H e a d e r\n");
  fprintf(f, " *\n");
  fprintf(f, " * SORCERER Developed by Terence Parr, Aaron Sawdey, & Gary Funck\n");
  fprintf(f, " * Parr Research Corporation, Intrepid Technology, University of Minnesota\n");
  fprintf(f, " * 1992-1994\n");
  fprintf(f, " * SORCERER Version %s\n", VersionText);
  fprintf(f, " */\n");
}

static void gen_hdr_CPP(void)
{
  char CPPParser_h_Name[MaxFileName+1];

  gen_info_hdr(output);
  fprintf(output, "#define SORCERER_VERSION\t%s\n", VersionNumber);
  if ( transform ) fprintf(output, "#define SORCERER_TRANSFORM\n");
  else fprintf(output, "#define SORCERER_NONTRANSFORM\n");
/*  Don't gen header action here as it's in the ClassName.h
  if ( header_action!=NULL )
    dumpAction(header_action, output, 0, -1, 0, 1); */
  fprintf(output, "#include \"%s\"\n", STPARSER_H);

  if ( UserDefdTokens ) fprintf(output, "#include \"%s\"\n", tokdefs_file);
  strcpy(CPPParser_h_Name, CurClassName);
  strcat(CPPParser_h_Name, ".h");
  fprintf(output, "#include \"%s\"\n", CPPParser_h_Name);
}

static void gen_hdr_C(void)
{
  if ( Inline ) return;
  gen_info_hdr(output);
  fprintf(output, "#define SORCERER_VERSION\t%s\n", VersionNumber);
  if ( transform ) fprintf(output, "#define SORCERER_TRANSFORM\n");
  else fprintf(output, "#define SORCERER_NONTRANSFORM\n");
  fprintf(output, "#include \"pcctscfg.h\"\n");
  fprintf(output, "#include <stdio.h>\n");
  fprintf(output, "#include <setjmp.h>\n");
  if ( Prefix[0]!='\0' )
  {
    fprintf(output, "/* rename error routines; used in macros, must use /lib/cpp */\n");
    fprintf(output, "#define mismatched_token %smismatched_token\n", Prefix);
    fprintf(output, "#define mismatched_range %smismatched_range\n", Prefix);
    fprintf(output, "#define missing_wildcard %smissing_wildcard\n", Prefix);
    fprintf(output, "#define no_viable_alt %sno_viable_alt\n", Prefix);
    fprintf(output, "#define sorcerer_panic %ssorcerer_panic\n", Prefix);
  }
  if ( header_action!=NULL )
    dumpAction(header_action, output, 0, -1, 0, 1);

  /* make a #define for the STreeParser containing the ref vars */
  if ( AllRefVars!=NULL )
  {
    ListNode *p;
    fprintf(output, "#define _REFVARS \\\n");
    for (p = AllRefVars->next; p!=NULL; p=p->next)
    {
      RefVarRec *r = (RefVarRec *)p->elem;
      fprintf(output, "\t%s; \\\n", r->decl);
    }
    fprintf(output, "\n\n");
  }

  fprintf(output, "#include \"sorcerer.h\"\n");
  if ( UserDefdTokens ) fprintf(output, "#include \"%s\"\n", tokdefs_file);
  if ( GenProtoFile!=NULL ) fprintf(output, "#include \"%s\"\n", GenProtoFile);
  else GenRulePrototypes( output, 0 );
  if ( transform ) fprintf(output, "#include \"sorcerer.c\"\n");
}

static void gen_hdr(void)
{
  if ( GenCPP ) gen_hdr_CPP();
  else gen_hdr_C();
}

static void gen_hdr1(void)
{
  if ( !Inline )
  {
    gen_hdr();
  }

  if ( ! GenCPP )
  {
    /* make a func to init the ref vars with inits */
    fprintf(output, "\nvoid\n");
    fprintf(output, "_refvar_inits(STreeParser *p)\n");
    fprintf(output, "{\n");
    if ( AllRefVars!=NULL )
    {
      ListNode *p;
      for (p = AllRefVars->next; p!=NULL; p=p->next)
      {
        RefVarRec *r = (RefVarRec *)p->elem;
        if ( r->init[0]!='\0' )
          fprintf(output, "\tp->%s=%s;\n", r->var, r->init);
      }
    }
    fprintf(output, "}\n\n");
  }

  if ( before_actions != NULL )
  {
    ListNode *p;
    for (p = before_actions->next; p!=NULL; p=p->next)
      dumpAction( (char *)p->elem, output, 0, -1, 0, 1);
  }
}

void gen(AST *t)
{
  if ( t==NULL ) return;
  while ( t!=NULL && t->token == NonTerm )
  {
    if ( labels_for_func != NULL )
    {
      /* reset 'defined' flag for previously defined labels and free list */
      ListNode *p, *q;
      for (p = labels_for_func->next; p!=NULL; p=q)
      {
        SymEntry *s = (SymEntry *)p->elem;
        s->defined = 0;
        q = p->next;
        free(p);
      }
      free(labels_for_func);
      labels_for_func = NULL;
    }
    gen_rule(t);
    t = t->right;
  }

  if ( after_actions != NULL )
  {
    ListNode *p;
    for (p = after_actions->next; p!=NULL; p=p->next)
      dumpAction( (char *)p->elem, output, 0, -1, 0, 1);
  }
  fclose(output);
}

static void gen_rule( AST *t )
{
  SymEntry *s;

  if ( t==NULL ) return;
  whichRule=t;
  s = (SymEntry *) hash_get(symbols, t->text);
  require(s!=NULL, "gen_rule: sym tab broken");

  if ( t->file != outfile )      /* open new output file if need to */
  {
    if (strcmp(FileStr[t->file], "stdin")==0) output = stdout;
    else {
      if ( output != NULL ) fclose( output );
      output = fopen(OutMetaName(outname(FileStr[t->file])), "w");
      require(output != NULL, "gen_rule: can't open output file");
#ifdef SPECIAL_FOPEN
      special_fopen_actions(OutMetaName(outname(FileStr[t->file])));
#endif
    }
    if ( outfile == -1 ) gen_hdr1();
    else gen_hdr();
    outfile = t->file;
  }

  fprintf(output, "\n");
  current_rule = t->text;
  GenRulePrototype(output, t, s, 0);

  /* What locals do we need? */
  tabs=1;
  if ( GenCPP ) fprintf(output, "{\n\tSORASTBase *_t = *_root;\n");
  else fprintf(output, "{\n\tSORAST *_t = *_root;\n");
  if ( s->rt!=NULL ) fprintf(output, "\t%s;\n", s->rt);
  if ( transform )
  {
    if ( GenCPP ) fprintf(output, "\tSORASTBase *_tresult=NULL;\n");
    else fprintf(output, "\tSORAST *_tresult=NULL;\n");
    fprintf(output, "\tTREE_CONSTR_PTRS;\n");
  }

  if ( t->refvars!=NULL )
  {
    ListNode *p;
    for (p = t->refvars->next; p!=NULL; p=p->next)
    {
      RefVarRec *r = (RefVarRec *)p->elem;
      if ( !r->global ) {
        indent(output);
        dump_prefixed_decl("_save_", r->decl, output);
        if ( GenCPP ) fprintf(output, "=%s;\n", r->var);
        else fprintf(output, "=_parser->%s;\n", r->var);
      }
    }
  }

  define_labels_in_block(t->down);
  if ( found_guess_block ) fprintf(output, "\t_GUESS_BLOCK;\n");
  if ( transform ) fprintf(output, "  *_result = NULL;\n");
  gen_block(t->down, BLOCK);
  indent(output);
  fprintf(output, "*_root = _t;\n");

  /* reset any ref vars defined in this routine */
  if ( t->refvars!=NULL )
  {
    ListNode *p;
    for (p = t->refvars->next; p!=NULL; p=p->next)
    {
      RefVarRec *r = (RefVarRec *)p->elem;
      if ( !r->global )
      {
        if ( GenCPP ) fprintf(output, "\t%s = _save_%s;\n", r->var, r->var);
        else fprintf(output, "\t_parser->%s = _save_%s;\n", r->var, r->var);
      }
    }
  }

  /* set result of tree copying efforts if no ! on rule */
  if ( transform&&!t->no_copy ) {
    indent(output);
    /* The 'if' check in front of the *_result assignment ensures
     * that if someone sets the result before here, don't mess with
     * it.  This assignment is the default.
     */
    fprintf(output, "if ( (*_result) == NULL ) *_result = _r;\n");
  }

  /* send back and return value */
  if ( s->rt!=NULL )
  {
    char *str = s->rt;
    indent(output);
    fprintf(output, "return ");
    DumpNextNameInDef(&str, output);
    fprintf(output, ";\n");
  }

  tabs=0;
  fprintf(output, "}\n");
}

/**
 * The lookahead is stored in the 1st node of each alt of the block.
 * The Lookahead of what follows (for CLOSURE and OPT) blocks, is stored
 * in the node which branches over the enclosed, optional block.
 */
static void gen_block( AST *q, int block_type )
{
  AST *t = q, *u, *g;
  GLA *start_state, *follow=NULL;
  int need_right_curly=0, guess_block_in_prev_alt=0, num_alts=0;
  require(q!=NULL, "gen_block: NULL block");
  require(q->start_state!=NULL, "gen_block: NULL start state");

  start_state = t->start_state->p1;
  require(start_state!=NULL, "gen_block: no GLA start state for AST block");

  switch ( block_type ) {
  case PRED_OP :
    indent(output);
    fprintf(output, "{_SAVE; TREE_CONSTR_PTRS;\n");
    break;
  case CLOSURE :
    indent(output);
    fprintf(output, "{int _done=0;\n");
    indent(output);
    fprintf(output, "while ( !_done ) {\n");
    tabs++;
    break;
  case POS_CLOSURE :
    indent(output);
    fprintf(output, "{int _done=0;\n");
    indent(output);
    fprintf(output, "do {\n");
    tabs++;
  }

  /* Error if (...)? with only one alt */
  for (u = q->down; u!=NULL; u=u->right ) {num_alts++;}
  g = first_guess_block(q->down);
  if ( num_alts==1 && g!=NULL )
  {
    warnFL("(...)? predicate in block with one alternative; will generate bad code",
         FileStr[g->file], g->line);
  }

  t = t->down;  /* move down to first alt */
  while ( t!=NULL && t->token == ALT )
  {
    require(start_state!=NULL, "gen_block: invalid GLA for block");
    if ( t!=q->down ) {
      indent(output);
      fprintf(output, "else {\n");
      need_right_curly++;
    }
    code_for_guess_block(t, &guess_block_in_prev_alt);
    indent(output);
    fprintf(output, "if (");
    /* To prevent/detect infinite recursion, ensure 'visited' flag is set
     * on node starting this alt
     */
    start_state->visited = 1;
    start_state->lookahead = gen_prediction_expr(t, start_state->p1);
    start_state->visited = 0;
    fprintf(output, " ) {\n");
    tabs++;
    gen_alt(t);
    tabs--; indent(output);
    fprintf(output, "}\n");
    t = t->right;         /* move to next alt in AST */
    start_state = start_state->p2;  /* move to next alt in GLA */
  }

  switch ( block_type ) {
  case OPT :
  case CLOSURE :
    follow = q->start_state->p2;
    break;
  case POS_CLOSURE :
    follow = q->start_state->next;
  }

  if ( block_type == OPT || block_type == CLOSURE || block_type == POS_CLOSURE )
  {
    indent(output);
    fprintf(output, "else {\n");
    need_right_curly++;
    indent(output);
    fprintf(output, "if (");
    /* what follows the block? */
    q->start_state->visited = 1;
    q->start_state->lookahead = gen_prediction_expr(NULL, follow);
    q->start_state->visited = 0;
    fprintf(output, " ) {\n");
    if ( block_type != OPT )
    {
      tabs++; indent(output);
      fprintf(output, "_done = 1;\n");
      tabs--;
    }
    indent(output);
    fprintf(output, "}\n");
  }

  /* Generate error clause */
  indent(output);
  fprintf(output, "else {\n");
  /* generate the _GUESS_FAIL even if no (..)? found because calling
   * routine may be using STreeTest()
   */
  tabs++;
  indent(output);
  if ( GenCPP ) fprintf(output, "if ( guessing ) _GUESS_FAIL;\n");
  else fprintf(output, "if ( _parser->guessing ) _GUESS_FAIL;\n");
  indent(output);
  if ( GenCPP ) fprintf(output, "no_viable_alt(\"%s\", _t);\n", current_rule);
  else fprintf(output, "no_viable_alt(_parser, \"%s\", _t);\n", current_rule);
  tabs--;
  indent(output);
  fprintf(output, "}\n");
  if ( transform ) { indent(output); fprintf(output, " _tresult = _r;\n"); }

  for (;need_right_curly>0; need_right_curly--)
  {
    indent(output);
    fprintf(output, "}\n");
  }

  switch ( block_type ) {
  case BLOCK :
    break;
  case CLOSURE :
    tabs--;
    indent(output); fprintf(output, "}\n");
/*    if ( transform ) fprintf(output, " _tresult = _r;\n"); */
    indent(output); fprintf(output, "}\n");
    break;
  case POS_CLOSURE :
    tabs--;
    indent(output);
    fprintf(output, "} while ( !_done );\n");
/*    if ( transform ) { indent(output); fprintf(output, " _tresult = _r;\n"); } */
    indent(output);
    fprintf(output, "}\n");
    break;
  case PRED_OP :
    indent(output);
    fprintf(output, "_RESTORE;\n");
/*    if ( transform ) { indent(output); fprintf(output, " _tresult = _r;\n"); } */
    indent(output); fprintf(output, "}\n");
    indent(output); fprintf(output, "_GUESS_DONE;\n");
    break;
  }

  test_block_consistency(q, block_type);
}

static void gen_alt( AST *t )
{
  require(t!=NULL && t->token==ALT, "gen_alt: invalid alt");

  t = t->down;
  if ( t==NULL ) return;
  if ( t->token==Action ) t->init_action = 1;
  gen_tree_pattern(t);

  /* find last element of alternative to see if it's a simple token */
  while ( t->right!=NULL ) { t = t->right; }
  if ( t->down==NULL && (t->token == Token || t->token == WILD) )
    { indent(output); go_right(1); }
}

static void gen_tree_pattern( AST *t )
{
  while ( t != NULL )
  {
    /* could be root of a tree, check it */
    if ( t->down != NULL && (t->token==Token||t->token==WILD) ) {
      indent(output);
      fprintf(output, "{_SAVE; TREE_CONSTR_PTRS;\n");
    }
    gen_element(t);
    if ( t->down != NULL ) {
      if ( t->token == Token || t->token==WILD )
      {
        gen_tree_pattern(t->down);  /* only token/wildcard can be root of tree */
        indent(output);
        fprintf(output, "_RESTORE;");
        if ( transform ) fprintf(output, " _tresult = _r;");
        fprintf(output, "\n");
        indent(output); fprintf(output, "}\n");
        /* link in result of #(...) */
        if ( transform && !whichRule->no_copy ) {
          indent(output); fprintf(output, "_mkchild(&_r,&_s,&_e,_tresult);\n");
        }
        indent(output); go_right(1);
      }
    }
    t = t->right;
  }
}

static void gen_element( AST *t )
{
  char *res;
  require(t!=NULL, "gen_element: NULL tree pointer");

  switch ( t->token )
  {
      case Token :
      indent(output);
      if ( t->upper_range!=0 )
        fprintf(output, "_MATCHRANGE(%s,%s);",
            t->text, token_dict[t->upper_range]);
      else fprintf(output, "_MATCH(%s);", t->text);
      fprintf(output, "\n");
      /* Make copy of token if transform && (copy or (labeled&&!copy)) */
      if ( transform && (t->label[0]!='\0'||!t->no_copy) ) {
        indent(output);
        if ( GenCPP )
          fprintf(output, "_tresult=_t->shallowCopy();\n");
        else
          fprintf(output, "_tresult=ast_dup_node(_t);\n");
      }
      if ( transform && !t->no_copy ) {
        if ( t->is_root )
          fprintf(output, " _mkroot(&_r,&_s,&_e,_tresult);\n");
        else
          fprintf(output, " _mkchild(&_r,&_s,&_e,_tresult);\n");
      }
      /* Do Label if any */
      if ( transform  ) {   /* label is output, label_in is input */
        if ( t->label[0]!='\0' )
        {
          indent(output);
          fprintf(output,
              "%s=(SORAST *)_tresult; %s_in=(SORAST *)_t;\n",
              t->label,t->label);
        }
      } else {
        if ( t->label[0]!='\0' )
          { indent(output); fprintf(output, "%s=(SORAST *)_t;\n", t->label); }
      }
      /* Move tree parser pointer */
      if ( t->down != NULL ) { indent(output); go_down(1); }
      else if ( t->right != NULL ) { indent(output); go_right(1); }
      break;
    case WILD :
      indent(output);
      fprintf(output, "_WILDCARD;");
      /* Make copy of token or tree if transform */
      if ( transform ) {
        indent(output);
        if ( GenCPP )
          fprintf(output, "_tresult=_t->deepCopy();");
        else
          fprintf(output, "_tresult=ast_dup(_t);");
        /* wildcard can't be root, must be child */
        if ( !t->no_copy ) {
          if ( t->is_root )
            fprintf(output, " _mkroot(&_r,&_s,&_e,_tresult);");
          else
            fprintf(output, " _mkchild(&_r,&_s,&_e,_tresult);");
        }
      }
      fprintf(output, "\n");
      if ( transform  ) {
        if ( t->label[0]!='\0' )
        {
          indent(output);
          fprintf(output,
              "%s=(SORAST *)_tresult; %s_in=(SORAST *)_t;\n",
              t->label,t->label);
        }
      } else {
        if ( t->label[0]!='\0' )
          { indent(output); fprintf(output, "%s=(SORAST *)_t;\n", t->label); }
      }
      if ( t->down != NULL ) { indent(output); go_down(1); }
      else if ( t->right != NULL ) { indent(output); go_right(1); }
      break;
    case NonTerm :
      indent(output);
      /* For nontransform mode, do labels first */
      if (t->label[0]!='\0')
        fprintf(output, "%s=(SORAST *)_t; ", t->label);
      if ( t->out ) {
        if ( t->in ) fprintf(output, "%s=", t->down->right->text);
        else fprintf(output, "%s=", t->down->text);
      }
      fprintf(output, "%s%s", Prefix, t->text);
      if ( transform ) res = ", &_tresult"; else res = "";
      if ( GenCPP ) {
        if ( t->in ) fprintf(output, "(&_t%s, %s);",
                   res, t->down->text);
        else fprintf(output, "(&_t%s);", res);
      }
      else {
        if ( t->in ) fprintf(output, "(_parser, &_t%s, %s);",
                   res, t->down->text);
        else fprintf(output, "(_parser, &_t%s);", res);
      }
      fprintf(output, "\n");
      if (transform && t->label[0]!='\0')
        {
          indent(output);
          fprintf(output, "%s=(SORAST *)_tresult;\n", t->label);
        }
      /* Link in result of rule */
      if ( transform&&!t->no_copy ) {
        indent(output);
        fprintf(output, "_mkchild(&_r,&_s,&_e,_tresult);\n");
      }
      break;
    case Action :
      if ( !t->init_action && found_guess_block ) {
        indent(output);
        if ( GenCPP ) fprintf(output, "if ( !guessing ) {\n");
        else fprintf(output, "if ( !_parser->guessing ) {\n");
      }
      dumpAction(t->action, output, tabs, 0, 0, 1);
      if ( !t->init_action && found_guess_block ) {indent(output); fprintf(output, "}\n");}
      break;
    case CLOSURE :
      gen_block(t->down, CLOSURE);
      break;
    case POS_CLOSURE :
      gen_block(t->down, POS_CLOSURE);
      break;
    case OPT :
      gen_block(t->down, OPT);
      break;
    case BLOCK :
      gen_block(t, BLOCK);
      break;
    case PRED_OP :
      require(t->up!=NULL, "define_labels_in_element: GLA is broken");
      if ( t->up->down==t ) {
        if ( t->down->token == BLOCK ) gen_block(t->down, PRED_OP);
      }
      else if ( t->up->down->token==Action && t->up->down->right==t ) {
        if ( t->down->token == BLOCK ) gen_block(t->down, PRED_OP);
      }
      else {
        warnFL("predicate not at beginning of alternative; ignored",
             FileStr[t->file], t->line);
      }
      break;
    default :
      fatal(eMsgd("invalid elemental node type: %d", t->token));
  }
}

/** walk the block of a rule and define all labels to be "SORAST *label_i" */
static void define_labels_in_block( AST *t )
{
  require(t!=NULL, "define_labels_in_block: NULL tree pointer");
  require(t->token==BLOCK, "define_labels_in_block: invalid block");
  t = t->down;
  while ( t!=NULL )
  {
    define_labels_in_alt(t);
    t = t->right;
  }
}

static void define_labels_in_alt( AST *t )
{
  require(t!=NULL, "define_labels_in_alt: NULL tree pointer");
  require(t->token==ALT, "gen_alt: invalid alt");

  define_labels_in_tree(t->down);
}

static void define_labels_in_tree( AST *t )
{
  while ( t != NULL )
  {
    define_labels_in_element(t);
    /* could be root of a tree, check it */
    if ( t->down != NULL ) {
      if ( t->token == Token || t->token==WILD )
      {
        define_labels_in_tree(t->down);
      }
    }
    t = t->right;
  }
}

static void define_labels_in_element( AST *t )
{
  require(t!=NULL, "define_labels_in_element: NULL tree pointer");
  switch ( t->token )
  {
      case Token :
    case WILD :
    case NonTerm :
      if ( t->label[0]!='\0' ) {
        SymEntry *s;
        s = (SymEntry *) hash_get(symbols, t->label);
        require(s!=NULL, "define_labels_in_element: sym tab broken");
        if ( s->token==LABEL && !s->defined ) {
          s->defined = 1;
          indent(output);
          fprintf(output, "SORAST *%s=NULL", t->label);
          if ( transform ) fprintf(output, ",*%s_in=NULL", t->label);
          fprintf(output, ";\n");
          /* track which ones we define so that we can undefine
           * them for the next rule
           */
          list_add(&labels_for_func, (void *)s);
        }
      }
      break;
    case Action :
      break;
    case CLOSURE :
    case POS_CLOSURE :
      define_labels_in_block(t->down);
      break;
    case PRED_OP :
      if ( t->down->token != Action )
      {
        define_labels_in_block(t->down);
      }
      break;
    case OPT :
      define_labels_in_block(t->down);
      break;
    case BLOCK :
      define_labels_in_block(t);
      break;
  }
}

/* dump action 's' to file 'output' starting at "local" tab 'tabs'
   Dump line information in front of action if GenLineInfo is set
   If file == -1 then GenLineInfo is ignored.
   The user may redefine the LineInfoFormatStr to his/her liking
   most compilers will like the default, however.

   June '93; changed so that empty lines are left alone so that
   line information is correct for the compiler/debuggers.
*/
void
dumpAction( char *s, FILE *output, int tabs, int file, int line, int final_newline )
{
    int inDQuote, inSQuote;
    require(s!=NULL, "dumpAction: NULL action");
    require(output!=NULL, eMsg1("dumpAction: output FILE is NULL for %s",s));

/*  if ( GenLineInfo && file != -1 )
  {
    fprintf(output, LineInfoFormatStr, line, FileStr[file]);
  }*/
    PastWhiteSpace( s );
    /* don't print a tab if first non-white char is a # (preprocessor command) */
    if ( *s!='#' ) {
        indent(output);
    }
    inDQuote = inSQuote = 0;
    while ( *s != '\0' )
    {
        if ( *s == '\\' )
        {
            putc( *s++, output ); /* Avoid '"' Case */
            if ( *s == '\0' ) return;
            if ( *s == '\'' ) putc( *s++, output );
            if ( *s == '\"' ) putc( *s++, output );
        }
        if ( *s == '\'' )
        {
            if ( !inDQuote ) inSQuote = !inSQuote;
        }
        if ( *s == '"' )
        {
            if ( !inSQuote ) inDQuote = !inDQuote;
        }
        if ( *s == '\n' )
        {
            putc('\n', output);
            s++;
            while (*s == '\n') {
               putc('\n', output);
               s++;
            }
            PastWhiteSpace( s );
            if ( *s == '}' )
            {
                --tabs;
                indent(output);
                putc( *s++, output );
                continue;
            }
            if ( *s == '\0' ) return;
            if ( *s != '#' )  /* #define, #endif etc.. start at col 1 */
            {
                indent(output);
            }
        }
        if ( *s == '}' && !(inSQuote || inDQuote) )
        {
            --tabs;            /* Indent one fewer */
        }
        if ( *s == '{' && !(inSQuote || inDQuote) )
        {
            tabs++;            /* Indent one more */
        }
        putc( *s, output );
        s++;
    }
    if ( final_newline ) putc('\n', output);
}

static char *find_predicate( AST *t )
{
  if ( t==NULL ) return NULL;
  if ( t->token == Action )
  {
    if ( t->right!=NULL && t->right->token==PRED_OP && t->right->down->token!=BLOCK )
    {
      return t->right->down->action;
    }
  }
  else if ( t->token == PRED_OP && t->down->token!=BLOCK )
  {
    return t->down->action;
  }
  return NULL;
}

/**
 * Given a pointer to a tree pattern element (Token, BLOCK, etc...),
 * generate an expression that predicts when that path would match.
 */
static set gen_prediction_expr( AST *alt, GLA *t )
{
  char *sempred;
  set a, rs;
  int tok, first=1, num_on_line=0;
  require(alt==NULL||alt->token==ALT,
      "gen_prediction_expr: AST node is invalid");

  a = Lookahead(t);
  rs = set_dup(a);

  /* if this production is a guess block, then test return value of setjmp() */

    /* MR21 Problem reported by Peter Keller
       THM: Sor doesn't see predicate because of intervening action.

    */

    if (alt != NULL) {
        AST* cursor;
        for (cursor = alt->down;
             cursor != NULL && Action == cursor->token;
             cursor = cursor->right) {
      /* nothing */
        }
        if (cursor != NULL &&
            cursor->token == PRED_OP &&
            cursor->down->token == BLOCK) {
            fprintf(output," !_gv &&");
        }
    }

  /* handle end of input first */
  if ( set_el(end_of_input, a) )
  {
    fprintf(output, " _t==NULL");
    set_rm(end_of_input, a);
    if ( !set_nil(a) ) fprintf(output, " ||");
  }
  else if ( set_el(wild_card, a) )
  {
    fprintf(output, " _t!=NULL");
    if ( alt!=NULL ) {
      sempred = find_predicate(alt->down);
      if ( sempred!=NULL ) fprintf(output, " && (%s)", sempred);
    }
    set_free(a);
    return rs;
  }
  else
  {
    fprintf(output, " _t!=NULL");
    if ( !set_nil(a) ) fprintf(output, " &&");
  }

  if ( set_nil(a) ) { set_free(a); return rs; }

  fprintf(output, " (");
  while ( !set_nil(a) )
  {
    tok = set_int(a);
    set_rm(tok, a);
    if ( first ) first = 0;
    else fprintf(output, "||");
    if ( num_on_line>=2 ) {num_on_line=0; fprintf(output,"\n"); indent(output);}
    else num_on_line++;
    if ( tok==wild_card ) fprintf(output, "_t!=NULL");
    else if ( token_dict[tok]==0 )
    {
      if ( GenCPP )
        fprintf(output, "_t->type()==UNDEFINED_TOKEN");
      else
        fprintf(output, "_t->token==UNDEFINED_TOKEN");
    }
    else
    {
      if ( GenCPP )
        fprintf(output, "_t->type()==%s", token_dict[tok]);
      else
        fprintf(output, "_t->token==%s", token_dict[tok]);
    }
  }
  fprintf(output, ")");

  if ( alt!=NULL ) {
    sempred = find_predicate(alt->down);
    if ( sempred!=NULL ) fprintf(output, " && (%s)", sempred);
  }
  set_free(a);
  return rs;
}

/**
 * Find all return types/parameters that require structs and def
 * all rules with ret types.
 */
void GenRulePrototypes(FILE *f, int tabs)
{
  AST *p;
  SymEntry *s;

  for (p=rules; p!=NULL; p=p->right)
  {
    int i;
    s = (SymEntry *) hash_get(symbols, p->text);
    require(s!=NULL, "GenRulePrototypes: sym tab broken");

    for (i=1; i<=tabs; i++) fprintf(f, "\t");
    GenRulePrototype(f, p, s, 1);
  }
}

static void GenRulePrototype(FILE *f, AST *p, SymEntry *s, int decl_not_def)
{
  require(s!=NULL&&s->definition!=NULL, "GenRulePrototype: no def for rule");

  if ( GenCPP )
  {
    if ( s->rt != NULL )
      DumpType(s->rt, f, FileStr[s->definition->file], s->definition->line);
    else fprintf(f, "void");
    if ( decl_not_def )
      fprintf(f, " %s(SORASTBase **_root%s",
          p->text,
          transform?", SORASTBase **_result":"");
    else
      fprintf(f, " %s::%s(SORASTBase **_root%s",
          CurClassName,
          p->text,
          transform?", SORASTBase **_result":"");
    if ( s->args != NULL ) fprintf(f, ",%s", s->args);
    fprintf(f, ")");
    if ( decl_not_def ) fprintf(f, ";");
    fprintf(f, "\n");
    return;
  }

  if ( decl_not_def ) fprintf(f, "extern ");

  if ( s->rt != NULL )
    DumpType(s->rt, f, FileStr[s->definition->file], s->definition->line);
  else fprintf(f, "void");
  fprintf(f, " %s%s(STreeParser *_parser, SORAST **_root%s",
      Prefix,
      p->text,
      transform?", SORAST **_result":"");
  if ( s->args != NULL ) fprintf(f, ",%s", s->args);
  fprintf(f, ")");
  if ( decl_not_def ) fprintf(f, ";");
  fprintf(f, "\n");
}

/**
 * For each referenced token, generate a #define in a file defined in
 * 'def_token_file'.
 */
void gen_tokens_file(void)
{
  FILE *f;
  ListNode *p;

  if ( def_token_file==NULL ) return;

  f = fopen(def_token_file, "w");
  if ( f==NULL )
  {
    errNoFL(eMsg1("cannot write token definition file %s", def_token_file));
    return;
  }
#ifdef SPECIAL_FOPEN
       special_fopen_actions(def_token_file);
#endif

  gen_info_hdr(f);
  fprintf(f, "\n");
  for (p = token_list->next; p!=NULL; p=p->next)
  {
    SymEntry *t = (SymEntry *) p->elem;
    fprintf(f, "#define %s %d\n", t->str, t->token_type);
  }
  fclose(f);
}

static AST *first_guess_block(AST *q)
{
  require(q!=NULL&&q->token==ALT, "first_guess_block: non-ALT ptr");

  if ( q->down!=NULL )
  {
    if ( (q->down->token==PRED_OP && q->down->down!=NULL &&
        q->down->down->token==BLOCK) )
    {
      return q->down;
    }
    else if (q->down->token==Action && q->down->right!=NULL &&
        q->down->right->token==PRED_OP && q->down->right->down!=NULL &&
        q->down->right->down->token==BLOCK)
    {
      return q->down->right;
    }
    else return NULL;
  }
  return NULL;
}

static void code_for_guess_block( AST *t, int *guess_block_in_prev_alt )
{
  if ( found_guess_block )
  {
    /* if previous alt had a guess block, check to turn it off */
    if ( *guess_block_in_prev_alt )
    {
      indent(output);
      if ( GenCPP ) fprintf(output, "if ( guessing ) _GUESS_DONE;\n");
      else fprintf(output, "if ( _parser->guessing ) _GUESS_DONE;\n");
    }
    /* if current alt has a guess block... */
    if ( first_guess_block(t)!=NULL )
    {
      indent(output);
      fprintf(output, "_GUESS;\n");
      *guess_block_in_prev_alt = 1;
    }
  }
}
