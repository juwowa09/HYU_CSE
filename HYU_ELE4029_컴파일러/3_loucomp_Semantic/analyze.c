/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

/* counter for variable memory locations */
static int location = 0;

static ScopeList globalScope = NULL;
static char *funcName;
static int predScope = FALSE;
int noReturn = FALSE;

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
static void traverse(TreeNode *t,
                     void (*preProc)(TreeNode *),
                     void (*postProc)(TreeNode *))
{
  if (t != NULL)
  {
    preProc(t);
    {
      int i;
      for (i = 0; i < MAXCHILDREN; i++)
        traverse(t->child[i], preProc, postProc);
    }
    postProc(t);
    traverse(t->sibling, preProc, postProc);
  }
}

static pushBuiltInFunc(void)
{
  TreeNode *func;
  TreeNode *param;
  TreeNode *compStmt;
  TreeNode *typeSpec;

  typeSpec = newExpNode(IdK);
  typeSpec->type = Integer;

  compStmt = newStmtNode(CompK);
  compStmt->child[0] = NULL;
  compStmt->child[1] = NULL;

  // input function
  func = newDeclareNode(FuncDK);
  func->type = Integer;
  func->lineno = 0;
  func->attr.name = "input";

  param = newDeclareNode(ParamDK);
  param->type = Void;

  func->child[0] = param;
  func->child[1] = compStmt;

  st_insert("input", 0, getLocation(), func);

  // output function
  func = newDeclareNode(FuncDK);
  func->type = Void;
  func->lineno = 0;
  func->attr.name = "output";

  param = newDeclareNode(ParamDK);
  param->attr.name = "value";
  param->type = Integer;

  func->child[0] = param;
  func->child[1] = compStmt;

  st_insert("output", 0, getLocation(), func);
  ScopeList scope = sc_create("output");
  sc_push(scope);
  st_insert("value", 0, getLocation(), param);
  sc_pop();
}

/* nullProc is a do-nothing procedure to
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode *t)
{
  if (t == NULL)
    return;
  else
    return;
}

/* Procedure insertNode inserts
 * identifiers stored in t into
 * the symbol table
 */
static void insertNode(TreeNode *t)
{
  switch (t->nodekind)
  {
  case StmtK:
    switch (t->kind.stmt)
    {
    case CompK:
      if (predScope)
        predScope = FALSE;
      else
      {
        ScopeList scope = sc_create(funcName);
        sc_push(scope);
        location++;
      }
      t->attr.scope = sc_top();
    default:
      break;
    }
    break;
  case ExpK:
    switch (t->kind.exp)
    {
    case IdK:
      if (st_lookup(t->attr.name) == -1)
      {
	fprintf(listing, "Error: undeclared variable \"%s\" is used at line %d\n", t->attr.name, t->lineno);
        Error = TRUE;
      }
      else
        st_add_lineno(t->attr.name, t->lineno);
      break;
    case CallK:
      if (st_lookup(t->attr.name) == -1)
      {
        fprintf(listing, "Error: undeclared function \"%s\" is called at line %d\n", t->attr.name, t->lineno);
        Error = TRUE;
      }
      else
        st_add_lineno(t->attr.name, t->lineno);
      break;
    default:
      break;
    }
    break;
  case DeclareK:
    switch (t->kind.dc)
    {
    case FuncDK:
      funcName = t->attr.name;
      if (st_lookup_now(funcName))
      {
        fprintf(listing, "Error: Symbol \"%s\" is redefined at line %d (already defined at line", t->attr.name, t->lineno);
        st_print_lineno(funcName);
        fprintf(listing, ")\n");
        Error = TRUE;
        break;
      }

      st_insert(funcName, t->lineno, getLocation(), t);
      sc_push(sc_create(funcName));
      predScope = TRUE;
      break;
    case NonArrayDK:
    {
      char *name = t->attr.name;
      if (t->type == Void)
      {
        fprintf(listing, "Error: The void-type variable is declared at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
        Error = TRUE;
        break;
      }

      if (t->child[0] != NULL)
        t->type = IntArray;

      if (!st_lookup_now(name))
        st_insert(name, t->lineno, getLocation(), t);
      else
      {
        fprintf(listing, "Error: Symbol \"%s\" is redefined at line %d (already defined at line", t->attr.name, t->lineno);
        st_print_lineno(name);
        fprintf(listing, ")\n");
        Error = TRUE;
      }
    }
    break;
    case ArrayDK:
    {
      char *name = t->attr.name;
      if (t->type == Void)
      {
        fprintf(listing, "Error: The void-type variable is declared at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
        Error = TRUE;
        break;
      }

      if (t->child[0] != NULL)
        t->type = IntArray;

      if (!st_lookup_now(name))
        st_insert(name, t->lineno, getLocation(), t);
      else
      {
        fprintf(listing, "Error: Symbol \"%s\" is redefined at line %d (already defined at line", t->attr.name, t->lineno);
        st_print_lineno(name);
        fprintf(listing, ")\n");
        Error = TRUE;
      }
    }
    break;
    case ParamDK:
      if (t->attr.name == NULL)
      {
        if (t->type == Void)
          break;
        else
        { /* parameter need name */
        }
      }
      else if (t->type == Void && t->child[0] != NULL)
      {
        fprintf(listing, "Error: The void-type variable is declared at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
        Error = TRUE;
      }
      else if (st_lookup(t->attr.name) == -1)
      {
        st_insert(t->attr.name, t->lineno, getLocation(), t);
        if (t->child[0] != NULL)
          t->type = IntArray;
      }
      else
      {
        fprintf(listing, "Error: Symbol \"%s\" is redefined at line %d (already defined at line", t->attr.name, t->lineno);
        st_print_lineno(t->attr.name);
        fprintf(listing, ")\n");
        Error = TRUE;
      }
      break;
    }
    break;
  default:
    break;
  }
}

static void afterInsertNode(TreeNode *t)
{
  if (t->nodekind == StmtK && t->kind.stmt == CompK)
  {
    sc_pop();
  }
}

static void beforeCheckNode(TreeNode *t)
{
  if (t->nodekind == StmtK)
  {
    if (t->kind.stmt == CompK)
      sc_push(t->attr.scope);
  }
  else if (t->nodekind == DeclareK)
  {
    if (t->kind.dc == FuncDK)
    {
      funcName = t->attr.name;
      noReturn = (t->type == Void);
    }
  }
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *syntaxTree)
{
  globalScope = sc_create("global");
  location = 0;

  sc_push(globalScope);
  pushBuiltInFunc();
  traverse(syntaxTree, insertNode, afterInsertNode);
  sc_pop();

  if (TraceAnalyze)
  {
    printSymTab(listing);
  }
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode *t)
{
  switch (t->nodekind)
  {
  case ExpK:
    switch (t->kind.exp)
    {
    case AssignK:
      if (t->child[0]->type == Void || t->child[1]->type == Void)
      {
	      fprintf(listing,"%s \n",t->child[0]->type);
	fprintf(listing, "asdf\n");
        fprintf(listing, "Error: invalid assignment at line %d\n", t->child[0]->lineno);
        Error = TRUE;
      }
      else if ((t->child[0]->type == IntArray && t->child[0]->child[0] == NULL) || (t->child[1]->type == IntArray && t->child[1]->child[0] == NULL))
      {
	fprintf(listing, "hihi\n");
        fprintf(listing, "Error: invalid assignment at line %d\n", t->child[0]->lineno);
        Error = TRUE;
      }
      else
        t->type = Integer;
      break;
    case BinK:
    {
      TreeNode *lhs = t->child[0];
      TreeNode *rhs = t->child[1];
      ExpType l = lhs->type;
      ExpType r = rhs->type;

      if (l == IntArray && lhs->child[0] != NULL)
      {
        l = Integer;
      }
      if (r == IntArray && rhs->child[0] != NULL)
      {
        r = Integer;
      }

      if (l == Void || r == Void)
      {
        fprintf(listing, "Error: invalid operation at line %d\n", t->lineno);
        Error = TRUE;
      }
      else if (l == IntArray && r == IntArray)
      {
        fprintf(listing, "Error: invalid operation at line %d\n", t->lineno);
        Error = TRUE;
      }
      else if (l != r)
      {
        fprintf(listing, "Error: invalid operation at line %d\n", t->lineno);
        Error = TRUE;
      }
      else
        t->type = Integer;

      break;
    }
    case CallK:
    {
      BucketList b = st_lookup_bucket(t->attr.name);
      if (b == NULL)
        break;

      TreeNode *func = b->treeNode;
      TreeNode *args = t->child[0];
      TreeNode *params = params = func->child[0];

      if (func->kind.dc != FuncDK)
      {
        fprintf(listing, "Error: Invalid function call at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
        Error = TRUE;
        break;
      }

      if (args == NULL)
      {
        if (params != NULL && params->type != Void)
        {
          fprintf(listing, "Error: Invalid function call at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
          Error = TRUE;
          break;
        }
      }

      while (args != NULL)
      {
        ExpType argType = args->type;
        if (argType == IntArray && args->child[0] != NULL)
          argType = Integer;
        if (params == NULL)
        {
          fprintf(listing, "Error: Invalid function call at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
          Error = TRUE;
          break;
        }
        else if (argType != params->type)
        {
          fprintf(listing, "Error: Invalid function call at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
          Error = TRUE;
          break;
        }
        else if (argType == Void)
        {
          fprintf(listing, "Error: Invalid function call at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
          Error = TRUE;
          break;
        }
        else
        {
          args = args->sibling;
          params = params->sibling;
        }
      }
      t->type = func->type;
      break;
    }
    case ConstK:
      t->type = Integer;
      break;
    case IdK:
    {
      BucketList b = st_lookup_bucket(t->attr.name);
      if (b == NULL)
        break;

      TreeNode *var = b->treeNode;
      if (var->nodekind == DeclareK && var->kind.dc == NonArrayDK && t->child[0] != NULL)
      {
        fprintf(listing, "Error: Invalid array indexing at line %d (name : \"%s\"). indexing can only allowed for int[] variables\n", t->lineno, t->attr.name);
        Error = TRUE;
	t->type = var->type;
      }
      else if (t->child[0] != NULL && t->child[0]->type != Integer)
      {
        fprintf(listing, "Error: Invalid array indexing at line %d (name : \"%s\"). indicies should be integer\n", t->lineno, t->attr.name);
        Error = TRUE;
	t->type = var->type;
      }
      else
        t->type = Integer;
    }
    break;
    default:
      break;
    }
    break;
  case StmtK:
    switch (t->kind.stmt)
    {
    case CompK:
      sc_pop();
      break;
    case IfK:
      if (t->child[0]->type != Integer)
      {
        fprintf(listing, "Error: invalid condition at line %d\n", t->lineno);
        Error = TRUE;
      }
      break;
    case IterK:
      if (t->child[0]->type != Integer)
      {
        fprintf(listing, "Error: invalid condition at line %d\n", t->lineno);
        Error = TRUE;
      }
      break;
    case ValRetK:
    {
      TreeNode *func = st_lookup_bucket(funcName)->treeNode;
      if (func->type == Void)
      {
        if (t->child[0] != NULL)
        {
          fprintf(listing, "Error: Invalid return at line %d\n", t->lineno);
          Error = TRUE;
        }
      }
      else if (func->type == Integer)
      {
        if (t->child[0] == NULL || t->child[0]->type == Void)
        {
          fprintf(listing, "Error: Invalid return at line %d\n", t->lineno);
          Error = TRUE;
        }
        else if (t->child[0]->type == IntArray && t->child[0]->child[0] == NULL)
        {
          fprintf(listing, "Error: Invalid return at line %d\n", t->lineno);
          Error = TRUE;
        }
        noReturn = TRUE;
      }
      break;
    }
    default:
      break;
    }
    break;

  case DeclareK:
    switch (t->kind.dc)
    {
    case NonArrayDK:
      if (t->type == Void)
      {
        fprintf(listing, "Error: The void-type variable is declared at line %d (name : \"%s\")\n", t->lineno, t->attr.name);
        Error = TRUE;
      }
      break;
    case FuncDK:
      if (!noReturn)
      {
        fprintf(listing, "Error: Invalid return at line %d\n", t->lineno);
        Error = TRUE;
      }
      break;
    case ParamDK:
      break;
    default:
      break;
    }
  default:
    break;
  }
  return;
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *syntaxTree)
{
  sc_push(globalScope);
  traverse(syntaxTree, beforeCheckNode, checkNode);
  sc_pop();
}
