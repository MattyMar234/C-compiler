#pragma once

#include "Scope.h"
#include "Token.h"
#include "Type.h"
#include "Value.h"

typedef enum
{

    BinOp_Add = 0,
    BinOp_Sub,
    BinOp_Mul,
    BinOp_Div,
    BinOp_And,
    BinOp_Or,
    BinOp_Xor,
    BinOp_ShiftLeft,
    BinOp_ShiftRight,
    BinOp_Mod,

    BinOp_LogicalAnd,
    BinOp_LogicalOr,
    BinOp_ArrayAccess,
    BinOp_StructAccessDot,
    BinOp_StructAccessArrow,
    BinOp_LessThan,
    BinOp_LessThanEq,
    BinOp_GreaterThan,
    BinOp_GreaterThanEq,
    BinOp_Equal,
    BinOp_NotEqual,
    BinOp_AssignmentAdd,
    BinOp_AssignmentSub,
    BinOp_AssignmentMul,
    BinOp_AssignmentDiv,
    BinOp_AssignmentAND,
    BinOp_AssignmentOR,
    BinOp_AssignmentXOR,
    BinOp_AssignmentShiftLeft,
    BinOp_AssignmentShiftRight,
    BinOp_AssignmentMod,
    BinOp_Assignment,

    // These BinOps aren't represented by C-tokens
    // but are used in code generation
    BinOp_MulH,
    BinOp_MulQ,
    BinOp_InvQ,

} BinOp;

typedef enum
{
    UnOp_PreIncrement,
    UnOp_PostIncrement,
    UnOp_PreDecrement,
    UnOp_PostDecrement,
    UnOp_LogicalNOT,
    UnOp_BitwiseNOT,
    UnOp_Negate,
    UnOp_Dereference,
    UnOp_AddressOf,
} UnOp;

typedef enum
{
    AST_ExpressionType_ListLiteral,
    AST_ExpressionType_StringLiteral,
    AST_ExpressionType_IntLiteral,
    AST_ExpressionType_TypeCast,
    AST_ExpressionType_UnaryOP,
    AST_ExpressionType_BinaryOP,
    AST_ExpressionType_FunctionCall,
    AST_ExpressionType_VariableAccess,
    AST_ExpressionType_Value,
} AST_ExpressionType;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc; // Only storing line + line number here for ease of debugging. Really
                        // we only need to store a pointer to the token at the start of the
                        // expression/stmt, and then use the LUT in TokenArray.
} AST_Expression;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    BinOp op;
    AST_Expression* exprA;
    AST_Expression* exprB;
} AST_Expression_BinOp;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    UnOp op;
    AST_Expression* exprA;
} AST_Expression_UnOp;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    char* id;
    AST_Expression** parameters;
    size_t numParameters;
} AST_Expression_FunctionCall;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    AST_Expression* exprA;
    VariableType* newType;
} AST_Expression_TypeCast;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    AST_Expression* exprA;
    VariableType* newType;
} AST_Expression_AddrOf;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    uint32_t literal;
} AST_Expression_IntLiteral;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    const char* data;
} AST_Expression_StringLiteral;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    AST_Expression** expressions;
    size_t numExpr;
} AST_Expression_ListLiteral;

typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    char* id;
} AST_Expression_VariableAccess;

// Not generated by the parser, merely
// used when postprocessing the AST
typedef struct
{
    AST_ExpressionType type;
    SourceLocation loc;
    Value value;
    VariableType* vType;
    bool readOnly;
} AST_Expression_Value;

typedef enum
{
    AST_StatementType_Expr,
    AST_StatementType_If,
    AST_StatementType_While,
    AST_StatementType_Do,
    AST_StatementType_For,
    AST_StatementType_Switch,
    AST_StatementType_Declaration,
    AST_StatementType_Scope,
    AST_StatementType_Return,
    AST_StatementType_Empty,
    AST_StatementType_Break,
    AST_StatementType_Continue,
    AST_StatementType_ASM,
    AST_StatementType_Label,
    AST_StatementType_Goto,

} AST_StatementType;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
} AST_Statement;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* expr;
} AST_Statement_Expr;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* cond;
    AST_Statement* ifTrue;
    AST_Statement* ifFalse;
} AST_Statement_If;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* cond;
    AST_Statement* body;
} AST_Statement_While;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* cond;
    AST_Statement* body;
} AST_Statement_Do;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Statement* init;
    AST_Expression* cond;
    AST_Expression* count;
    AST_Statement* body;
    Scope* statementScope;
} AST_Statement_For;

typedef struct
{
    AST_Statement** statements;
    size_t numStatements;
    uint16_t id;
} AST_Statement_Switch_SwitchCase;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* selector;
    AST_Statement_Switch_SwitchCase* cases;
    size_t numCases;
    AST_Statement** defaultCaseStmts;
    size_t numStmtsDefCase;

} AST_Statement_Switch;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* value;
    char* variableName;
    VariableType* variableType;
    AST_Expression_VariableAccess* lastAccess;

} AST_Statement_Declaration;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Statement** statements;
    size_t numStatements;
    Scope* scope;
} AST_Statement_Scope;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    AST_Expression* expr;
} AST_Statement_Return;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    const char* code;
} AST_Statement_ASM;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    const char* label;
} AST_Statement_Label;

typedef struct
{
    AST_StatementType type;
    SourceLocation loc;
    const char* label;
} AST_Statement_Goto;