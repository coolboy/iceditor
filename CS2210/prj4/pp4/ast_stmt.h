/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 *
 * pp2: You will need to add new expression and statement node c
 * classes for the additional grammar elements (Switch/Postfix)
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include "list.h"
#include "ast.h"

class Decl;
class VarDecl;
class FnDecl;
class Expr;
class IntConstant;

struct ScopedNode;
struct KeyValuePair;
struct ClassScope;
class SemanticAnalyzer;
  
class Program : public Node
{
  protected:
     List<Decl*> *decls;
     
  public:
     Program(List<Decl*> *declList);
     const char *GetPrintNameForNode() { return "Program"; }
     void PrintChildren(int indentLevel);
     List<FnDecl*>* functions;
     void CheckSemantics();
     void Emit();
};

class Stmt : public Node
{
  public:
     Stmt() : Node() {}
     Stmt(yyltype loc) : Node(loc) {}
     // this function can be overridden, in the case of stmtblocks
     virtual List<VarDecl*>* GetVarDecls() { return new List<VarDecl*>; }
};

class StmtBlock : public Stmt 
{
  protected:
    List<VarDecl*> *decls;
    List<Stmt*> *stmts;
    
  public:
    StmtBlock(List<VarDecl*> *variableDeclarations, List<Stmt*> *statements);
    const char *GetPrintNameForNode() { return "StmtBlock"; }
    void PrintChildren(int indentLevel);

    List<VarDecl*>* GetVarDecls() { return decls; }
    List<Stmt *> *GetStmts() { return stmts; }
    void CheckSemantics();
    void Emit();
};

class CaseStmt : public Stmt
{
  protected:
    List<Stmt*> *stmts;
    IntConstant *condition;
  
  public:
    CaseStmt(IntConstant *condition, List<Stmt*> *stmts);
    const char *GetPrintNameForNode() { if(condition != NULL) return "Case";  else return "Default"; }
    void PrintChildren(int indentLevel);
    void CheckSemantics();
};

class SwitchStmt : public Stmt
{
  protected:
    Expr *test;
    List<CaseStmt*> *cases;
    CaseStmt *defaultStmt;
    
  public:
    SwitchStmt(Expr *test, List<CaseStmt*> *cases, CaseStmt *defaultStmt);
    const char *GetPrintNameForNode() { return "SwitchStmt"; }
    void PrintChildren(int indentLevel);
    void CheckSemantics();
};

class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;
  
  public:
    ConditionalStmt(Expr *testExpr, Stmt *body);

    void CheckSemantics();
};

class LoopStmt : public ConditionalStmt 
{
  protected:
    char *endLabel;
    
  public:
    LoopStmt(Expr *testExpr, Stmt *body)
            : ConditionalStmt(testExpr, body) {}
    char *GetEndLabel() { return endLabel; }
};

class ForStmt : public LoopStmt 
{
  protected:
    Expr *init, *step;
  
  public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
    const char *GetPrintNameForNode() { return "ForStmt"; }
    void PrintChildren(int indentLevel);
    
    void CheckSemantics();
    void Emit();
};

class WhileStmt : public LoopStmt 
{
  public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
    const char *GetPrintNameForNode() { return "WhileStmt"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
};

class IfStmt : public ConditionalStmt 
{
  protected:
    Stmt *elseBody;
  
  public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    const char *GetPrintNameForNode() { return "IfStmt"; }
    void PrintChildren(int indentLevel);
    
    void CheckSemantics();
    void Emit();
};

class BreakStmt : public Stmt 
{
  public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    const char *GetPrintNameForNode() { return "BreakStmt"; }

    void CheckSemantics();
    void Emit();
};

class ReturnStmt : public Stmt  
{
  protected:
    Expr *expr;
  
  public:
    ReturnStmt(yyltype loc, Expr *expr);
    const char *GetPrintNameForNode() { return "ReturnStmt"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
};

class PrintStmt : public Stmt
{
  protected:
    List<Expr*> *args;
    
  public:
    PrintStmt(List<Expr*> *arguments);
    const char *GetPrintNameForNode() { return "PrintStmt"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
};


#endif
