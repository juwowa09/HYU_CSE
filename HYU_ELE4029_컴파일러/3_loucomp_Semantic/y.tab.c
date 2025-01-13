/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 7 "cminus.y"

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


#line 88 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IFX = 258,                     /* IFX  */
    IF = 259,                      /* IF  */
    ELSE = 260,                    /* ELSE  */
    WHILE = 261,                   /* WHILE  */
    RETURN = 262,                  /* RETURN  */
    INT = 263,                     /* INT  */
    VOID = 264,                    /* VOID  */
    ID = 265,                      /* ID  */
    NUM = 266,                     /* NUM  */
    COMMA = 267,                   /* COMMA  */
    ASSIGN = 268,                  /* ASSIGN  */
    LT = 269,                      /* LT  */
    LE = 270,                      /* LE  */
    GT = 271,                      /* GT  */
    GE = 272,                      /* GE  */
    EQ = 273,                      /* EQ  */
    NE = 274,                      /* NE  */
    PLUS = 275,                    /* PLUS  */
    MINUS = 276,                   /* MINUS  */
    TIMES = 277,                   /* TIMES  */
    OVER = 278,                    /* OVER  */
    LPAREN = 279,                  /* LPAREN  */
    RPAREN = 280,                  /* RPAREN  */
    LBRACE = 281,                  /* LBRACE  */
    RBRACE = 282,                  /* RBRACE  */
    LCURLY = 283,                  /* LCURLY  */
    RCURLY = 284,                  /* RCURLY  */
    SEMI = 285,                    /* SEMI  */
    ERROR = 286,                   /* ERROR  */
    ENDFILE = 287                  /* ENDFILE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IFX 258
#define IF 259
#define ELSE 260
#define WHILE 261
#define RETURN 262
#define INT 263
#define VOID 264
#define ID 265
#define NUM 266
#define COMMA 267
#define ASSIGN 268
#define LT 269
#define LE 270
#define GT 271
#define GE 272
#define EQ 273
#define NE 274
#define PLUS 275
#define MINUS 276
#define TIMES 277
#define OVER 278
#define LPAREN 279
#define RPAREN 280
#define LBRACE 281
#define RBRACE 282
#define LCURLY 283
#define RCURLY 284
#define SEMI 285
#define ERROR 286
#define ENDFILE 287

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IFX = 3,                        /* IFX  */
  YYSYMBOL_IF = 4,                         /* IF  */
  YYSYMBOL_ELSE = 5,                       /* ELSE  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_RETURN = 7,                     /* RETURN  */
  YYSYMBOL_INT = 8,                        /* INT  */
  YYSYMBOL_VOID = 9,                       /* VOID  */
  YYSYMBOL_ID = 10,                        /* ID  */
  YYSYMBOL_NUM = 11,                       /* NUM  */
  YYSYMBOL_COMMA = 12,                     /* COMMA  */
  YYSYMBOL_ASSIGN = 13,                    /* ASSIGN  */
  YYSYMBOL_LT = 14,                        /* LT  */
  YYSYMBOL_LE = 15,                        /* LE  */
  YYSYMBOL_GT = 16,                        /* GT  */
  YYSYMBOL_GE = 17,                        /* GE  */
  YYSYMBOL_EQ = 18,                        /* EQ  */
  YYSYMBOL_NE = 19,                        /* NE  */
  YYSYMBOL_PLUS = 20,                      /* PLUS  */
  YYSYMBOL_MINUS = 21,                     /* MINUS  */
  YYSYMBOL_TIMES = 22,                     /* TIMES  */
  YYSYMBOL_OVER = 23,                      /* OVER  */
  YYSYMBOL_LPAREN = 24,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 25,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 26,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 27,                    /* RBRACE  */
  YYSYMBOL_LCURLY = 28,                    /* LCURLY  */
  YYSYMBOL_RCURLY = 29,                    /* RCURLY  */
  YYSYMBOL_SEMI = 30,                      /* SEMI  */
  YYSYMBOL_ERROR = 31,                     /* ERROR  */
  YYSYMBOL_ENDFILE = 32,                   /* ENDFILE  */
  YYSYMBOL_YYACCEPT = 33,                  /* $accept  */
  YYSYMBOL_program = 34,                   /* program  */
  YYSYMBOL_declare_list = 35,              /* declare_list  */
  YYSYMBOL_declare = 36,                   /* declare  */
  YYSYMBOL_var_declare = 37,               /* var_declare  */
  YYSYMBOL_38_1 = 38,                      /* @1  */
  YYSYMBOL_id = 39,                        /* id  */
  YYSYMBOL_num = 40,                       /* num  */
  YYSYMBOL_type_spec = 41,                 /* type_spec  */
  YYSYMBOL_func_declare = 42,              /* func_declare  */
  YYSYMBOL_43_2 = 43,                      /* @2  */
  YYSYMBOL_params = 44,                    /* params  */
  YYSYMBOL_param_list = 45,                /* param_list  */
  YYSYMBOL_param = 46,                     /* param  */
  YYSYMBOL_compound_stmt = 47,             /* compound_stmt  */
  YYSYMBOL_local_declare = 48,             /* local_declare  */
  YYSYMBOL_stmt_list = 49,                 /* stmt_list  */
  YYSYMBOL_stmt = 50,                      /* stmt  */
  YYSYMBOL_exp_stmt = 51,                  /* exp_stmt  */
  YYSYMBOL_if_stmt = 52,                   /* if_stmt  */
  YYSYMBOL_if_else_stmt = 53,              /* if_else_stmt  */
  YYSYMBOL_iteration_stmt = 54,            /* iteration_stmt  */
  YYSYMBOL_non_val_ret_stmt = 55,          /* non_val_ret_stmt  */
  YYSYMBOL_val_ret_stmt = 56,              /* val_ret_stmt  */
  YYSYMBOL_exp = 57,                       /* exp  */
  YYSYMBOL_simple_exp = 58,                /* simple_exp  */
  YYSYMBOL_var = 59,                       /* var  */
  YYSYMBOL_60_3 = 60,                      /* @3  */
  YYSYMBOL_relop = 61,                     /* relop  */
  YYSYMBOL_additive_exp = 62,              /* additive_exp  */
  YYSYMBOL_addop = 63,                     /* addop  */
  YYSYMBOL_term = 64,                      /* term  */
  YYSYMBOL_mulop = 65,                     /* mulop  */
  YYSYMBOL_call = 66,                      /* call  */
  YYSYMBOL_67_4 = 67,                      /* @4  */
  YYSYMBOL_args = 68,                      /* args  */
  YYSYMBOL_arg_list = 69,                  /* arg_list  */
  YYSYMBOL_factor = 70                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   104

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  111

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   287


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    39,    39,    43,    55,    60,    60,    61,    67,    67,
      79,    82,    86,    91,    94,    94,   107,   109,   112,   122,
     123,   129,   136,   142,   153,   155,   165,   167,   168,   169,
     170,   171,   172,   173,   176,   176,   177,   184,   192,   199,
     204,   210,   217,   221,   227,   230,   235,   235,   243,   246,
     249,   252,   255,   258,   261,   266,   269,   272,   277,   284,
     287,   290,   293,   293,   302,   302,   303,   313,   315,   316,
     316,   317,   318
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IFX", "IF", "ELSE",
  "WHILE", "RETURN", "INT", "VOID", "ID", "NUM", "COMMA", "ASSIGN", "LT",
  "LE", "GT", "GE", "EQ", "NE", "PLUS", "MINUS", "TIMES", "OVER", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "LCURLY", "RCURLY", "SEMI", "ERROR",
  "ENDFILE", "$accept", "program", "declare_list", "declare",
  "var_declare", "@1", "id", "num", "type_spec", "func_declare", "@2",
  "params", "param_list", "param", "compound_stmt", "local_declare",
  "stmt_list", "stmt", "exp_stmt", "if_stmt", "if_else_stmt",
  "iteration_stmt", "non_val_ret_stmt", "val_ret_stmt", "exp",
  "simple_exp", "var", "@3", "relop", "additive_exp", "addop", "term",
  "mulop", "call", "@4", "args", "arg_list", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-63)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      16,   -94,   -94,     8,    16,   -94,   -94,     2,   -94,   -94,
     -94,   -94,   -21,   -94,     3,    33,    47,    25,   -94,    -8,
      34,     2,    37,    49,   -94,    39,    45,    35,    16,   -94,
      43,   -94,   -94,   -94,   -94,    16,   -94,     2,     7,    42,
      50,    60,    -4,    44,   -94,   -94,   -94,    15,   -94,   -94,
     -94,   -94,   -94,   -94,   -94,   -94,   -94,    55,   -94,    73,
      62,    22,   -94,   -94,    44,    44,   -94,    57,    48,    63,
      64,   -94,    44,   -94,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,    44,    44,   -94,   -94,    44,    65,    66,   -94,   -94,
      44,    44,   -94,   -94,    30,    22,   -94,    36,    36,    67,
     -94,    68,    80,    90,   -94,   -94,   -94,    44,    36,   -94,
     -94
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    12,    13,     0,     2,     4,     5,     0,     6,     1,
       3,    10,     8,     7,     0,     0,     0,     0,    11,     0,
      13,     0,     0,    16,    19,     0,    20,     0,     0,     9,
       0,    24,    15,    18,    21,    26,    23,     0,     0,     8,
       0,     0,     0,     0,    22,    35,    72,    45,    71,    28,
      25,    27,    29,    30,    31,    33,    32,     0,    42,    69,
      44,    55,    70,    59,     0,     0,    39,     0,     0,     0,
       0,    34,     0,    50,    51,    49,    48,    52,    53,    56,
      57,     0,     0,    60,    61,     0,     0,     0,    40,    68,
       0,    65,    41,    69,    43,    54,    58,     0,     0,     0,
      67,     0,    64,    36,    38,    47,    63,     0,     0,    66,
      37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -94,   -94,   -94,    92,    69,   -94,    -5,    81,    -7,   -94,
     -94,   -94,   -94,    70,    72,   -94,   -94,   -93,   -94,   -94,
     -94,   -94,   -94,   -94,   -42,   -94,   -29,   -94,   -94,    19,
     -94,    20,   -94,   -94,   -94,   -94,   -94,    18
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,     6,    14,    47,    48,     7,     8,
      15,    22,    23,    24,    49,    35,    38,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    69,    81,    60,
      82,    61,    85,    62,    70,   101,   102,    63
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      67,    68,    12,   -14,   103,   104,    11,    18,     9,    13,
      21,    40,    11,    41,    42,   110,    26,    11,    18,    25,
      43,    21,    86,    87,     1,     2,    66,    46,    37,    16,
      92,    43,    39,     1,    20,    31,    44,    45,    46,   -62,
      40,   -46,    41,    42,    83,    84,    11,    18,    99,   100,
      79,    80,    93,    93,    11,    18,    93,    17,    18,   -17,
      43,    28,    27,    31,    31,   109,    45,    46,    43,    29,
      34,    30,    13,    89,    64,    46,    73,    74,    75,    76,
      77,    78,    79,    80,    65,    71,    72,    88,    91,    90,
      97,    98,   107,   106,   105,   108,    10,    19,    33,    32,
      94,     0,    95,    96,    36
};

static const yytype_int8 yycheck[] =
{
      42,    43,     7,    24,    97,    98,    10,    11,     0,    30,
      17,     4,    10,     6,     7,   108,    21,    10,    11,    27,
      24,    28,    64,    65,     8,     9,    30,    31,    35,    26,
      72,    24,    37,     8,     9,    28,    29,    30,    31,    24,
       4,    26,     6,     7,    22,    23,    10,    11,    90,    91,
      20,    21,    81,    82,    10,    11,    85,    24,    11,    25,
      24,    12,    25,    28,    28,   107,    30,    31,    24,    30,
      27,    26,    30,    25,    24,    31,    14,    15,    16,    17,
      18,    19,    20,    21,    24,    30,    13,    30,    24,    26,
      25,    25,    12,    25,    27,     5,     4,    16,    28,    27,
      81,    -1,    82,    85,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     8,     9,    34,    35,    36,    37,    41,    42,     0,
      36,    10,    39,    30,    38,    43,    26,    24,    11,    40,
       9,    41,    44,    45,    46,    27,    39,    25,    12,    30,
      26,    28,    47,    46,    27,    48,    37,    41,    49,    39,
       4,     6,     7,    24,    29,    30,    31,    39,    40,    47,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      62,    64,    66,    70,    24,    24,    30,    57,    57,    60,
      67,    30,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    61,    63,    22,    23,    65,    57,    57,    30,    25,
      26,    24,    57,    59,    62,    64,    70,    25,    25,    57,
      57,    68,    69,    50,    50,    27,    25,    12,     5,    57,
      50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    37,    38,    37,
      39,    40,    41,    41,    43,    42,    44,    44,    45,    45,
      46,    46,    47,    48,    48,    49,    49,    50,    50,    50,
      50,    50,    50,    50,    51,    51,    52,    53,    54,    55,
      56,    57,    57,    58,    58,    59,    60,    59,    61,    61,
      61,    61,    61,    61,    62,    62,    63,    63,    64,    64,
      65,    65,    67,    66,    68,    68,    69,    69,    70,    70,
      70,    70,    70
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     0,     7,
       1,     1,     1,     1,     0,     7,     1,     1,     3,     1,
       2,     4,     4,     2,     0,     2,     0,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     5,     7,     5,     2,
       3,     3,     1,     3,     1,     1,     0,     5,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     0,     5,     1,     0,     3,     1,     3,     1,
       1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: declare_list  */
#line 40 "cminus.y"
                 { savedTree = yyvsp[0];}
#line 1327 "y.tab.c"
    break;

  case 3: /* declare_list: declare_list declare  */
#line 43 "cminus.y"
                                     {
                  YYSTYPE t = yyvsp[-1];
                  if(t!=NULL){
                    while(t->sibling != NULL)
                      t=t->sibling;
                    t->sibling = yyvsp[0];
                    yyval = yyvsp[-1];
                  }
                  else{
                    yyval = yyvsp[0];
                  }
                }
#line 1344 "y.tab.c"
    break;

  case 4: /* declare_list: declare  */
#line 55 "cminus.y"
                       {
                yyval = yyvsp[0];
             }
#line 1352 "y.tab.c"
    break;

  case 5: /* declare: var_declare  */
#line 60 "cminus.y"
                        {yyval = yyvsp[0];}
#line 1358 "y.tab.c"
    break;

  case 6: /* declare: func_declare  */
#line 60 "cminus.y"
                                                 {yyval = yyvsp[0];}
#line 1364 "y.tab.c"
    break;

  case 7: /* var_declare: type_spec id SEMI  */
#line 61 "cminus.y"
                                 {
                  yyval = newDeclareNode(NonArrayDK);
                  yyval->type = yyvsp[-2]->type;
                  yyval->attr.name =savedName;
                  yyval->lineno = savedLineNo;
              }
#line 1375 "y.tab.c"
    break;

  case 8: /* @1: %empty  */
#line 67 "cminus.y"
                            {
                  yyval = newDeclareNode(ArrayDK);
                  yyval->type = yyvsp[-1]->type;
                  yyval->attr.name =savedName;
                  yyval->lineno = savedLineNo;
              }
#line 1386 "y.tab.c"
    break;

  case 9: /* var_declare: type_spec id @1 LBRACE num RBRACE SEMI  */
#line 72 "cminus.y"
                                      {
                  yyval=yyvsp[-4];
                  
                  yyval->child[0] = yyvsp[-2];

              }
#line 1397 "y.tab.c"
    break;

  case 10: /* id: ID  */
#line 79 "cminus.y"
                     {  
                   savedName = copyString(tokenString);
                   savedLineNo = lineno; }
#line 1405 "y.tab.c"
    break;

  case 11: /* num: NUM  */
#line 82 "cminus.y"
                     {  
                 yyval = newExpNode(ConstK);
                 yyval->lineno = lineno;
                 yyval->attr.val = atoi(tokenString); }
#line 1414 "y.tab.c"
    break;

  case 12: /* type_spec: INT  */
#line 86 "cminus.y"
                  {
                  yyval=newExpNode(IdK);
                  yyval->type = Integer;
                  yyval->lineno = lineno;
              }
#line 1424 "y.tab.c"
    break;

  case 13: /* type_spec: VOID  */
#line 91 "cminus.y"
                    {yyval=newExpNode(IdK);
                  yyval->type = Void;
                  yyval->lineno = lineno;}
#line 1432 "y.tab.c"
    break;

  case 14: /* @2: %empty  */
#line 94 "cminus.y"
                              {
                    yyval = newDeclareNode(FuncDK);
                    yyval->type = yyvsp[-1]->type;
                    yyval->attr.name = savedName;
                    yyval->lineno = savedLineNo;

                  }
#line 1444 "y.tab.c"
    break;

  case 15: /* func_declare: type_spec id @2 LPAREN params RPAREN compound_stmt  */
#line 100 "cminus.y"
                                                     {
                    yyval = yyvsp[-4];
                    yyval->child[0] = yyvsp[-2];
                    yyval->child[1] = yyvsp[0];

              }
#line 1455 "y.tab.c"
    break;

  case 16: /* params: param_list  */
#line 107 "cminus.y"
                          {yyval=yyvsp[0];}
#line 1461 "y.tab.c"
    break;

  case 17: /* params: VOID  */
#line 109 "cminus.y"
                      {yyval=newDeclareNode(NonParamDK);
                  yyval->lineno = lineno;
                  yyval->type = Void;}
#line 1469 "y.tab.c"
    break;

  case 18: /* param_list: param_list COMMA param  */
#line 112 "cminus.y"
                                      {
                  YYSTYPE t = yyvsp[-2];
                  if(t != NULL){
                  while(t->sibling != NULL)
                    t=t->sibling;
                  t->sibling = yyvsp[0];
                  yyval = yyvsp[-2];}
                  else yyval = yyvsp[0];
                  
              }
#line 1484 "y.tab.c"
    break;

  case 19: /* param_list: param  */
#line 122 "cminus.y"
                      {yyval=yyvsp[0];}
#line 1490 "y.tab.c"
    break;

  case 20: /* param: type_spec id  */
#line 123 "cminus.y"
                             {
                  yyval = newDeclareNode(ParamDK);
                  yyval->type = yyvsp[-1]->type;
                  yyval->attr.name = savedName;
                  yyval->lineno = savedLineNo;
              }
#line 1501 "y.tab.c"
    break;

  case 21: /* param: type_spec id LBRACE RBRACE  */
#line 129 "cminus.y"
                                          {
                  yyval = newDeclareNode(ParamDK);
                  yyval->type = yyvsp[-3]->type;
                  yyval->attr.name = savedName;
                  yyval->lineno = savedLineNo;
              }
#line 1512 "y.tab.c"
    break;

  case 22: /* compound_stmt: LCURLY local_declare stmt_list RCURLY  */
#line 136 "cminus.y"
                                                     {
                yyval=newStmtNode(CompK);
                yyval->child[0] = yyvsp[-2];
                yyval->child[1] = yyvsp[-1];
                yyval->lineno = lineno;
              }
#line 1523 "y.tab.c"
    break;

  case 23: /* local_declare: local_declare var_declare  */
#line 142 "cminus.y"
                                          {
                 YYSTYPE t = yyvsp[-1];
                  if(t!=NULL){
                    while(t->sibling != NULL)
                      t=t->sibling;
                    t->sibling = yyvsp[0];
                    yyval = yyvsp[-1];
                  }
                  else yyval = yyvsp[0];
              
              }
#line 1539 "y.tab.c"
    break;

  case 24: /* local_declare: %empty  */
#line 153 "cminus.y"
                {yyval=NULL;}
#line 1545 "y.tab.c"
    break;

  case 25: /* stmt_list: stmt_list stmt  */
#line 156 "cminus.y"
                 { YYSTYPE t = yyvsp[-1];
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = yyvsp[0];
                     yyval = yyvsp[-1]; 
                     }
                     else yyval = yyvsp[0];
                 }
#line 1559 "y.tab.c"
    break;

  case 26: /* stmt_list: %empty  */
#line 165 "cminus.y"
                { yyval=NULL ;}
#line 1565 "y.tab.c"
    break;

  case 27: /* stmt: exp_stmt  */
#line 167 "cminus.y"
                        { yyval = yyvsp[0]; }
#line 1571 "y.tab.c"
    break;

  case 28: /* stmt: compound_stmt  */
#line 168 "cminus.y"
                             { yyval = yyvsp[0]; }
#line 1577 "y.tab.c"
    break;

  case 29: /* stmt: if_stmt  */
#line 169 "cminus.y"
                      { yyval = yyvsp[0]; }
#line 1583 "y.tab.c"
    break;

  case 30: /* stmt: if_else_stmt  */
#line 170 "cminus.y"
                           { yyval=yyvsp[0]; }
#line 1589 "y.tab.c"
    break;

  case 31: /* stmt: iteration_stmt  */
#line 171 "cminus.y"
                              { yyval = yyvsp[0]; }
#line 1595 "y.tab.c"
    break;

  case 32: /* stmt: val_ret_stmt  */
#line 172 "cminus.y"
                            { yyval = yyvsp[0]; }
#line 1601 "y.tab.c"
    break;

  case 33: /* stmt: non_val_ret_stmt  */
#line 173 "cminus.y"
                                {yyval = yyvsp[0];}
#line 1607 "y.tab.c"
    break;

  case 34: /* exp_stmt: exp SEMI  */
#line 176 "cminus.y"
                       {yyval=yyvsp[-1];}
#line 1613 "y.tab.c"
    break;

  case 35: /* exp_stmt: SEMI  */
#line 176 "cminus.y"
                                      {yyval=NULL;}
#line 1619 "y.tab.c"
    break;

  case 36: /* if_stmt: IF LPAREN exp RPAREN stmt  */
#line 177 "cminus.y"
                                                  {
                  yyval = newStmtNode(IfK);
                  yyval->lineno = lineno;
                  yyval->child[0] = yyvsp[-2];
                  yyval->child[1] = yyvsp[0];
              }
#line 1630 "y.tab.c"
    break;

  case 37: /* if_else_stmt: IF LPAREN exp RPAREN stmt ELSE stmt  */
#line 184 "cminus.y"
                                                  {
                  yyval = newStmtNode(IfElseK);
                  yyval->lineno = lineno;
                  yyval->child[0] = yyvsp[-4];
                  yyval->child[1] = yyvsp[-2];
                  yyval->child[2] = yyvsp[0];
              }
#line 1642 "y.tab.c"
    break;

  case 38: /* iteration_stmt: WHILE LPAREN exp RPAREN stmt  */
#line 192 "cminus.y"
                                             {
                  yyval = newStmtNode(IterK);
                  yyval->lineno = lineno;
                  yyval->child[0] =yyvsp[-2];
                  yyval->child[1] =yyvsp[0];
              }
#line 1653 "y.tab.c"
    break;

  case 39: /* non_val_ret_stmt: RETURN SEMI  */
#line 199 "cminus.y"
                             {
                  yyval=newStmtNode(NonValRetK);
                  yyval->lineno = lineno;
              }
#line 1662 "y.tab.c"
    break;

  case 40: /* val_ret_stmt: RETURN exp SEMI  */
#line 204 "cminus.y"
                                {
                  yyval=newStmtNode(ValRetK);
                  yyval->lineno = lineno;
                  yyval->child[0] = yyvsp[-1];
              }
#line 1672 "y.tab.c"
    break;

  case 41: /* exp: var ASSIGN exp  */
#line 210 "cminus.y"
                             {
                yyval=newExpNode(AssignK);
                yyval->lineno = lineno;
                yyval->child[0] = yyvsp[-2];
                yyval->child[1] = yyvsp[0];

              }
#line 1684 "y.tab.c"
    break;

  case 42: /* exp: simple_exp  */
#line 217 "cminus.y"
                          {
                  yyval=yyvsp[0];
              }
#line 1692 "y.tab.c"
    break;

  case 43: /* simple_exp: additive_exp relop additive_exp  */
#line 222 "cminus.y"
                 { yyval = yyvsp[-1];
                   yyval->child[0] = yyvsp[-2];
                   yyval->child[1] = yyvsp[0];
                 }
#line 1701 "y.tab.c"
    break;

  case 44: /* simple_exp: additive_exp  */
#line 227 "cminus.y"
                           { yyval = yyvsp[0]; }
#line 1707 "y.tab.c"
    break;

  case 45: /* var: id  */
#line 230 "cminus.y"
                  { 
                   yyval = newExpNode(IdK);
                   yyval->lineno = savedLineNo;
                   yyval->attr.name =savedName;
                 }
#line 1717 "y.tab.c"
    break;

  case 46: /* @3: %empty  */
#line 235 "cminus.y"
                  {
                   yyval = newExpNode(IdK);
                   yyval->lineno = savedLineNo;
                   yyval->attr.name =savedName;
              }
#line 1727 "y.tab.c"
    break;

  case 47: /* var: id @3 LBRACE exp RBRACE  */
#line 239 "cminus.y"
                                 {
                  yyval=yyvsp[-3];
                  yyval->child[0] = yyvsp[-1];
             }
#line 1736 "y.tab.c"
    break;

  case 48: /* relop: GE  */
#line 243 "cminus.y"
                  {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=GE;}
#line 1744 "y.tab.c"
    break;

  case 49: /* relop: GT  */
#line 246 "cminus.y"
                  {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=GT;}
#line 1752 "y.tab.c"
    break;

  case 50: /* relop: LT  */
#line 249 "cminus.y"
                  {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=LT;}
#line 1760 "y.tab.c"
    break;

  case 51: /* relop: LE  */
#line 252 "cminus.y"
                  {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=LE;}
#line 1768 "y.tab.c"
    break;

  case 52: /* relop: EQ  */
#line 255 "cminus.y"
                  {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=EQ;}
#line 1776 "y.tab.c"
    break;

  case 53: /* relop: NE  */
#line 258 "cminus.y"
                  {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=NE;}
#line 1784 "y.tab.c"
    break;

  case 54: /* additive_exp: additive_exp addop term  */
#line 261 "cminus.y"
                                      {
                   yyval = yyvsp[-1];
                   yyval->child[0] = yyvsp[-2];
                   yyval->child[1] = yyvsp[0];
                }
#line 1794 "y.tab.c"
    break;

  case 55: /* additive_exp: term  */
#line 266 "cminus.y"
                      {
                  yyval=yyvsp[0];
                }
#line 1802 "y.tab.c"
    break;

  case 56: /* addop: PLUS  */
#line 269 "cminus.y"
            {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=PLUS;}
#line 1810 "y.tab.c"
    break;

  case 57: /* addop: MINUS  */
#line 272 "cminus.y"
                {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=MINUS;}
#line 1818 "y.tab.c"
    break;

  case 58: /* term: term mulop factor  */
#line 278 "cminus.y"
                  { 
                   yyval = yyvsp[-1];
                   yyval->child[0] = yyvsp[-2];
                   yyval->child[1] = yyvsp[0];
                 }
#line 1828 "y.tab.c"
    break;

  case 59: /* term: factor  */
#line 284 "cminus.y"
                      { yyval = yyvsp[0]; }
#line 1834 "y.tab.c"
    break;

  case 60: /* mulop: TIMES  */
#line 287 "cminus.y"
                    {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=TIMES;}
#line 1842 "y.tab.c"
    break;

  case 61: /* mulop: OVER  */
#line 290 "cminus.y"
                    {yyval=newExpNode(BinK);
                  yyval->lineno = lineno;
                  yyval->attr.op=OVER;}
#line 1850 "y.tab.c"
    break;

  case 62: /* @4: %empty  */
#line 293 "cminus.y"
                  {
                yyval=newExpNode(CallK);
                yyval->lineno = savedLineNo;
                yyval->attr.name = savedName;

                }
#line 1861 "y.tab.c"
    break;

  case 63: /* call: id @4 LPAREN args RPAREN  */
#line 298 "cminus.y"
                                    {
                yyval =yyvsp[-3];
                yyval->child[0] = yyvsp[-1];
              }
#line 1870 "y.tab.c"
    break;

  case 64: /* args: arg_list  */
#line 302 "cminus.y"
                        {yyval=yyvsp[0];}
#line 1876 "y.tab.c"
    break;

  case 65: /* args: %empty  */
#line 302 "cminus.y"
                                   {yyval=NULL;}
#line 1882 "y.tab.c"
    break;

  case 66: /* arg_list: arg_list COMMA exp  */
#line 303 "cminus.y"
                                 {
                  YYSTYPE t = yyvsp[-2];
                  if(t!=NULL){
                  while (t->sibling != NULL)
                        t = t->sibling;
                  t->sibling = yyvsp[0];
                  yyval = yyvsp[-2]; 
                  }
                  else yyval=yyvsp[0];
                  
             }
#line 1898 "y.tab.c"
    break;

  case 67: /* arg_list: exp  */
#line 313 "cminus.y"
                     {yyval=yyvsp[0];}
#line 1904 "y.tab.c"
    break;

  case 68: /* factor: LPAREN exp RPAREN  */
#line 315 "cminus.y"
                                 { yyval = yyvsp[-1]; }
#line 1910 "y.tab.c"
    break;

  case 69: /* factor: var  */
#line 316 "cminus.y"
                  {yyval=yyvsp[0]; }
#line 1916 "y.tab.c"
    break;

  case 70: /* factor: call  */
#line 316 "cminus.y"
                                  { yyval=yyvsp[0];}
#line 1922 "y.tab.c"
    break;

  case 72: /* factor: ERROR  */
#line 318 "cminus.y"
                     { yyval = NULL; }
#line 1928 "y.tab.c"
    break;


#line 1932 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 321 "cminus.y"


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

