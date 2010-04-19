/* File: ast_type.h
 * ----------------
 * In our parse tree, Type nodes are used to represent and
 * store type information. The base Type class is used
 * for built-in types, the NamedType for classes and interfaces,
 * and the ArrayType for arrays of other types.  
 */
 
#ifndef _H_ast_type
#define _H_ast_type

#include "ast.h"
#include "list.h"

struct ScopedNode;
struct KeyValuePair;
struct ClassScope;
class SemanticAnalyzer;

class Type : public Node 
{
  protected:
    char *typeName;

  public :
    static Type *intType, *doubleType, *boolType, *voidType,
      *nullType, *stringType, *errorType, *emptyType;

    Type(yyltype loc) : Node(loc) {}
    Type(const char *str);
    
    virtual void PrintToStream(ostream& out) { out << typeName; }
    friend ostream& operator<<(ostream& out, Type *t) { t->PrintToStream(out); return out; }
    virtual bool IsEquivalentTo(Type *other);

    const char *GetPrintNameForNode() { return "Type"; }
    void PrintChildren(int indentLevel);
    char *GetName() { return typeName; }
    void CheckSemantics();
};

class NamedType : public Type 
{
  protected:
    Identifier *id;
    
  public:
    NamedType(Identifier *i);
    
    void PrintToStream(ostream& out) { out << id; }
    bool IsEquivalentTo(Type *other);

    const char *GetPrintNameForNode() { return "NamedType"; }
    void PrintChildren(int indentLevel);
    
    char *GetName() { return id->GetName(); }
    Identifier* GetIdentifier() { return id; }
    void CheckSemantics();
};

class ArrayType : public Type 
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);
    
    void PrintToStream(ostream& out) { out << elemType << "[]"; }
    bool IsEquivalentTo(Type *other);

    const char *GetPrintNameForNode() { return "ArrayType"; }
    void PrintChildren(int indentLevel);

    Type *GetElemType() { return elemType; }
    void CheckSemantics();
};

 
#endif
