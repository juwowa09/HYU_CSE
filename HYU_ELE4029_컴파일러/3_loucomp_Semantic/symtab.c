/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4
#define MAX_SCOPE 1000

/* the hash function */
static int hash(char *key)
{
  int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  {
    temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

ScopeList scopes[MAX_SCOPE], scopeStack[MAX_SCOPE];
int location[MAX_SCOPE];
int scopeCnt = 0, stackCnt = 0;

void sc_push(ScopeList scope)
{
  location[stackCnt] = 0;
  scopeStack[stackCnt++] = scope;
}

void sc_pop(void)
{
  --stackCnt;
}

ScopeList sc_top(void)
{
  if (stackCnt == 0)
    return NULL; // 스택이 비어있을 경우 NULL 반환
  return scopeStack[stackCnt - 1];
}

ScopeList sc_create(char *scopeName)
{
  ScopeList newScope;

  newScope = (ScopeList)malloc(sizeof(struct ScopeListRec));
  newScope->scopeName = scopeName;
  newScope->nestedLevel = stackCnt;
  newScope->parent = sc_top();
  scopes[scopeCnt++] = newScope;

  return newScope;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char *name, int lineno, int loc, TreeNode *treeNode)
{
  int h = hash(name);
  ScopeList topScope = sc_top();
  BucketList l = topScope->bucket[h];
  while ((l != NULL) && (strcmp(name, l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  {
    l = (BucketList)malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->treeNode = treeNode;
    l->lines = (LineList)malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = topScope->bucket[h];
    topScope->bucket[h] = l;
  }
  else /* found in table, so just add line number */
  {
    //    LineList t = l->lines;
    //    while (t->next != NULL) t = t->next;
    //    t->next = (LineList) malloc(sizeof(struct LineListRec));
    //    t->next->lineno = lineno;
    //    t->next->next = NULL;
  }
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char *name)
{
  BucketList l = st_lookup_bucket(name);
  if (l != NULL)
    return l->memloc;

  return -1;
}

BucketList st_lookup_bucket(char *name)
{
  int h = hash(name);
  ScopeList now = sc_top();

  while (now)
  {
    BucketList l = now->bucket[h];

    while ((l != NULL) && (strcmp(l->name, name) != 0))
      l = l->next;

    if (l)
      return l;
    now = now->parent;
  }
  return NULL;
}

int st_lookup_now(char *name)
{
  int h = hash(name);
  ScopeList now = sc_top();

  if (now)
  {
    BucketList l = now->bucket[h];

    while ((l != NULL) && (strcmp(l->name, name) != 0))
      l = l->next;
    if (l)
      return 1;
  }
  return 0;
}

void st_print_lineno(char *name)
{
  BucketList b = st_lookup_bucket(name);
  LineList l = b->lines;

  while (l->next)
  {
    fprintf(listing, " %d", l->lineno);
    l = l->next;
  }
}

void st_add_lineno(char *name, int lineno)
{
  BucketList b = st_lookup_bucket(name);
  LineList l = b->lines;

  while (l->next)
    l = l->next;

  l->next = (LineList)malloc(sizeof(struct LineListRec));
  l->next->lineno = lineno;
  l->next->next = NULL;
}

int getLocation(void)
{
  return location[stackCnt - 1]++;
}

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */

void printFuncTab(FILE *listing)
{
  int i;
  fprintf(listing, "< Functions >\n");
  fprintf(listing, "Function Name   Return Type   Parameter Name  Parameter Type\n");
  fprintf(listing, "-------------  -------------  --------------  --------------\n");
  for (i = 0; i < scopeCnt; ++i)
  {
    ScopeList scope = scopes[i];
    BucketList *hashTable = scope->bucket;

    for (int j = 0; j < SIZE; ++j)
    {
      if (hashTable[j] != NULL)
      {
        BucketList b = hashTable[j];
        TreeNode *tree = b->treeNode;

        while (b != NULL)
        {
          if (tree->nodekind == DeclareK)
          {
            if (tree->kind.dc == FuncDK)
            {
              fprintf(listing, "%-15s", b->name);
              if (tree->type == Void)
              {
                fprintf(listing, "%-15s", "void");
              }
              else if (tree->type == Integer)
              {
                fprintf(listing, "%-15s", "int");
              }

              int noParam = TRUE;

              for (int k = 0; k < scopeCnt; ++k)
              {
                ScopeList paramScope = scopes[k];
                if (strcmp(paramScope->scopeName, b->name) != 0)
                  continue;
                BucketList *paramHashTable = paramScope->bucket;

                for (int l = 0; l < SIZE; ++l)
                {
                  if (paramHashTable[l])
                  {
                    BucketList paramB = paramHashTable[l];
                    TreeNode *pnode = paramB->treeNode;

                    while (paramB != NULL)
                    {
                      if (pnode->kind.dc == ParamDK)
                      {
                        noParam = FALSE;
                        fprintf(listing, "\n%-15s%-15s%-16s", "-", "-", paramB->name);
                        if (pnode->type == Integer)
                          fprintf(listing, "%-15s", "int");
                        else if (pnode->type == Void)
                          fprintf(listing, "%-15s", "void");
                        else if (pnode->type == IntArray)
                          fprintf(listing, "%-15s", "int[]");
                      }
                      paramB = paramB->next;
                    }
                  }
                }
              }
              if (noParam)
              {
                //                                if (strcmp(b->name, "output") != 0) {
                //                                    fprintf(listing, "%-16s", "");
                //                                    fprintf(listing, "%-14s", "void");
                //                                } else {
                //                                    fprintf(listing, "\n%-15s%-13s%-16s", "-", "-", "value");
                //                                    fprintf(listing, "%-14s", "int");
                //                                }
                fprintf(listing, "%-16s", "");
                fprintf(listing, "%-14s", "void");
              }

              fprintf(listing, "\n");
            }
          }
          b = b->next;
        }
      }
    }
  }
}

void printGlobalSym(FILE *listing)
{
  fprintf(listing, "< Global Symbols >\n");
  fprintf(listing, " Symbol Name   Symbol Kind   Symbol Type\n");
  fprintf(listing, "-------------  -----------  -------------\n");

  int i;
  for (i = 0; i < scopeCnt; ++i)
  {
    ScopeList scope = scopes[i];
    if (strcmp(scope->scopeName, "global") != 0)
      continue;

    BucketList *hashTable = scope->bucket;

    for (int j = 0; j < SIZE; ++j)
    {
      if (hashTable[j] != NULL)
      {
        BucketList b = hashTable[j];
        TreeNode *tree = b->treeNode;

        while (b != NULL)
        {
          LineList l = b->lines;
          fprintf(listing, "%-15s", b->name);

          if (tree->nodekind == DeclareK)
          {
            switch (tree->kind.dc)
            {
            case NonArrayDK:
              fprintf(listing, "%-13s", "Variable");
              if (tree->type == Void)
              {
                fprintf(listing, "%-12s", "void");
              }
              else if (tree->type == Integer)
              {
                fprintf(listing, "%-12s", "int");
              }
              break;
            case ArrayDK:
              fprintf(listing, "%-13s", "Variable");
              if (tree->type == Integer)
              {
                fprintf(listing, "%-11s", "int");
              }
              break;
            case FuncDK:
              fprintf(listing, "%-13s", "Function");
              if (tree->type == Void)
              {
                fprintf(listing, "%-11s", "void");
              }
              else if (tree->type == Integer)
              {
                fprintf(listing, "%-11s", "int");
              }
              break;
            default:
              break;
            }
          }
          fprintf(listing, "\n");
          b = b->next;
        }
      }
    }
  }
}

void printScope(FILE *listing)
{
  int i;
  fprintf(listing, "< Scopes >\n");
  fprintf(listing, " Scope Name   Nested Level   Symbol Name   Symbol Type\n");
  fprintf(listing, "------------  ------------  -------------  -----------\n");

  for (i = 0; i < scopeCnt; ++i)
  {
    ScopeList scope = scopes[i];
    if (!strcmp(scope->scopeName, "global"))
      continue;

    BucketList *hashTable = scope->bucket;
    int isNewLine = FALSE;

    for (int j = 0; j < SIZE; ++j)
    {
      if (hashTable[j] != NULL)
      {
        BucketList b = hashTable[j];
        TreeNode *node = b->treeNode;

        while (b != NULL)
        {
          if (node->nodekind == DeclareK)
          {
            if (node->kind.dc == ParamDK || node->kind.dc == NonArrayDK || node->kind.dc == ArrayDK)
            {
              isNewLine = TRUE;
              fprintf(listing, "%-14s", scope->scopeName);
              fprintf(listing, "%-14d", scope->nestedLevel);
              fprintf(listing, "%-15s", node->attr.name);
            }
            if (node->type == Void)
              fprintf(listing, "%-13s", "void");
            else
            {
              if (node->child[0])
                fprintf(listing, "%-13s", "int[]");
              else
                fprintf(listing, "%-13s", "int");
            }
          }
          fprintf(listing, "\n");
          b = b->next;
        }
      }
    }
    if (isNewLine)
      fprintf(listing, "\n");
  }
}

void printST(FILE *listing)
{
  int i;
  fprintf(listing, "< Symbol Table >\n");
  fprintf(listing, " Symbol Name   Symbol Kind   Symbol Type    Scope Name   Location  Line Numbers\n");
  fprintf(listing, "-------------  -----------  -------------  ------------  --------  ------------\n");
  for (i = 0; i < scopeCnt; ++i)
  {
    ScopeList scope = scopes[i];
    BucketList *hashTable = scope->bucket;

    for (int j = 0; j < SIZE; ++j)
    {
      if (hashTable[j] != NULL)
      {
        BucketList b = hashTable[j];
        TreeNode *tree = b->treeNode;

        while (b != NULL)
        {
          LineList l = b->lines;
          fprintf(listing, "%-15s", b->name);

          if (tree->nodekind == DeclareK)
          {
            switch (tree->kind.dc)
            {
            case NonArrayDK:
              fprintf(listing, "%-13s", "Variable");
              if (tree->type == Void)
              {
                fprintf(listing, "%-15s", "void");
              }
              else if (tree->type == Integer)
              {
                fprintf(listing, "%-15s", "int");
              }
              break;
            case ArrayDK:
	      fprintf(listing, "%-13s", "Variable");
              if (tree->type == IntArray)
              {
                fprintf(listing, "%-15s", "int[]");
              }
              break;
            case FuncDK:
              fprintf(listing, "%-13s", "Function");
              if (tree->type == Void)
              {
                fprintf(listing, "%-15s", "void");
              }
              else if (tree->type == Integer)
              {
                fprintf(listing, "%-15s", "int");
              }
              break;
            case ParamDK:
              fprintf(listing, "%-13s", "Variable");
              if (tree->type == Void)
              {
                fprintf(listing, "%-15s", "void");
              }
              else if (tree->type == Integer)
              {
                fprintf(listing, "%-15s", "int");
              }
              else if (tree->type == IntArray)
              {
                fprintf(listing, "%-15s", "int[]");
              }
              break;
            default:
              break;
            }
          }

          fprintf(listing, "%-14s", scope->scopeName);
          fprintf(listing, "%-8d", b->memloc);

          while (l != NULL)
          {
            fprintf(listing, "%5d", l->lineno);
            l = l->next;
          }
          fprintf(listing, " \n");
          b = b->next;
        }
      }
    }
  }
} /* printSymTab */

void printSymTab(FILE *listing)
{
  fprintf(listing, "\n\n");
  printST(listing);
  fprintf(listing, "\n\n");
  printFuncTab(listing);
  fprintf(listing, "\n\n");
  printGlobalSym(listing);
  fprintf(listing, "\n\n");
  printScope(listing);
}
