/* File: semantic_analyzer.cc

 */

#include "semantic_analyzer.h"
#include "ast.h"
#include "ast_decl.h"
#include "ast_stmt.h"
#include "ast_type.h"

SemanticAnalyzer::SemanticAnalyzer() {
    numErrors = 0;
    curScopeNum = 0;
    curClass = NULL;
}

SemanticAnalyzer::~SemanticAnalyzer() { }

// CONSIDER PUTTING THIS IN AS PART OF CheckSemantics FOR A ClassDecl
void SemanticAnalyzer::handleSubClasses() {
  /*
  Iterator<ClassScope*> hClassesItr = existingClasses.GetIterator();  // iterate over all classes in this program
  ClassScope *curClassScope;
  while((curClassScope = hClassesItr.GetNextValue()) != NULL) {
    if(curClassScope->extends != "") {  // if this class is a subclass of another class
      printf("Incorporating superclass %s.\n", curClassScope->extends.c_str());
      ClassScope *superClassScope = existingClasses.Lookup(curClassScope->extends.c_str());
      if(superClassScope == NULL) {
	// TODO: REPORT ERROR - TRYIGN TO SUBCLASS OFF OF A NON-EXISTENT CLASS
      }
      else {
	// copy the superclass' member vars into this class, taking noteofoverlaps
	Iterator<VarDecl*> varsItr = superClassScope->memberVars.GetIterator();
	VarDecl* curVar;
	while((curVar = varsItr.GetNextValue()) != NULL) {
	  if(curClassScope->memberVars.Lookup(curVar->GetName()) != NULL) {
	    // TODO: REPORT ERROR - THIS CLASS ALREADY HAS A VARIABLE OF THE SAME NAME AS THE SUPERCLASS'
	  }
	  else
	    curClassScope->memberVars.Enter(curVar->GetName(), curVar);
	}
	// copy the superclass' member funcs into this class, taking noteof overlaps
	Iterator< List<FnDecl*>* > funcsItr = superClassScope->memberFuncs.GetIterator();
	List< FnDecl* >* curFuncs;
	while((curFuncs = funcsItr.GetNextValue()) != NULL) {
	  if(curFuncs->NumElements() != 0) {  // just checking..
	    List< FnDecl* >* thisClassFuncsSameName = curClassScope->memberFuncs.Lookup(curFuncs->Nth(0)->GetName());
	    if(thisClassFuncsSameName != NULL) { // we already have funcs of this name, so add the superclass list to this class' list
	      for(int j=0; j<curFuncs->NumElements(); j++)
		thisClassFuncsSameName->Append(curFuncs->Nth(j));
	    }
	    else {  // dump a pointer to this list right into this class' function list
	      curClassScope->memberFuncs.Enter(curFuncs->Nth(0)->GetName(), curFuncs);
	    }
	  }
	}
      }
    }
  }
  */
}

void SemanticAnalyzer::Prepare(Program *_top) {
  top = _top;
  // do some other preparation before tree-traversal:
  // (1) handle subclasses
  handleSubClasses();
}

void SemanticAnalyzer::AddDecls(List<Decl*>* decls) {
    for (int i = 0; i < decls->NumElements(); i++) {
        if (DEBUG) printf("Scope %d: Attempting to add declaration of %s %s.\n",
                curScopeNum, decls->Nth(i)->GetPrintNameForNode(), decls->Nth(i)->GetName());

        // Create new ScopedDecl using current declaration and scope number.
        ScopedDecl* cur = new ScopedDecl;
        cur->scopeNumber = curScopeNum;
        cur->decl = decls->Nth(i);

        // Check for conflict with previous declaration of the same name 
        // within the same scope.
        ScopedDecl* old = existingDecls.Lookup(cur->decl->GetName());
        if (old != NULL && old->scopeNumber == cur->scopeNumber) {
            ReportError::DeclConflict(cur->decl, old->decl);
        }

        // Add to set of existing declarations, shadowing previous declarations.
        existingDecls.Enter(cur->decl->GetName(), cur, false);

        // If the declaration is of a particular type, add it to the appropiate set.
        const char* declType = cur->decl->GetPrintNameForNode();
        if (strcmp(declType, "VarDecl") == 0) {
            curVars.Enter(cur->decl->GetName(), cur, false);
        } else if (strcmp(declType, "FnDecl") == 0) {
            curFuncs.Enter(cur->decl->GetName(), cur, false);
        } else if (strcmp(declType, "ClassDecl") == 0) {
            curClasses.Enter(cur->decl->GetName(), cur, false);
            existingNamedTypes.Enter(cur->decl->GetName(), new NamedType(cur->decl->GetIdentifier()), false);
        } else if (strcmp(declType, "InterfaceDecl") == 0) {
            hInterfaces.Enter(cur->decl->GetName(), (InterfaceDecl*) cur->decl, false);
            existingNamedTypes.Enter(cur->decl->GetName(), new NamedType(cur->decl->GetIdentifier()), false);
        }
    }
}

void SemanticAnalyzer::RemoveDecls(List<Decl*>* decls) {
    for (int i = 0; i < decls->NumElements(); i++) {
        if (DEBUG) printf("Scope %d: Attempting to remove declaration of %s %s.\n",
                curScopeNum, decls->Nth(i)->GetPrintNameForNode(), decls->Nth(i)->GetName());
        
        // Remove declaration from set of existing declarations.  Asserts guarantee that 
        // this is the declaration we added in the first place.
        Decl* cur = decls->Nth(i);
        char* declName = cur->GetName();
        ScopedDecl* toRemove = existingDecls.Lookup(declName);
        Assert(toRemove != NULL);
        if (DEBUG) printf("this declaration has a scope of %d, the current scope number is %d.\n", toRemove->scopeNumber, curScopeNum);
        Assert(toRemove->scopeNumber == curScopeNum);
        existingDecls.Remove(declName, toRemove);

        // If the declaration is of a particular type, remove it from the appropiate set.
        const char* declType = cur->GetPrintNameForNode();
        if (strcmp(declType, "VarDecl") == 0) {
            ScopedDecl* toRemove = curVars.Lookup(declName);
            Assert(toRemove != NULL);
            curVars.Remove(declName, toRemove);
        } else if (strcmp(declType, "FnDecl") == 0) {
            ScopedDecl* toRemove = curFuncs.Lookup(declName);
            Assert(toRemove != NULL);
            curFuncs.Remove(declName, toRemove);
        }
    }
}

FnDecl* SemanticAnalyzer::FnDeclCmp(FnDecl* fn, List<FnDecl*>* fnList) {
  // if 2 functions have the same name, we need to know whether they are
  // overloaded functions or actually a redeclaration.
  // we need to check:  their return types and formals
  for(int i=0; i<fnList->NumElements(); i++) {
    if(fn->MatchesSignature(fnList->Nth(i)))
      return fnList->Nth(i);
  }
  return NULL;
}
