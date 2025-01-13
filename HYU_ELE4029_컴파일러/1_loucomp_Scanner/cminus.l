/****************************************************/
/* File: cminus.l                                   */
/* Lex specification for C-MINUS                    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"while"		{return WHILE;}
"return"	{return RETURN;}
"int"		{return INT;}
"void"		{return VOID;}
"="             {return ASSIGN;}
"=="            {return EQ;}
"!="		{return NE;}
"<"             {return LT;}
"<="            {return LE;}
">"             {return GT;}
">="            {return GE;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LBRACE;}
"]"             {return RBRACE;}
"{"             {return LCURLY;}
"}"             {return RCURLY;}
";"             {return SEMI;}
","             {return COMMA;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"            {
                  int c;
                  int comment_done = 0;
                  while (!comment_done) {
                    c = input();  // 주석 안의 문자 하나씩 읽기
                    if (c == EOF)
                      break;
                    if (c == '\n') {
                      lineno++;  // 새로운 줄일 때 라인 번호 증가
                    }
                    if (c == '*') {  // 주석 끝을 찾기 위해 '*'를 체크
                      c = input();
                      if (c == '/') {
                        comment_done = 1;  // 주석 종료
                      }
                    }
                  }
                }

.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

