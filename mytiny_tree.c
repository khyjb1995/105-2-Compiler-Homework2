#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mytiny.h"
#include "mytiny_tree.h"

char c_types1[3][6] = {"int", "float", "char*"};
char c_types2[3][5] = {"int", "real", "str"};

tEXP* create_exp ( ) {
  tEXP* tmp;
  tmp = (struct t_exp *)malloc(sizeof(struct t_exp));
  if( tmp ) {
    tmp->exp_id = eMIN;
    tmp->name[0] = '\0';
    tmp->ival = 0;
    tmp->rval = 0.0;
    tmp->qstr[0] = '\0';
    tmp->exp1 = NULL;
    tmp->next = NULL;
  }
  return tmp;
}

tSTM* create_stm ( ) {
  tSTM* tmp;
  tmp = (struct t_stm *)malloc(sizeof(struct t_stm));
  if( tmp ) {
    tmp->stm_id = sMIN;
    tmp->exp1 = NULL;
    tmp->exp2 = NULL;
    tmp->stm1 = NULL;
    tmp->stm2 = NULL;
    tmp->next = NULL;
  }
  return tmp;
}

symNODE* create_symnode ( char* s, int t ) {
  symNODE* tmp;
  tmp = (struct sym_node *)malloc(sizeof(struct sym_node));
  if( tmp ) {
    strcpy( tmp->name, s );
    tmp->type = t;
    tmp->next = symtab;
  }
  return tmp;
}

void free_exp ( tEXP* p ) {
  if( p ) {
    if( p->exp1 ) free_exp( p->exp1 );
    if( p->next ) free_exp( p->next );
    free( p );
  }
}

void free_stm ( tSTM* p ) {
  if( p ) {
    if( p->exp1 ) free_exp( p->exp1 );
    if( p->exp2 ) free_exp( p->exp2 );
    if( p->stm1 ) free_stm( p->stm1 );
    if( p->stm2 ) free_stm( p->stm2 );
    if( p->next ) free_stm( p->next );
    free( p );
  }
}

void free_symnode ( symNODE* p ) {
  if( p ) {
    if( p->next ) free_symnode( p->next );
    free( p );
  }
}

void gen_exp ( tEXP* p ) {
  tEXP* te;
  if( p ) {
    switch( p->exp_id ) {
    case eTYPE: fprintf( yyout, "%s", c_types1[ p->ival - 1] );
		break;
    case eFORM: gen_exp( p->exp1 );
		fprintf( yyout, " %s", p->name );
		if (p->next) {
		    fprintf( yyout, ", " );
		    gen_exp( p->next );
		}
		break;
    case eEXPR: gen_exp( p->exp1 );
		gen_exp( p->next );
		break;
    case eADD:  fprintf( yyout, " + " );
		gen_exp( p->exp1 );
		gen_exp( p->next );
		break;
    case eMINUS: fprintf( yyout, " - " );
		gen_exp( p->exp1 );
    gen_exp( p->next );
		break;
    case eMEXP:
      gen_exp(p->exp1);
      gen_exp(p->next);
		break;
    case eTIMES:
      fprintf(yyout, " * ");
      gen_exp(p->exp1);
      gen_exp(p->next);
		break;
    case eDIVIDE:
      fprintf(yyout, " / ");
      gen_exp(p->exp1);
      gen_exp(p->next);
		break;
    case eINUM: fprintf( yyout, "%d", p->ival );
		break;
    case eRNUM: fprintf( yyout, "%f", p->rval );
		break;
    case eID:   fprintf( yyout, "%s", p->name );
		break;
    case ePAREN:
      fprintf(yyout, "(");
      gen_exp(p->exp1);
      fprintf(yyout, ")");
		break;
    case eFUNC:
      fprintf(yyout, "%s(", p->name);
      gen_exp(p->exp1);
      fprintf(yyout, ")");
		break;
    case eEQ:
      gen_exp(p->exp1);
      fprintf(yyout, " == ");
      gen_exp(p->next);
		break;
    case eNE:
      gen_exp(p->exp1);
      fprintf(yyout, " != ");
      gen_exp(p->next);
		break;
    case eGT:
      gen_exp(p->exp1);
      fprintf(yyout, " > ");
      gen_exp(p->next);
		break;
    case eLT:
      gen_exp(p->exp1);
      fprintf(yyout, " < ");
      gen_exp(p->next);
		break;
    case eGE:
      gen_exp(p->exp1);
      fprintf(yyout, " >= ");
      gen_exp(p->next);
		break;
    case eLE:
      gen_exp(p->exp1);
      fprintf(yyout, " <= ");
      gen_exp(p->next);
		break;
    case eAPARM:
      gen_exp(p->exp1);
      if(p->next)
      {
        fprintf(yyout, ", ");
      }
      gen_exp(p->next);
		break;
    case eASSIGN1: fprintf( yyout, "%s = ", p->name );
		gen_exp( p->exp1 );
		fprintf( yyout, ";\n" );
		break;
    case eASSIGN2: fprintf( yyout, "%s = %s;\n", p->name, p->qstr );
		break;
    case eWSTM:
      fprintf(yyout, "printf(%s);\n", p->qstr);
      fprintf(yyout, "printf(\"%%d\", ");
      gen_exp(p->exp1);
      fprintf(yyout, ");");
		break;
    case eDSTM:
      fprintf(yyout, "printf(%s);\n", p->qstr);
      fprintf(yyout, "scanf(\"%%d\", &%s);\n", p->name);
		break;
    default: fprintf(stderr, "******* An error in expressions!\n");
	     break;
    }
  }
}

void gen_code ( tSTM* p ) {
  tEXP *te;
  tSTM *ts;
  int t;

  if( p ) {
    switch( p->stm_id ) {
    case sMAIN: gen_exp( p->exp1->exp1 );
		fprintf( yyout, " main(" );
		gen_exp( p->exp2 );
		fprintf( yyout, ")\n" );
		gen_code( p->stm1 );
		break;
    case sMDCL: gen_exp( p->exp1->exp1 );
		fprintf( yyout, " " );
		gen_exp( p->exp1 );
		fprintf( yyout, "(" );
		gen_exp( p->exp2 );
		fprintf( yyout, ")\n" );
		gen_code( p->stm1 );
		break;
    case sBLOCK: fprintf( yyout, "{ \n");
		gen_code( p->stm1 );
		fprintf( yyout, "\n}\n");
		break;
    case sVDCL1: gen_exp( p->exp1 );
		fprintf( yyout, " %s;\n", p->exp1->name );
		break;
    case sVDCL2: gen_exp( p->exp1 );
		fprintf( yyout, " " );
		gen_code( p->stm1 );
		break;
    case sASTM: gen_exp( p->exp1 );
		break;
    case sRSTM:
      fprintf(yyout, "return ");
      gen_exp(p->exp1);
      fprintf(yyout, ";");
		break;
    case sISTM:
      fprintf(yyout, "if(");
      gen_exp(p->exp1);
      fprintf(yyout, ")\n{\n");
      gen_code(p->stm1);
      fprintf(yyout, "}\n");
      if(p->stm2)
      {
        fprintf(yyout, "else\n{\n");
        gen_code(p->stm2);
        fprintf(yyout, "}\n");
      }
		break;
    case sWSTM:
      gen_exp(p->exp1);
		break;
    case sDSTM:
      gen_exp(p->exp1);
		break;
    default:    fprintf(stderr, "******* An error in statements!\n");
		break;
    }
    if (p->next) gen_code( p->next );
  }
}

int lookup( symNODE* p, char* s ) {
  int tmp = 0;
  if (p) {
    if (strcmp( p->name, s ) == 0) {
      tmp = p->type;
    } else {
      tmp = lookup( p->next, s );
    }
  }
  return tmp;
}

int ana_exptype( tEXP *p ) {
  // Will get bonus if you implement an ana_exptype function.
  int tmp = tINT;
  return tmp;
}

void init_all () {
    int i, j;
    // initialize gen_rw
    for (i=0; i<2; i++)
	for (j=0; j<3; j++) gen_rw[i][j] = 0;
    printf("mytiny2c: init_all() done!\n");
}

void gen_rwcode () {
    int i, j;

// Combine init_code
  fprintf( yyout, "#include <stdio.h>\n");
  fprintf( yyout, "#include <stdlib.h>\n");
  fprintf( yyout, "#include <string.h>\n");
}
