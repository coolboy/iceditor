/* File: ast_expr.h
 * ----------------
 * The Expr class and its subclasses are used to represent
 * expressions in the parse tree.  For each expression in the
 * language (add, call, New, etc.) there is a corresponding
 * node class for that construct. 
 */


#ifndef _H_ast_expr
#define _H_ast_expr

#include "ast.h"
#include "ast_stmt.h"
#include "ast_type.h"
#include "list.h"
#include "codegen.h"

struct ScopedNode;
struct KeyValuePair;
struct ClassScope;
class SemanticAnalyzer;


class Expr : public Stmt 
{
  public:
    Expr(yyltype loc) : Stmt(loc) {}
    Expr() : Stmt() {}
    Type *GetResultType() { return resultType; }  // every expression must result in one type (e.g. double, int, Cow)
    // tell me whether it's a boolean or integer or what
    bool isResultError() { return resultType->IsEquivalentTo(Type::errorType); }

    // these must be overridden
    virtual Expr* GetBase() { printf("Default GetBase() called!!!\n"); return NULL;}
    virtual void SetIsLHS(bool _isLHS) { printf("Default SetIsLHS() called!!!\n"); }
    virtual int GetOffset() { printf("Default GetOffset() called!!!\n"); return -1; }
    virtual bool GetIsAMemberVariable() { printf("Default GetIsAMemberVariable() called!!!\n"); return false; }
    
 protected:
    Type *resultType;  // NOTE:  the CheckSemantics() function for any Expr subclass is responsible for filling the field resultType
};

/* This node type is used for those places where an expression is optional.
 * We could use a NULL pointer, but then it adds a lot of checking for
 * NULL. By using a valid, but no-op, node, we save that trouble */ class EmptyExpr : public Expr
{
  public:
    const char *GetPrintNameForNode() { return "Empty"; }
    //    Type *GetResultType() { return Type::emptyType; }
    void CheckSemantics() { resultType = Type::voidType; }
};

class IntConstant : public Expr 
{
  protected:
    int value;
  
  public:
    IntConstant(yyltype loc, int val);
    const char *GetPrintNameForNode() { return "IntConstant"; }
    void PrintChildren(int indentLevel);
    void CheckSemantics() { resultType = Type::intType; };
    void Emit();
};

class DoubleConstant : public Expr 
{
  protected:
    double value;
    
  public:
    DoubleConstant(yyltype loc, double val);
    const char *GetPrintNameForNode() { return "DoubleConstant"; }
    void PrintChildren(int indentLevel);
    void CheckSemantics() { resultType = Type::doubleType; };
};

class BoolConstant : public Expr 
{
  protected:
    bool value;
    
  public:
    BoolConstant(yyltype loc, bool val);
    const char *GetPrintNameForNode() { return "BoolConstant"; }
    void PrintChildren(int indentLevel);
    void CheckSemantics() { resultType = Type::boolType; };
    void Emit();
};

class StringConstant : public Expr 
{ 
  protected:
    char *value;
    
  public:
    StringConstant(yyltype loc, const char *val);
    const char *GetPrintNameForNode() { return "StringConstant"; }
    void PrintChildren(int indentLevel);
    void CheckSemantics() { };
    void Emit();
};

class NullConstant: public Expr 
{
  public: 
    NullConstant(yyltype loc) : Expr(loc) { resultType = Type::nullType; }
    const char *GetPrintNameForNode() { return "NullConstant"; }
    void CheckSemantics() { resultType = Type::nullType; };
    void Emit();
};

class Operator : public Node 
{
  protected:
    char tokenString[4];
    
  public:
    Operator(yyltype loc, const char *tok);
    friend ostream& operator<<(ostream& out, Operator *o) { return out << o->tokenString; }

    const char *GetPrintNameForNode() { return "Operator"; }
    void PrintChildren(int indentLevel);
    char *GetTokenString() { return tokenString; }
    void CheckSemantics() { };
};
 
class CompoundExpr : public Expr
{
  protected:
    Operator *op;
    Expr *left, *right; // left will be NULL if unary
    
  public:
    CompoundExpr(Expr *lhs, Operator *op, Expr *rhs); // for binary
    CompoundExpr(Operator *op, Expr *rhs);             // for unary
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
};

class ArithmeticExpr : public CompoundExpr 
{
  public:
    ArithmeticExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    ArithmeticExpr(Operator *op, Expr *rhs) : CompoundExpr(op,rhs) {}
    const char *GetPrintNameForNode() { return "ArithmeticExpr"; }
    //    Type* GetResultType();
    void CheckSemantics();
    void Emit();
};

class PostfixExpr : public Expr
{
  protected:
    Expr *left;
    Operator *op;
  public:
    PostfixExpr(Operator *op, Expr *left);
    const char *GetPrintNameForNode() { return "PostfixExpr"; }
    void PrintChildren(int indentLevel);
    //    Type* GetResultType();
    void CheckSemantics();
};

class RelationalExpr : public CompoundExpr 
{
  public:
    RelationalExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    const char *GetPrintNameForNode() { return "RelationalExpr"; }

    //    Type *GetResultType() { return Type::boolType; }
    //    Type *GetResultType();  // don't automatically return boolType, b/c there may be something like "1 && 'c'" in this expression, which is an *errorType*
    void CheckSemantics();
};

class EqualityExpr : public CompoundExpr 
{
  public:
    EqualityExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    const char *GetPrintNameForNode() { return "EqualityExpr"; }

    //    Type *GetResultType() { return Type::boolType; }
    //   Type *GetResultType();  // don't automatically return boolType, b/c there may be something like "1 && 'c'" in this expression, which is an *errorType*
    void CheckSemantics();
    void Emit();
};

class LogicalExpr : public CompoundExpr 
{
  public:
    LogicalExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    LogicalExpr(Operator *op, Expr *rhs) : CompoundExpr(op,rhs) {}
    const char *GetPrintNameForNode() { return "LogicalExpr"; }

    //    Type *GetResultType() { return Type::boolType; }
    //    Type *GetResultType();  // don't automatically return boolType, b/c there may be something like "1 && 'c'" in this expression, which is an *errorType*
    void CheckSemantics();
    void Emit();
};

class AssignExpr : public CompoundExpr 
{
  public:
    AssignExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    const char *GetPrintNameForNode() { return "AssignExpr"; }
    //    Type *GetResultType();
    void CheckSemantics();
    void Emit();
};

class LValue : public Expr 
{
  protected:
    bool isLHS;
    int offset;
    bool isAMemberVariable;
    
  public:
    LValue(yyltype loc) : Expr(loc) {}
  //  Type *GetResultType();
    void SetIsLHS(bool _isLHS) { isLHS = _isLHS; }
    int GetOffset() { return offset; }
    bool GetIsAMemberVariable() { return isAMemberVariable; }
};

class This : public Expr 
{
  public:
    This(yyltype loc) : Expr(loc) {}
    const char *GetPrintNameForNode() { return "This"; }

    void CheckSemantics();
    void Emit();
    Expr *GetBase() { return NULL; }
};

class ArrayAccess : public LValue 
{
  protected:
    Expr *base, *subscript;
    
  public:
    ArrayAccess(yyltype loc, Expr *base, Expr *subscript);
    const char *GetPrintNameForNode() { return "ArrayAccess"; }
    void PrintChildren(int indentLevel);
    //    Type *GetResultType(); // TODO i should be able to get the type immediately with something like   base->GetResultType()
    
    void CheckSemantics();
    void Emit();

    Expr* GetBase() { return base; }
};

/* Note that field access is used both for qualified names
 * base.field and just field without qualification. We don't
 * know for sure whether there is an implicit "this." in
 * front until later on, so we use one node type for either
 * and sort it out later. */ class FieldAccess : public LValue 
{
  protected:
    Expr *base;	// will be NULL if no explicit base
    Identifier *field;
    
  public:
    FieldAccess(Expr *base, Identifier *field); //ok to pass NULL base
    const char *GetPrintNameForNode() { return "FieldAccess"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
    //    Type *GetResultType();
    char *GetFieldName();

    Expr* GetBase() { return base; }
};

/* Like field access, call is used both for qualified base.field()
 * and unqualified field().  We won't figure out until later
 * whether we need implicit "this." so we use one node type for either
 * and sort it out later. */ 
class Call : public Expr 
{
  protected:
    Expr *base;	// will be NULL if no explicit base
    Identifier *field;
    List<Expr*> *actuals;
    
  public:
    Call(yyltype loc, Expr *base, Identifier *field, List<Expr*> *args);
    const char *GetPrintNameForNode() { return "Call"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
    //    Type *GetResultType();
};

class NewExpr : public Expr
{
  protected:
    NamedType *cType;
    
  public:
    NewExpr(yyltype loc, NamedType *clsType);
    const char *GetPrintNameForNode() { return "NewExpr"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
};

class NewArrayExpr : public Expr
{
  protected:
    Expr *size;
    Type *elemType;
    
  public:
    NewArrayExpr(yyltype loc, Expr *sizeExpr, Type *elemType);
    const char *GetPrintNameForNode() { return "NewArrayExpr"; }
    void PrintChildren(int indentLevel);
    
    void CheckSemantics();
    void Emit();
};

class ReadIntegerExpr : public Expr
{
  public:
    ReadIntegerExpr(yyltype loc) : Expr(loc) { resultType = Type::intType; }
    const char *GetPrintNameForNode() { return "ReadIntegerExpr"; }

    void CheckSemantics();
    void Emit();
};

class ReadLineExpr : public Expr
{
  public:
    ReadLineExpr(yyltype loc) : Expr (loc) { resultType = Type::stringType; }
    const char *GetPrintNameForNode() { return "ReadLineExpr"; }

    void CheckSemantics();
    void Emit();
};

    
#endif
