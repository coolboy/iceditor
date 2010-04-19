/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>

#include "semantic_analyzer.h"
extern SemanticAnalyzer SA;

/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather than
 * creates lots of copies.
 */

Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error");
Type *Type::emptyType  = new Type("empty");

Type::Type(const char *n) {
    Assert(n);
    typeName = strdup(n);
}

void Type::PrintChildren(int indentLevel) {
    printf("%s", typeName);
}

void Type::CheckSemantics() {
  
}

bool Type::IsEquivalentTo(Type *other) {
  if(DEBUG) {
    printf("\ntesting equivalence of %s, %s:", typeName, other->GetName());
    this->Print(0);
    other->Print(0);
  }
  return strcmp(typeName, other->GetName()) == 0;
}
	
NamedType::NamedType(Identifier *i) : Type(*i->GetLocation()) {
    Assert(i != NULL);
    (id=i)->SetParent(this);
} 

bool NamedType::IsEquivalentTo(Type *other) {
    // TODO account for when this and other are of different types, but
    // this is a subclass of other
    //   also account for when other is an interface and this is a class
    //   that implements it
    
    if (DEBUG) printf("using NamedType::IsEquivalentTo\n");
    
    // NamedType objects can be equal to null
    //    if (other->IsEquivalentTo(Type::nullType))  <-- this is wrong. we don't want to see if other is equivalent to nullType in general, we want to know if it IS the nullType
    if(strcmp(other->GetName(), Type::nullType->GetName())==0)
        return true;
    // since this is a NamedType, other can't be an ArrayType
    if( strcmp(other->GetPrintNameForNode(), "ArrayType") == 0 )
      return false;
    // if this and other types are the same, they're equivalent
    if( strcmp(GetName(), other->GetName()) == 0 )
      return true;
    
    // Remaining Cases:
    //  * other is a subclass of this
    ScopedDecl* classLookup = SA.curClasses.Lookup(other->GetName());
    if(classLookup != NULL) {
      ClassDecl* otherClass = (ClassDecl*)(classLookup->decl);
      NamedType* otherSuperClass = otherClass->GetExtends();
      if(otherSuperClass != NULL && this->IsEquivalentTo(otherSuperClass))
	return true;
    }

//THIS IS THE OPPOSITE OF WHAT WE WANT, I THINK  * this is a subclass of other
    //ScopedDecl* classLookup = SA.curClasses.Lookup(id->GetName());
    //if(classLookup != NULL) {
      //ClassDecl* thisClass = (ClassDecl*)(classLookup->decl);
      //NamedType* thisSuperClass = thisClass->GetExtends();
      //      if(thisSuperClass != NULL && thisSuperClass->IsEquivalentTo(other))
	//return true;
      //    }

    //  * this is an interface implemented by class other
    InterfaceDecl* interfaceLookup = SA.hInterfaces.Lookup(id->GetName());
    classLookup = SA.curClasses.Lookup(other->GetName());
    if(interfaceLookup != NULL  &&  classLookup != NULL) {
      ClassDecl* otherClass = (ClassDecl*)(classLookup->decl);
      List<NamedType*> *otherInterfaceList = otherClass->GetImplements();
      // see if this is one of the interfaces implemented by class other
      for(int i=0; i<otherInterfaceList->NumElements(); i++) {
	if(otherInterfaceList->Nth(i)->IsEquivalentTo(this))
	  return true;
      }
    }
    //  TODO any more cases?

    // none of the possible combinations worked. so the types aren't equivalent
    return false;
}

void NamedType::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
}

void NamedType::CheckSemantics() {
  if(SA.curClasses.Lookup(typeName) == NULL) {
    ReportError::IdentifierNotDeclared(id, LookingForClass);
    SA.numErrors++;
  }
}

ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
    Assert(et != NULL);
    (elemType=et)->SetParent(this);
    typeName = (char *) "ArrayType";
}

bool ArrayType::IsEquivalentTo(Type *other) {
    if (DEBUG) printf("using ArrayType::IsEquivalentTo\n");
    if (DEBUG) printf("current is an array with elemType %s\n", elemType->GetName());
    bool otherIsArray = strcmp(other->GetPrintNameForNode(), "ArrayType") == 0;
    // TODO account for when this and other are of different types, but
    // this is a subclass of other
    if (otherIsArray) {
        Type *otherElemType = ((ArrayType *) other)->GetElemType();
	if (DEBUG) printf("other is an array with elemType %s\n", otherElemType->GetName());
	// if this is a 1-d array of namedtype's, then other must be EXACTLY the same, not an equivalent type
        if(strcmp(elemType->GetPrintNameForNode(), "NamedType") == 0) {
	  if(strcmp(otherElemType->GetPrintNameForNode(), "NamedType") == 0
	     && strcmp(elemType->GetName(), otherElemType->GetName()) == 0)
	    return true;
	  else
	    return false;
	}
	else  // if this isn't a 1-d array of namedtype's, find equivalence as usual
	  return elemType->IsEquivalentTo(otherElemType);
    }
    return false;
}

void ArrayType::PrintChildren(int indentLevel) {
    elemType->Print(indentLevel+1);
}

void ArrayType::CheckSemantics() {
  elemType->CheckSemantics();
}
