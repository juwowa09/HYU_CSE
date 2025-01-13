/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void); // added 11/2/11 to ensure no conflict with lex
static int yyerror(char * message);

%}
%nonassoc IFX
%nonassoc ELSE

%token IF ELSE WHILE RETURN INT VOID
%token ID NUM

%left COMMA
%right ASSIGN
%nonassoc LT LE GT GE EQ NE
%left PLUS MINUS 
%left TIMES OVER 
%left LPAREN RPAREN LBRACE RBRACE LCURLY RCURLY SEMI 
%token ERROR ENDFILE

%% /* Grammar for TINY */

program     : declare_list
                 { savedTree = $1;} 
            ;

declare_list : declare_list  declare {
                  YYSTYPE t = $1;
                  if(t!=NULL){
                    while(t->sibling != NULL)
                      t=t->sibling;
                    t->sibling = $2;
                    $$ = $1;
                  }
                  else{
                    $$ = $2;
                  }
                }
             | declare {
                $$ = $1;
             }
             ;

declare    : var_declare{$$ = $1;} | func_declare{$$ = $1;};
var_declare  : type_spec id SEMI {
                  $$ = newDeclareNode(NonArrayDK);
                  $$->type = $1->type;
                  $$->attr.name =savedName;
                  $$->lineno = savedLineNo;
              }
              | type_spec id{
                  $$ = newDeclareNode(ArrayDK);
                  $$->type = $1->type;
                  $$->attr.name =savedName;
                  $$->lineno = savedLineNo;
              } LBRACE num RBRACE SEMI{
                  $$=$3;
                  
                  $$->child[0] = $5;

              }
              ;
id              : ID {  
                   savedName = copyString(tokenString);
                   savedLineNo = lineno; }
num             :NUM {  
                 $$ = newExpNode(ConstK);
                 $$->lineno = lineno;
                 $$->attr.val = atoi(tokenString); }
type_spec    : INT{
                  $$=newExpNode(IdK);
                  $$->type = Integer;
                  $$->lineno = lineno;
              }
              | VOID{$$=newExpNode(IdK);
                  $$->type = Void;
                  $$->lineno = lineno;};
func_declare   : type_spec id {
                    $$ = newDeclareNode(FuncDK);
                    $$->type = $1->type;
                    $$->attr.name = savedName;
                    $$->lineno = savedLineNo;

                  }LPAREN params RPAREN compound_stmt{
                    $$ = $3;
                    $$->child[0] = $5;
                    $$->child[1] = $7;

              }
              ;
params        : param_list{$$=$1;}
                |  
                  VOID{$$=newDeclareNode(NonParamDK);
                  $$->lineno = lineno;
                  $$->type = Void;};
param_list    : param_list COMMA param{
                  YYSTYPE t = $1;
                  if(t != NULL){
                  while(t->sibling != NULL)
                    t=t->sibling;
                  t->sibling = $3;
                  $$ = $1;}
                  else $$ = $3;
                  
              } 
              | param {$$=$1;};
param         : type_spec id {
                  $$ = newDeclareNode(ParamDK);
                  $$->type = $1->type;
                  $$->attr.name = savedName;
                  $$->lineno = savedLineNo;
              }
              | type_spec id LBRACE RBRACE{
                  $$ = newDeclareNode(ParamDK);
                  $$->type = $1->type;
                  $$->attr.name = savedName;
                  $$->lineno = savedLineNo;
              }
              ;
compound_stmt : LCURLY local_declare stmt_list RCURLY{
                $$=newStmtNode(CompK);
                $$->child[0] = $2;
                $$->child[1] = $3;
                $$->lineno = lineno;
              };
local_declare : local_declare var_declare {
                 YYSTYPE t = $1;
                  if(t!=NULL){
                    while(t->sibling != NULL)
                      t=t->sibling;
                    t->sibling = $2;
                    $$ = $1;
                  }
                  else $$ = $2;
              
              }
              | {$$=NULL;};

stmt_list    : stmt_list stmt
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; 
                     }
                     else $$ = $2;
                 }
             |  { $$=NULL ;}
             ;
stmt         : exp_stmt { $$ = $1; }
             | compound_stmt { $$ = $1; }
             | if_stmt{ $$ = $1; }
             | if_else_stmt{ $$=$1; }
             | iteration_stmt { $$ = $1; }
             | val_ret_stmt { $$ = $1; }
             | non_val_ret_stmt {$$ = $1;}
             ;

exp_stmt     : exp SEMI{$$=$1;} | SEMI{$$=NULL;};
if_stmt      : IF LPAREN exp RPAREN stmt %prec IFX{
                  $$ = newStmtNode(IfK);
                  $$->lineno = lineno;
                  $$->child[0] = $3;
                  $$->child[1] = $5;
              };

if_else_stmt : IF LPAREN exp RPAREN stmt ELSE stmt{
                  $$ = newStmtNode(IfElseK);
                  $$->lineno = lineno;
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                  $$->child[2] = $7;
              };

iteration_stmt : WHILE LPAREN exp RPAREN stmt{
                  $$ = newStmtNode(IterK);
                  $$->lineno = lineno;
                  $$->child[0] =$3;
                  $$->child[1] =$5;
              };

non_val_ret_stmt :RETURN SEMI{
                  $$=newStmtNode(NonValRetK);
                  $$->lineno = lineno;
              };
               
val_ret_stmt   : RETURN exp SEMI{
                  $$=newStmtNode(ValRetK);
                  $$->lineno = lineno;
                  $$->child[0] = $2;
              } ;

exp          : var ASSIGN exp{
                $$=newExpNode(AssignK);
                $$->lineno = lineno;
                $$->child[0] = $1;
                $$->child[1] = $3;

              } 
              | simple_exp{
                  $$=$1;
              };
                 
simple_exp   : additive_exp relop additive_exp 
                 { $$ = $2;
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                 }
             | 
              additive_exp { $$ = $1; }
             ;

var          : id { 
                   $$ = newExpNode(IdK);
                   $$->lineno = savedLineNo;
                   $$->attr.name =savedName;
                 }
             | id {
                   $$ = newExpNode(IdK);
                   $$->lineno = savedLineNo;
                   $$->attr.name =savedName;
              }LBRACE exp RBRACE {
                  $$=$2;
                  $$->child[0] = $4;
             };
relop        : GE {$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=GE;} 
              | GT{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=GT;}  
              | LT{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=LT;}  
              | LE{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=LE;}  
              | EQ{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=EQ;}  
              | NE{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=NE;} ;
additive_exp : additive_exp addop term{
                   $$ = $2;
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                } 
                | term{
                  $$=$1;
                };
addop : PLUS{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=PLUS;} 
        | MINUS {$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=MINUS;};


term         : term mulop factor 
                  { 
                   $$ = $2;
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                 }
      
             | factor { $$ = $1; }
             ;

mulop        : TIMES{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=TIMES;} 
              | OVER{$$=newExpNode(BinK);
                  $$->lineno = lineno;
                  $$->attr.op=OVER;};
call         :  id{
                $$=newExpNode(CallK);
                $$->lineno = savedLineNo;
                $$->attr.name = savedName;

                } LPAREN args RPAREN{
                $$ =$2;
                $$->child[0] = $4;
              };
args         :  arg_list{$$=$1;} | {$$=NULL;};
arg_list     : arg_list COMMA exp{
                  YYSTYPE t = $1;
                  if(t!=NULL){
                  while (t->sibling != NULL)
                        t = t->sibling;
                  t->sibling = $3;
                  $$ = $1; 
                  }
                  else $$=$3;
                  
             } | exp {$$=$1;};

factor       : LPAREN exp RPAREN { $$ = $2; }
             | var{$$=$1; }| call { $$=$1;}
             | num
             | ERROR { $$ = NULL; }
            ;

%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

