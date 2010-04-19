/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "semantic_analyzer.h"
#include "codegen.h"

extern SemanticAnalyzer SA;
extern CodeGenerator cg;

Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}

VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}
  
void VarDecl::PrintChildren(int indentLevel) { 
   type->Print(indentLevel+1);
   id->Print(indentLevel+1);
}

void VarDecl::CheckSemantics() {
    Type* toCheck = type;

    // If the type is an array type, we want to check whether the array's
    // elemType is legit rather than the type itself.
    // TODO does this take care of arrays of arrays?
    if (strcmp(toCheck->GetPrintNameForNode(), "ArrayType") == 0) {
        toCheck = ((ArrayType*) toCheck)->GetElemType();
    }

    // If variable type is not one of the default types, check to make sure  
    // the variable type was declared.
    if (strcmp(toCheck->GetPrintNameForNode(), "NamedType") == 0) {
        NamedType* lookup = (NamedType*) SA.existingNamedTypes.Lookup(toCheck->GetName());
        if (lookup == NULL) {
            ReportError::IdentifierNotDeclared(((NamedType*) toCheck)->GetIdentifier(), LookingForType);
        }
    }
    // TODO make sure variables can't be void (and some other default Types) - which ReportError method to use?
}

void VarDecl::Emit() {
    if (CGDEBUG) printf("VarDecl::Emit: emitting variable named %s\n", GetName());

    // Allocate location differently depending on whether the variable is global.
    const char *parentType = GetParent()->GetPrintNameForNode();
    if (strcmp(parentType, "Program") == 0) {
        // Global variable
        if (CGDEBUG) printf("\tallocated relative to global pointer\n");
        cgLoc = new Location(gpRelative, CodeGenerator::OffsetToFirstGlobal + CodeGenerator::VarSize * cg.curNumGlobals++, GetName());
    } else {
        // Variable within a function
        if (CGDEBUG) printf("\tallocated relative to function pointer\n");
        cgLoc = new Location(fpRelative, CodeGenerator::OffsetToFirstLocal - CodeGenerator::VarSize * cg.curNumLocals++, GetName());
    }
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);     
    extends = ex;    
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);

    // fill variable and functions lists
    hMembers = new Hashtable<Decl *>;
    variables = new Hashtable<VarDecl*>;
    functions = new Hashtable<FnDecl*>;
    for (int i = 0; i < members->NumElements(); i++) {
        Decl *decl = members->Nth(i);
        hMembers->Enter(decl->GetName(), decl);
        const char *declType = decl->GetPrintNameForNode();
        if (strcmp(declType, "VarDecl") == 0) {
            variables->Enter(decl->GetName(), (VarDecl*) decl);
        } else if (strcmp(declType, "FnDecl") == 0) {
            functions->Enter(decl->GetName(), (FnDecl*) decl);
        }
    }
}

void ClassDecl::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
    if (extends) extends->Print(indentLevel+1, "(extends) ");
    implements->PrintAll(indentLevel+1, "(implements) ");
    members->PrintAll(indentLevel+1);
}

int ClassDecl::getSuperClassMembers(ClassDecl *superclass) {
  // this function was called to take all members of superclass and
  // add them to the members of this (with relevant duplicate/mismatch
  // checking). the function then recurs on the super-class of superclass.
  // the function returns the number of super class members added.

  int numMembersAdded = 0;
   // go through each member of the superclass
   for (int i = 0; i < superclass->members->NumElements(); i++) {
     Decl* decl = superclass->members->Nth(i);
     const char *declType = decl->GetPrintNameForNode();
     // if this is a VarDecl, report errors as necessary and add to members & variables
     if (strcmp(declType, "VarDecl") == 0) {
       // Report any subclass attempts to override the superclass variable.
       Decl *declConflict = hMembers->Lookup(decl->GetName());
       if (declConflict != NULL) {
	 ReportError::DeclConflict(declConflict, decl);
       } else {
	 // Add variable declaration from superclass to the subclass.
	 variables->Enter(decl->GetName(), (VarDecl *) decl);
	 
	 // Insert at beginning so the declarations precede the subclass
	 // declarations when adding to semantic analyser later on.
	 // TODO maybe we want to add to hMembers as well?  we'd have to 
	 // first remove the subclass member, add the superclass member, then
	 // re-add the subclass member on top - see how functions are handled
	 // as an example
	 members->InsertAt(decl, 0);
	 numMembersAdded++;
       }
     } else if (strcmp(declType, "FnDecl") == 0) {
       if (DEBUG) printf("checking superclass function %s against current class\n", decl->GetName());
       // Report any subclass variables that conflict with superclass function.
       VarDecl *var = variables->Lookup(decl->GetName());
       if (var != NULL) {
	 ReportError::DeclConflict((Decl *) var, decl);
       }
       // Report any subclass functions that are mismatched overrides of superclass function.
       FnDecl* func = functions->Lookup(decl->GetName());
       if (func != NULL && !func->MatchesSignature((FnDecl*) decl)) {
	 ReportError::OverrideMismatch(func);
       }
     }
   }
   
   // Add function declaration from superclass to the subclass.
   // We do this in a separate for loop afterward to prevent the program 
   // from thinking that method conflicts in the superclass are mismatched 
   // overrides (i.e. if superclass mistakenly has two methods named "f," 
   // adding the first one to "functions" as soon as you see it would cause 
   // the program to think the second one was a mismatched override of the
   // first method).
   for (int i = 0; i < superclass->members->NumElements(); i++) {
     Decl *decl = superclass->members->Nth(i);
     const char *declType = decl->GetPrintNameForNode();
     
     if (strcmp(declType, "FnDecl") == 0) {
       if (DEBUG) printf("Adding superclass function %s to subclass %s\n", decl->GetName(), GetName());
       
       VarDecl *varConflict = variables->Lookup(decl->GetName());
       
       // If the subclass attempts to override the superclass method (even 
       // if it does it wrong), we make sure the subclass method is "above"
       // the superclass method in the hashtable.
       FnDecl *subclassMethod = functions->Lookup(decl->GetName());
       if (subclassMethod != NULL) {
	 functions->Remove(decl->GetName(), subclassMethod);
       } else if (varConflict == NULL) {
	 // If the function hasn't been overriden and it has no subclass 
	 // variables conflicting with it either, then we add it to
	 // the list of members that the subclass has.
	 members->InsertAt(decl, 0);
	 numMembersAdded++;
       }
       functions->Enter(decl->GetName(), (FnDecl *) decl, false);
       if (subclassMethod != NULL) {
	 functions->Enter(decl->GetName(), subclassMethod, false);
       }
     }
   }

   // if this superclass has its own superclass, get *its* members
   if(superclass->GetExtends() != NULL) {
     ScopedDecl *lookup = SA.curClasses.Lookup(superclass->GetExtends()->GetName());
     if (lookup == NULL)
       ReportError::IdentifierNotDeclared(extends->GetIdentifier(), LookingForClass);
     else {
       ClassDecl* superduperclass = (ClassDecl*) lookup->decl;
       return numMembersAdded + getSuperClassMembers(superduperclass);
     }
   }
   // if not (or if the superduperclass was inaccessible), just return info from this superclass
   return numMembersAdded;
}

void ClassDecl::CheckSemantics() {
    if (DEBUG) printf("Checking semantics for class %s.\n", GetName());
    SA.curClass = this;

    // TODO Make sure class declaration is global.
    if (SA.curScopeNum != 0) {
        // Haven't done this yet because not sure which ReportError method to use.
    }

    // Superclass-related checks
    int numMembersFromSuperclass = 0;
    if (extends != NULL) {
        if (DEBUG) printf("Doing superclass checks for class %s.\n", GetName());
	
        // Check to make sure any superclass type was previously declared.
        ScopedDecl *lookup = SA.curClasses.Lookup(extends->GetName());
        if (lookup == NULL) {
            ReportError::IdentifierNotDeclared(extends->GetIdentifier(), LookingForClass);
        } else {
            if (DEBUG) printf("looked up value was not null\n");
            // Check to make sure there are no declarations that conflict with superclass
            // We iterate through superclass' members and check against the subclass' hashtables 
            // of variables and functions.
            ClassDecl* superclass = (ClassDecl*) lookup->decl;
	    numMembersFromSuperclass = getSuperClassMembers(superclass);
	    
	    /* for (int i = 0; i < superclass->members->NumElements(); i++) {
                Decl* decl = superclass->members->Nth(i);
                const char *declType = decl->GetPrintNameForNode();
                if (strcmp(declType, "VarDecl") == 0) {
                    // Report any subclass attempts to override the superclass variable.
                    Decl *declConflict = hMembers->Lookup(decl->GetName());
                    if (declConflict != NULL) {
                        ReportError::DeclConflict(declConflict, decl);
                    } else {
                        // Add variable declaration from superclass to the subclass.
                        variables->Enter(decl->GetName(), (VarDecl *) decl);

                        // Insert at beginning so the declarations precede the subclass
                        // declarations when adding to semantic analyser later on.
                        // TODO maybe we want to add to hMembers as well?  we'd have to 
                        // first remove the subclass member, add the superclass member, then
                        // re-add the subclass member on top - see how functions are handled
                        // as an example
                        members->InsertAt(decl, 0);
                        numMembersFromSuperclass++;
                    }
                } else if (strcmp(declType, "FnDecl") == 0) {
                    if (DEBUG) printf("checking superclass function %s against current class\n", decl->GetName());
                    // Report any subclass variables that conflict with superclass function.
                    VarDecl *var = variables->Lookup(decl->GetName());
                    if (var != NULL) {
                        ReportError::DeclConflict((Decl *) var, decl);
                    }
                    // Report any subclass functions that are mismatched overrides of superclass function.
                    FnDecl* func = functions->Lookup(decl->GetName());
                    if (func != NULL && !func->MatchesSignature((FnDecl*) decl)) {
                        ReportError::OverrideMismatch(func);
                    }
                }
            }
	    
            // Add function declaration from superclass to the subclass.
            // We do this in a separate for loop afterward to prevent the program 
            // from thinking that method conflicts in the superclass are mismatched 
            // overrides (i.e. if superclass mistakenly has two methods named "f," 
            // adding the first one to "functions" as soon as you see it would cause 
            // the program to think the second one was a mismatched override of the
            // first method).
            for (int i = 0; i < superclass->members->NumElements(); i++) {
                Decl *decl = superclass->members->Nth(i);
                const char *declType = decl->GetPrintNameForNode();

                if (strcmp(declType, "FnDecl") == 0) {
                    if (DEBUG) printf("Adding superclass function %s to subclass %s\n", decl->GetName(), GetName());

                    VarDecl *varConflict = variables->Lookup(decl->GetName());

                    // If the subclass attempts to override the superclass method (even 
                    // if it does it wrong), we make sure the subclass method is "above"
                    // the superclass method in the hashtable.
                    FnDecl *subclassMethod = functions->Lookup(decl->GetName());
                    if (subclassMethod != NULL) {
                        functions->Remove(decl->GetName(), subclassMethod);
                    } else if (varConflict == NULL) {
                        // If the function hasn't been overriden and it has no subclass 
                        // variables conflicting with it either, then we add it to
                        // the list of members that the subclass has.
                        members->InsertAt(decl, 0);
                        numMembersFromSuperclass++;
                    }
                    functions->Enter(decl->GetName(), (FnDecl *) decl, false);
                    if (subclassMethod != NULL) {
                        functions->Enter(decl->GetName(), subclassMethod, false);
                    }
                }
		}*/
        }
    }

    // Check to make sure all interfaces are implemented
    if (DEBUG) printf("Doing interface checks for class %s.\n", GetName());
    for (int i = 0; i < implements->NumElements(); i++) {
        NamedType* cur = implements->Nth(i);
        const char *interfaceName = cur->GetName();
        if (DEBUG) printf("\tChecking interface %s.\n", interfaceName);
        InterfaceDecl* interface = SA.hInterfaces.Lookup(interfaceName);
        if (interface == NULL) {
            ReportError::IdentifierNotDeclared(cur->GetIdentifier(), LookingForInterface);
        } else {
            // Make sure all required methods are implemented.
            List<Decl *> *prototypes = interface->GetMembers();
            if (DEBUG) printf("\t\tEnsuring that all required methods are implemented.\n");
            bool alreadyReported = false;
            for (int j = 0; j < prototypes->NumElements(); j++) {
                Decl *prototype = prototypes->Nth(j);
                if (DEBUG) printf("\t\t\tLooking for method %s\n", prototype->GetName());
                FnDecl *implementingFunction = functions->Lookup(prototype->GetName());
                if (implementingFunction == NULL && !alreadyReported) {
                    // If any required method is missing, the interface was not implemented.
                    ReportError::InterfaceNotImplemented((Decl *) this, (Type *) cur);
                    alreadyReported = true; // so we don't double-report
                }
		else if(implementingFunction != NULL  &&  !implementingFunction->MatchesSignature((FnDecl *) prototype)) {
                    // Report any mismatched signatures
                    ReportError::OverrideMismatch(implementingFunction);
		}
            }
        }
    }
    if (DEBUG) printf("Finished with interface checks.\n");



    // Run class through the semantic analyser.
    SA.curScopeNum++;
    SA.AddDecls(members);
    for (int i = 0; i < members->NumElements(); i++) {
        Decl* decl = members->Nth(i);
        decl->CheckSemantics();
    }
    SA.RemoveDecls(members);
    
    // Remove any superclass members that were added to members previously.
    for (int i = 0; i < numMembersFromSuperclass; i++) {
        members->RemoveAt(0);
    }

    ScopedDecl *classScope = SA.curClasses.Lookup(id->GetName());

    SA.curScopeNum--;
    SA.curClass = NULL;
}

/**
 * This assumes that ClassDecl::AddToOffsets has already executed.
 */
void ClassDecl::Emit() {
    if (CGDEBUG) printf("in ClassDecl::Emit\n");
    cg.curClass = this;
    cg.curScopeNum++;

    for (int i = 0; i < members->NumElements(); i++) {
        members->Nth(i)->Emit();
    }

    cg.curScopeNum--;
    cg.curClass = NULL;
    if (CGDEBUG) printf("done with ClassDecl::Emit\n");
}

void ClassDecl::AddToOffsets(ClassDecl *superclass) {
    // First Case: passed 'superclass' is just this class itself.
    // Initialize data structures and offset-tracker variables.
    if (this == superclass) {
        if (CGDEBUG) printf("The AddToOffsets chain starts here, with class %s.\n", this->GetName());
        curVarOffset = 0;
        curMethodOffset = 0;
        varIndices = new Hashtable<vtableEntry *>;
        methodIndices = new Hashtable<vtableEntry *>;
        methodLabels = new List<const char *>;
    }

    // If there is a superclass, add its vars and methods to offsets hashtable first.
    if (superclass->GetExtends() != NULL) {
        if (CGDEBUG) printf("Adding to offsets vtable of class %s\n", superclass->GetName());
        ScopedDecl *lookup = cg.existingDecls.Lookup(superclass->GetExtends()->GetName());
        Assert(lookup != NULL); // semantic analyser should have caught this
        ClassDecl *superduperclass = (ClassDecl*) lookup->decl;
        if (CGDEBUG) printf("\tbut first look at superclass %s.\n", superduperclass->GetName());
        AddToOffsets(superduperclass);
    }
    
    // Now, add this class's vars and methods to the var and method offsets hashtables.
    List<Decl*>* superclassMembers = superclass->GetMembers();
    if (CGDEBUG) printf("Looking through vars and methods of class %s.\n", superclass->GetName());
    for (int i = 0; i < superclassMembers->NumElements(); i++) {
        // if adding a var
        if (strcmp(superclassMembers->Nth(i)->GetPrintNameForNode(), "VarDecl") == 0) {
            VarDecl *curVarDecl = (VarDecl*) superclassMembers->Nth(i);
            vtableEntry *curEntry = new vtableEntry;
            curEntry->name = strdup((string("_") + string(superclass->GetName()) + string(".") + string(curVarDecl->GetName())).c_str());
            curEntry->offset = curVarOffset;

            varIndices->Enter(curVarDecl->GetName(), curEntry, false);
            if (CGDEBUG) printf("\tAdded var %s at offset %d in vtable of class %s.\n", curEntry->name, curVarOffset, this->GetName());
            curVarOffset += CodeGenerator::VarSize;
        }
        // if adding a method
        else if (strcmp(superclassMembers->Nth(i)->GetPrintNameForNode(), "FnDecl") == 0) {
            FnDecl *curFnDecl = (FnDecl*) superclassMembers->Nth(i);
            vtableEntry *curEntry = new vtableEntry;
            curEntry->name = strdup((string("_") + string(superclass->GetName()) + string(".") + string(curFnDecl->GetName())).c_str());
            curEntry->offset = curMethodOffset;

            //if(CGDEBUG) printf("\t\tcurFuncName = %p/%s/%d\n", curEntry, curEntry->name, strlen(curEntry->name));
            // If there is already a method of this name in the vtable, replace it.
            vtableEntry *methodNameLookup = methodIndices->Lookup(curFnDecl->GetName());
            if (methodNameLookup != NULL) {
                curEntry->offset = methodNameLookup->offset;
                // Replace old method label with new method label (curEntry->name)
                int methodLabelIndex = curEntry->offset / CodeGenerator::VarSize;
                methodLabels->RemoveAt(methodLabelIndex);
                methodLabels->InsertAt(curEntry->name, methodLabelIndex);
                if (CGDEBUG) printf("\tReplaced method %s at offset %d in vtable of class %s.\n", curEntry->name, curEntry->offset, this->GetName());
            } else {
                methodLabels->Append(curEntry->name);
                if (CGDEBUG) printf("\tAdded method %s at offset %d in vtable of class %s.\n", curEntry->name, curMethodOffset, this->GetName());
                curMethodOffset += CodeGenerator::VarSize;
            }

            methodIndices->Enter(curFnDecl->GetName(), curEntry, false);
            //if(CGDEBUG) printf("\t\thashresult = %p\n", methodIndices->Lookup(curFnDecl->GetName()));
            //if(CGDEBUG) printf("\t\t\t newcurFuncName = %p/%s/%d\n", curEntry, curEntry->name, strlen(curEntry->name));
        }
    }

    if (CGDEBUG) printf("DONE tracking offsets for class %s in (superclass) %s.\n\n", this->GetName(), superclass->GetName());
    // store the number of member variables and member functions for the base case class
    if(this == superclass) {
	totalMemberVars = curVarOffset / 4;
	totalMemberMethods = curMethodOffset / 4;
    }
}

vtableEntry *ClassDecl::GetMethodVtableEntry(char *methodName) {
    return methodIndices->Lookup(methodName);
}

InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);

    // add prototypes to hashmap functions
    functions = new Hashtable<FnDecl*>;
    for (int i = 0; i < members->NumElements(); i++)
      functions->Enter(members->Nth(i)->GetName(), (FnDecl*)(members->Nth(i)) );
}

void InterfaceDecl::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
    members->PrintAll(indentLevel+1);
}

void InterfaceDecl::CheckSemantics() {
    for (int i = 0; i < members->NumElements(); i++)
        members->Nth(i)->CheckSemantics();
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
    label = NULL;

    // Add formals to list of variables.
    variables = new Hashtable<VarDecl *>;
    for (int i = 0; i < formals->NumElements(); i++) {
        VarDecl *formal = formals->Nth(i);
        variables->Enter(formal->GetName(), formal);
    }
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}

void FnDecl::PrintChildren(int indentLevel) {
    returnType->Print(indentLevel+1, "(return type) ");
    id->Print(indentLevel+1);
    formals->PrintAll(indentLevel+1, "(formals) ");
    if (body) body->Print(indentLevel+1, "(body) ");
}

void FnDecl::CheckSemantics() {
    if (DEBUG) printf("Now checking semantics for function %s\n", GetName());
    SA.curFunc = this;
    
    /*// if this is main, let's make sure its signature + return type are correct.
    if(strcmp(id->GetName(), "main") == 0 && SA.curScopeNum == 0) {
      if(this->GetFormals()->NumElements() > 0)
        ReportError::Formatted(this->GetLocation(), "Main should not have any arguments.");
      if(!Type::intType->IsEquivalentTo(returnType) && !Type::voidType->IsEquivalentTo(returnType))
        ReportError::Formatted(this->GetLocation(), "Main must return an integer.");
	}*/

    // Make sure return type has been declared.
    // TODO make sure function can't have a return type of null (although it *can*
    // return the literal value null)
    bool badReturnType = false;
    Type* toCheck = returnType;
    // If the type is an array type, we want to check whether the array's
    // elemType is legit rather than the type itself.
    if (strcmp(toCheck->GetPrintNameForNode(), "ArrayType") == 0) {
        toCheck = ((ArrayType*) toCheck)->GetElemType();
    }
    // If variable type is not one of the default types, check to make sure  
    // the variable type was declared.
    if (strcmp(toCheck->GetPrintNameForNode(), "NamedType") == 0) {
        NamedType* lookup = (NamedType*) SA.existingNamedTypes.Lookup(toCheck->GetName());
        if (lookup == NULL) {
            badReturnType = true;
        }
    }
    if (badReturnType) {
        ReportError::IdentifierNotDeclared(((NamedType *) toCheck)->GetIdentifier(), LookingForType);
    }

    // Add formals (parameters) to current variables 
    // and check their semantics.
    SA.curScopeNum++;
    if (DEBUG) printf("scope is now %d\n", SA.curScopeNum);
    SA.AddDecls((List<Decl*>*) formals);
    for (int i = 0; i < formals->NumElements(); i++) {
        formals->Nth(i)->CheckSemantics();
    }

    // Check statement block semantics if it exists.
    if (body != NULL) {
        body->CheckSemantics();
    }

    // Remove formals (parameters)
    SA.RemoveDecls((List<Decl*>*) formals);
    SA.curScopeNum--;
    SA.curFunc = NULL;
}

bool FnDecl::MatchesSignature(FnDecl *other) {
    if (DEBUG) printf("seeing if %s matches signature for %s.\n", GetName(), other->GetName());
    // Match return type.
    if (!returnType->IsEquivalentTo(other->GetReturnType())) {
        return false;
    }
    if (DEBUG) printf("\twell, the return types match so far\n");
    // Match number of formals
    List<VarDecl *> *otherFormals = other->GetFormals();
    if (otherFormals->NumElements() != variables->NumEntries()) {
        return false;
    }
    if (DEBUG) printf("\tand so do the number of formals for each function\n");
    // Match name and type of each formal 
    for (int i = 0; i < otherFormals->NumElements(); i++) {
        VarDecl *otherFormal = otherFormals->Nth(i);
        VarDecl *curFormal = formals->Nth(i);
        if (DEBUG) printf("\tnow testing the formals named %s and %s\n", curFormal->GetName(), otherFormal->GetName());
        if (!curFormal->GetType()->IsEquivalentTo(otherFormal->GetType())) {
            return false;
        }
    }
    if (DEBUG) printf("\they, they match!\n");
    return true;
}

void FnDecl::SetLabel() {
    if (CGDEBUG) printf("in FnDecl::SetLabel: generating the label for function %s\n", GetName());
    if (cg.curClass == NULL) {
        label = id->GetName();

        // Prepend an underscore if it's not main.
        if (strcmp(id->GetName(), "main") != 0) {
            label = strdup(("_" + string(label)).c_str());
        }
    } else {
        label = cg.curClass->GetLongMethodName(id->GetName());
    }
    if (CGDEBUG) printf("\tThis is the label: %s\n", label);
}

void FnDecl::Emit() {
    if (CGDEBUG) printf("in FnDecl::Emit for function named %s\n", GetName());

    // Set state-tracking variables in CodeGenerator.
    cg.curNumLocals = 0;

    // Set cgLocations of formals.
    // insert curClass if it exists as a formal
    for (int i = 0; i < formals->NumElements(); i++) {
        VarDecl *cur = formals->Nth(i);
        cur->SetCgLoc(new Location(fpRelative, CodeGenerator::OffsetToFirstParam + CodeGenerator::VarSize * (i + (cg.curClass != NULL ? 1 : 0)), cur->GetName()));
    }

    // Add formals to set of existing variables
    cg.curScopeNum++;
    cg.AddDecls((List<Decl*>*) formals);

    // Emit the function.
    cg.GenLabel(label);
    BeginFunc *fn = cg.GenBeginFunc();
    body->Emit();
    fn->SetFrameSize(cg.curNumLocals * CodeGenerator::VarSize);
    cg.GenEndFunc();

    // Remove formals from set of existing variables
    cg.RemoveDecls((List<Decl*>*) formals);
    cg.curScopeNum--;

    // Restore previous values of state-tracking variables in CodeGenerator.
    cg.curNumLocals = 0;
}
