/* File: semantic_analyzer.h
  Just a header file, so other files can believe that these
  structs and classes exist.
 */

#include "errors.h"
#include "list.h"
#include "hashtable.h"
#include <stack>
#include <string>

extern bool DEBUG;
class VarDecl;
class FnDecl;
class ClassDecl;
class InterfaceDecl;
class Program;
class NamedType;

// why do we need this?
// b/c when we're in a scope and we see the line "int a;",
// and we search hCurVars for a and find 2 entries, we need
// to take the one with deeper scope
struct ScopedDecl {
    int scopeNumber;
    Decl* decl;
};

/*
// upon entering a scope, we store all its variables and functions
// in a list, and put that on a stack (in addition to adding the vars/funcs to the hashmaps)
// then, upon leaving the scope, we pop the stack, iterate through the list
// of KeyValuePairs, and remove them from the hashmaps.
// (we need to pass the Hashtable's Remove method both a key and value
// to remove a pair)
struct KeyValuePair {
  char key[32];  // allows var/func names up to 31 chars in length
  ScopedNode *value;
  };*/

// this stores the member functions and variables (including
// those of a superclass, if applicable) of a class.
// it also stores the name of the superclass (again, if applicable)
/*struct ClassScope {
    ClassDecl* c;
    Hashtable<VarDecl*> memberVars;
    Hashtable< List<FnDecl*>* > memberFuncs;  // this is a list while the above is not b/c a class can have multiple functions (but not variables) of the same name
    string extends;
};*/

class SemanticAnalyzer {
    private:
        void handleSubClasses();

    public:
        int numErrors;
        int curScopeNum;
        Program *top;

        SemanticAnalyzer();
        ~SemanticAnalyzer();
        void Prepare(Program *_top);

        // data structures that contain references to all variables and functions available 
        // at any given point in the tree traversal  
        Hashtable<ScopedDecl*> existingDecls;
        Hashtable<NamedType*> existingNamedTypes;
        Hashtable<ScopedDecl*> curVars;
        Hashtable<ScopedDecl*> curFuncs;
        //stack< List<KeyValuePair>* > sVarsToRemove;
        //stack< List<KeyValuePair>* > sFuncsToRemove;

        // data structures that store global class and interface information
        // (we fill these up before tree traversal)
        Hashtable<ScopedDecl*> curClasses;  // stores information for every class in our program
        Hashtable<InterfaceDecl* > hInterfaces;  // stores the list of prototypes for each interface in the program

        void AddDecls(List<Decl*>* decls);
        void RemoveDecls(List<Decl*>* decls);

        static FnDecl* FnDeclCmp(FnDecl* fn, List<FnDecl*>* fnList);

	// these help us keep track of whether we are inside a certain
	// scope (inside a fndecl, classdecl, etc.)
	ClassDecl *curClass;
	FnDecl *curFunc;
	//LoopStmt* curLoop;  // TODO if say LoopStmt * curLoop, g++ complains...
	bool curInsideLoop;
};
