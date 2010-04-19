/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 ***/

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "hashtable.h"
#include "list.h"

class Type;
class NamedType;
class Identifier;
class Stmt;

struct ScopedNode;
struct KeyValuePair;
struct ClassScope;
class SemanticAnalyzer;


class Decl : public Node 
{
  protected:
    Identifier *id;
    Hashtable<VarDecl*> *variables;
    Hashtable<FnDecl*> *functions;
  
  public:
    Decl(Identifier *name);
    friend ostream& operator<<(ostream& out, Decl *d) { return out << d->id; }

    char *GetName() { return id->GetName(); }
    Identifier *GetIdentifier() { return id; }

    Hashtable<VarDecl*>* GetVarDeclsHashmap() { return variables; }
    Hashtable<FnDecl*>* GetFnDeclsHashmap() { return functions; }
};

class VarDecl : public Decl 
{
  protected:
    Type *type;
    
  public:
    VarDecl(Identifier *name, Type *type);

    const char *GetPrintNameForNode() { return "VarDecl"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();
    Type *GetType() { return type; }
};

struct vtableEntry {
    char *name;
    int offset;
};

class ClassDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;
    Hashtable<Decl *> *hMembers;

    int getSuperClassMembers(ClassDecl* superclass);

    Hashtable<vtableEntry*>* varIndices;
    Hashtable<vtableEntry*>* methodIndices;
    int curVarOffset, curMethodOffset;
    List<const char*> *methodLabels;
    int totalMemberVars, totalMemberMethods;

  public:
    ClassDecl(Identifier *name, NamedType *extends, 
              List<NamedType*> *implements, List<Decl*> *members);

    List<Decl*> *GetMembers() { return members; }
    NamedType *GetExtends() { return extends; }
    List<NamedType*> *GetImplements() { return implements; }

    const char *GetPrintNameForNode() { return "ClassDecl"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    void Emit();

    void AddToOffsets(ClassDecl* superclass);
    vtableEntry *GetMethodVtableEntry(char *methodName);
    List<const char*>* GetMethodLabels() { return methodLabels; }
    Decl *GetDecl(char *name) { return hMembers->Lookup(name); }
    int GetVarOffset(char *varName) { return varIndices->Lookup(varName)->offset + CodeGenerator::VarSize; }
    int GetMethodOffset(char *methodName) { return methodIndices->Lookup(methodName)->offset; }
    char *GetLongMethodName(char *methodName) { return methodIndices->Lookup(methodName)->name; }
    int GetTotalMemberVars() { return totalMemberVars; }
    int GetTotalMemberMethods() { return totalMemberMethods; }
};

class InterfaceDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    
  public:
    InterfaceDecl(Identifier *name, List<Decl*> *members);

    const char *GetPrintNameForNode() { return "InterfaceDecl"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    List<Decl*> *GetMembers() { return members; }
};

class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    const char *label;
    
  public:
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);

    const char *GetPrintNameForNode() { return "FnDecl"; }
    void PrintChildren(int indentLevel);

    void CheckSemantics();
    bool MatchesSignature(FnDecl *other);
    Type *GetReturnType() { return returnType; }
    List<VarDecl*> *GetFormals() { return formals; }
    void Emit();
    const char *GetLabel() { return label; }
    void SetLabel();
};

#endif
