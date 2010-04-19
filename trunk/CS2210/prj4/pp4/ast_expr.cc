/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>
#include "semantic_analyzer.h"
#include "codegen.h"

extern SemanticAnalyzer SA;
extern CodeGenerator cg;

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
    resultType = Type::intType;
}

void IntConstant::PrintChildren(int indentLevel) { 
    printf("%d", value);
}

void IntConstant::Emit() {
    if (CGDEBUG) printf("IntConstant::Emit: value is %d\n", value);
    cgLoc = cg.GenLoadConstant(value);
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
    resultType = Type::doubleType;
}

void DoubleConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
    resultType = Type::boolType;
}

void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

void BoolConstant::Emit() {
    int intVal = value ? 1 : 0;
    cgLoc = cg.GenLoadConstant(intVal); // TODO can this just be cg.GenLoadConstant(value)
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
    resultType = Type::stringType;
}

void StringConstant::PrintChildren(int indentLevel) { 
    printf("%s",value);
}

void StringConstant::Emit() {
    cgLoc = cg.GenLoadConstant(value);
}

void NullConstant::Emit() {
    cgLoc = cg.GenLoadConstant(0);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   right->Print(indentLevel+1);
}

void CompoundExpr::CheckSemantics() {
    // TODO more specific checks involving the operator
    if (left)
        left->CheckSemantics();
    right->CheckSemantics();
}

void CompoundExpr::Emit() {
    if (CGDEBUG) printf("in CompoundExpr::Emit\n");

    // Assume at first that the left/right/operator expressions for code 
    // generation are just those of this CompoundExpr object.
    Expr *leftExpr = left;
    Expr *rightExpr = right;
    char *opTokenString = op->GetTokenString();

    // Take care of binary operators not covered by TAC.
    RelationalExpr *lt = new RelationalExpr(left, new Operator(*op->GetLocation(), "<"), right);
    EqualityExpr *eq = new EqualityExpr(left, new Operator(*op->GetLocation(), "=="), right);
    if (strcmp(opTokenString, "<=") == 0) {
        leftExpr = lt;
        rightExpr = eq;
        opTokenString = (char *) "||";
    // For the following cases, make the left expression its opposite, and 
    // subtract one from the result to get the inverse boolean value.
    } else if (strcmp(opTokenString, ">") == 0) {
        leftExpr = new LogicalExpr(lt, new Operator(*op->GetLocation(), "||"), eq);
        opTokenString = (char *) "-";
        rightExpr = new IntConstant(*right->GetLocation(), 1);
    } else if (strcmp(opTokenString, ">=") == 0) {
        leftExpr = lt;
        opTokenString = (char *) "-";
        rightExpr = new IntConstant(*right->GetLocation(), 1);
    } else if (strcmp(opTokenString, "!=") == 0) {
        leftExpr = eq;
        opTokenString = (char *) "-";
        rightExpr = new IntConstant(*right->GetLocation(), 1);
    }

    // Emit left and right expressions, and generate binary operation.
    leftExpr->Emit();
    rightExpr->Emit();
    cgLoc = cg.GenBinaryOp(opTokenString, leftExpr->GetCgLoc(), rightExpr->GetCgLoc());
    if (CGDEBUG) printf("finished CompoundExpr::Emit\n");
}

void ArithmeticExpr::CheckSemantics() {
  if(DEBUG)  printf("Checking arithmetic expr semantics.\n");
  if(left != NULL)
    left->CheckSemantics();
  right->CheckSemantics();
  if((left != NULL && left->isResultError()) || right->isResultError()) {
    resultType = Type::errorType;
    return;
  }
  
  char *rightName = right->GetResultType()->GetName();
  if(left == NULL) {
    if(strcmp(rightName, Type::intType->GetName()) != 0
       && strcmp(rightName, Type::doubleType->GetName()) != 0) {
      ReportError::IncompatibleOperand(op, right->GetResultType());
      resultType = Type::errorType;
      return;
    }
  }
  else {
    char *leftName = left->GetResultType()->GetName();
    if(DEBUG)  printf("left's type name: %s\n", leftName);
    if(DEBUG)  printf("right's type name: %s\n", rightName);
    if( !( strcmp(leftName, rightName) == 0
	   && ( strcmp(leftName, Type::intType->GetName())==0
		|| strcmp(leftName, Type::doubleType->GetName())==0 ) ) ) {
          ReportError::IncompatibleOperands(op, left->GetResultType(), right->GetResultType());
	  resultType = Type::errorType;
	  return;
	}
  }
  // if no errors:
  resultType = right->GetResultType();
}

void ArithmeticExpr::Emit() {
    if (CGDEBUG) printf("in ArithmeticExpr::Emit\n");

    // Unary minus case
    if (left == NULL) {
        right->Emit();
        Location *zero = cg.GenLoadConstant(0);
        cgLoc = cg.GenBinaryOp("-", zero, right->GetCgLoc());
    } else {
        CompoundExpr::Emit();
    }
}

PostfixExpr::PostfixExpr(Operator *o, Expr *l) 
  : Expr(Join(l->GetLocation(), o->GetLocation())) {
    Assert(o != NULL && l != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this);
}

void PostfixExpr::PrintChildren(int indentLevel) {
  left->Print(indentLevel+1);
  op->Print(indentLevel+1);
}

void PostfixExpr::CheckSemantics() {
  left->CheckSemantics();
  if(left->isResultError()) {
    resultType = Type::errorType;
    return;
  }

  char *leftName = left->GetResultType()->GetName();
  if(strcmp(leftName, Type::intType->GetName()) != 0
     && strcmp(leftName, Type::doubleType->GetName()) != 0) {
      ReportError::IncompatibleOperand(op, left->GetResultType());
      resultType = Type::errorType;
      return;
  }
  // if operand is compatible:
  resultType = left->GetResultType();
}

void RelationalExpr::CheckSemantics() {
  left->CheckSemantics();
  right->CheckSemantics();
  if(left->isResultError() || right->isResultError()) {
    resultType = Type::errorType;
    return;
  }

  char *leftName = left->GetResultType()->GetName();
  char *rightName = right->GetResultType()->GetName();
  if( !( strcmp(leftName, rightName) == 0
	 && ( strcmp(leftName, Type::intType->GetName())==0
	      || strcmp(leftName, Type::doubleType->GetName())==0 ) ) ) {
    ReportError::IncompatibleOperands(op, left->GetResultType(), right->GetResultType());
    resultType = Type::errorType;
    return;
	}
  // if compatible:
  resultType = Type::boolType;
}

void EqualityExpr::CheckSemantics() {
  left->CheckSemantics();
  right->CheckSemantics();
  if (left->isResultError() || right->isResultError()) {
    resultType = Type::errorType;
    return;
  }

  // determine whether the lhs and rhs are of the same type, by using the appropriate IsEquivalentTo function
  if (left->GetResultType()->IsEquivalentTo(right->GetResultType())) {
    resultType = Type::boolType;
  } else {
    ReportError::IncompatibleOperands(op, left->GetResultType(), right->GetResultType());
    resultType = Type::errorType;
  }
}

/**
 * Needs own Emit() function instead of just falling to 
 * CompoundExpr::Emit() because of need to call built-in StringEqual
 * comparison function.
 */ void EqualityExpr::Emit() {
    if (left->GetResultType()->IsEquivalentTo(Type::stringType) &&
            right->GetResultType()->IsEquivalentTo(Type::stringType)) {
        left->Emit();
        right->Emit();
        cgLoc = cg.GenBuiltInCall(StringEqual, left->GetCgLoc(), right->GetCgLoc());
    } else {
        CompoundExpr::Emit();
    }
}

void LogicalExpr::CheckSemantics() {
  resultType = Type::boolType;  // no matter what, we report this as a boolean expression

  if(DEBUG) {
    printf("\nChecking LogicalExpr semantics. Printing them:");
    if(left != NULL)
      left->Print(0);
    right->Print(0);
  }
  if(left != NULL)
    left->CheckSemantics();
  right->CheckSemantics();
  if((left != NULL && left->isResultError()) || right->isResultError()) {
    //    resultType = Type::errorType;
    return;
  }
  
  bool isRightBool = (strcmp(right->GetResultType()->GetName(), Type::boolType->GetName()) == 0);
  if(DEBUG) printf("\nIs the rhs a bool (as required)? %d", isRightBool);
  if(left != NULL) {
    if(!isRightBool  ||  strcmp(left->GetResultType()->GetName(), Type::boolType->GetName()) != 0) {
      ReportError::IncompatibleOperands(op, left->GetResultType(), right->GetResultType());
      //      resultType = Type::errorType;
      return;
    }
  }
  else if(!isRightBool) {
    ReportError::IncompatibleOperand(op, right->GetResultType());
    //    resultType = Type::errorType;
    return;
  }
  
  // but if both left and right are bools:
  resultType = Type::boolType;
}

void LogicalExpr::Emit() {
    if (CGDEBUG) printf("in LogicalExpr::Emit\n");

    // Unary '!' case
    if (left == NULL) {
        right->Emit();
        Location *one = cg.GenLoadConstant(1);
        cgLoc = cg.GenBinaryOp("-", right->GetCgLoc(), one);
    } else {
        CompoundExpr::Emit();
    }
}

void This::CheckSemantics() {
    if (DEBUG) printf("Now checking a reference to 'this'\n");
    // Make sure 'this' is used within the scope of some class.
    if (SA.curClass == NULL) {
        ReportError::ThisOutsideClassScope(this);
        resultType = Type::errorType;
    } else {
        NamedType *typeLookup = SA.existingNamedTypes.Lookup(SA.curClass->GetName());
        Assert(typeLookup != NULL); // if the ClassDecl exists in SA, so should the NamedType
        if (DEBUG) printf("'this' currently refers to the type %s\n", typeLookup->GetName());
        resultType = typeLookup;
    }
    if (DEBUG) printf("Finished checking the 'this' reference.\n");
}

void This::Emit() {
    cgLoc = new Location(fpRelative, CodeGenerator::OffsetToFirstParam, "this");
}

ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);

    // This is relevant for code generation:
    //  By default, we assume the array access is happening on the RHS
    isLHS = false;
    offset = 0;
    isAMemberVariable = false;
}

void ArrayAccess::PrintChildren(int indentLevel) {
  base->Print(indentLevel+1);
  subscript->Print(indentLevel+1, "(subscript) ");
}

void ArrayAccess::CheckSemantics() {
    if (DEBUG) printf("Now checking ArrayAccess semantics.\n");

    base->CheckSemantics();
    subscript->CheckSemantics();

    if(base->isResultError() || subscript->isResultError()) {
        resultType = Type::errorType;
        return;
    }

    if (DEBUG) printf("\tno internal errors, so we check if base and subscript are legit\n");

    // base must be of ArrayType
    if (strcmp(base->GetResultType()->GetPrintNameForNode(), "ArrayType") != 0) {
        if (DEBUG) printf("here\n");
        resultType = Type::errorType;
        ReportError::BracketsOnNonArray(base);
        return;
    }

    if (DEBUG) printf("there\n");
    // subscript must be an integer
    if(strcmp(subscript->GetResultType()->GetName(), Type::intType->GetName()) != 0) {
        resultType = Type::errorType;
        ReportError::SubscriptNotInteger(subscript);
        return;
    }

    resultType = ((ArrayType*)(base->GetResultType()))->GetElemType();
    if (DEBUG) printf("finished checking ArrayAccess semantics.\n");
}

void ArrayAccess::Emit() {
    if (CGDEBUG) printf("in ArrayAccess::Emit\n");
    if (CGDEBUG) printf("ArrayAccess::Emit: emitting base. Note that isLHS = %d\n", isLHS);
    base->SetIsLHS(isLHS);
    base->Emit();
    Location *baseLoc = base->GetCgLoc();

    if (CGDEBUG) printf("ArrayAccess::Emit: emitting subscript\n");
    subscript->Emit();
    
    // check array out-of-bounds
    if(CGDEBUG) printf("ArrayAccess::Emit: emitting array-out-of-bounds check\n");
    Location *zero = cg.GenLoadConstant(0);
    Location *subscriptLessThanZero = cg.GenBinaryOp("<", subscript->GetCgLoc(), zero);
    Location *arrayLength = cg.GenLoad(baseLoc, -CodeGenerator::VarSize);
    Location *subscriptLessThanLength = cg.GenBinaryOp("<", subscript->GetCgLoc(), arrayLength);
    Location *subscriptNotLessThanLength = cg.GenBinaryOp("==", zero, subscriptLessThanLength);
    Location *invalidSubscript = cg.GenBinaryOp("||", subscriptNotLessThanLength, subscriptLessThanZero);
    char *afterCheck = cg.NewLabel();
    cg.GenIfZ(invalidSubscript, afterCheck);
    
    // array access is out-of-bounds, so report this
    Location *errorMsg = cg.GenLoadConstant(err_arr_out_of_bounds);
    cg.GenBuiltInCall(PrintString, errorMsg);
    cg.GenBuiltInCall(Halt);
    
    // if the array access is in-bounds, we arrive here
    if(CGDEBUG) printf("ArrayAccess::Emit: emit instructions to (1) compute offset and (2) add  baseLoc + offset.\n");
    cg.GenLabel(afterCheck);
    Location *numBytesOffset = cg.GenBinaryOp("*", subscript->GetCgLoc(), cg.GenLoadConstant(CodeGenerator::VarSize));
    cgLoc = cg.GenBinaryOp("+", baseLoc, numBytesOffset);

    // if this access is not happening on the last operation of an LHS, load the 4 bytes at this address now
    if (!(isLHS && strcmp(GetParent()->GetPrintNameForNode(), "AssignExpr") == 0)) {
        if (CGDEBUG) printf("ArrayAccess::Emit:  this array access is NOT the right-most operation of a LHS, so dereference:  cgLoc = *(baseLoc+offset)\n");
        cgLoc = cg.GenLoad(cgLoc);
    } else
        if (CGDEBUG) printf("ArrayAccess::Emit:  this array access IS the right-most operation of a LHS, so DON'T dereference. Keep  cgLoc = baseLoc + offset\n");
}

FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);

    // This is relevant for code generation:
    //  By default, we assume the field access is happening on the RHS
    offset = 0;
    isLHS = false;
    isAMemberVariable = false;
}

void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
}

void FieldAccess::Emit() {
    if (CGDEBUG) printf("in FieldAccess:Emit\n");
    if (base == NULL) {
        if (CGDEBUG) printf("FieldAccess::Emit: base is null. trying to lookup %s\n", field->GetName());
        if (cg.curClass == NULL) {
            if (CGDEBUG) printf("\twe're not inside a class, so just looking in existingDecls\n");
            // This is just a regular variable that we should look up in the set of existing declarations.
            ScopedDecl *lookup = cg.existingDecls.Lookup(field->GetName());
            Assert(lookup != NULL); // semantic checking should have caught this
            if (CGDEBUG) printf("\tfound %s\n", lookup->decl->GetName());
            cgLoc = lookup->decl->GetCgLoc();
            if (CGDEBUG && lookup->decl->GetCgLoc() == NULL) printf("\tbut it has no location set!!!\n");
        } else {
            if (CGDEBUG) printf("FieldAccess::Emit: in a class, so %s could be a local variable or a member variable\n", field->GetName());
            // We're currently inside a class, so this field could either be a local
            // variable in a method, or a member variable in the class.
            ScopedDecl *lookup = cg.existingDecls.Lookup(field->GetName());
            if (lookup != NULL) {
                if (CGDEBUG) printf("\tit's a local variable!\n");
                // This is a local variable, so we just get the cgLoc.
                cgLoc = lookup->decl->GetCgLoc();
            } else {
                if (CGDEBUG) printf("\tit's a member variable!\n");
                // This is a member variable of the class that we're currently inside.
                // We set the cgLoc to 'this,' which is the first parameter passed in
                // the method call, and set the offset to the member's offset from the
                // object address.
                cgLoc = new Location(fpRelative, CodeGenerator::OffsetToFirstParam, "this");
                offset = cg.curClass->GetVarOffset(field->GetName());
                isAMemberVariable = true;

                // Dereference the variable if it's the rightmost operation of an LHS.
                if (!(isLHS && strcmp(GetParent()->GetPrintNameForNode(), "AssignExpr") == 0)) {
                    if (CGDEBUG) printf("\twe're dereferencing this member variable (b/c we're not at the rightmost end of a LHS): %s!\n", field->GetName());
                    cgLoc = cg.GenLoad(cgLoc, offset);
                } else
                    if (CGDEBUG) printf("\tdon't dereference this member variable (b/c we're at the rightmost end of a LHS:  %s\n", field->GetName());
            }
        }
    } else {
        // Base is an object.
        if (CGDEBUG) printf("Base is not NULL. (field=%s)\n", field->GetName());
        isAMemberVariable = true;

        // Let's first find its class.
        if (CGDEBUG) printf("\t(field = %s), base's resulttype ptr = %p\n", field->GetName(), base->GetResultType());
        char *className = base->GetResultType()->GetName();
        ScopedDecl *lookup = cg.existingDecls.Lookup(className);
        Assert(lookup != NULL);  // the class className must exist
        ClassDecl *classDecl = (ClassDecl *) lookup->decl;

        // Now let's get the offset of the field from the base object's address.
        offset = classDecl->GetVarOffset(field->GetName());
        if (CGDEBUG) printf("\t offset of field %s from base is %d\n", field->GetName(), offset);

        // Emit the base so the cgLoc is set.
        if (CGDEBUG) printf("\t emitting base.\n");
        base->Emit();

        // Now we can emit instructions. There are 2 cases: the field access is either the right-most operation of an LHS, or not.
        if (!(isLHS && strcmp(GetParent()->GetPrintNameForNode(), "AssignExpr") == 0)) {
            cgLoc = cg.GenLoad(base->GetCgLoc(), offset);
            if (CGDEBUG) printf("\t access of field %s is NOT right-most op of an LHS, so we dereference here.\n", field->GetName());
        } else {
            cgLoc = base->GetCgLoc();
            if (CGDEBUG) printf("\t access of field %s IS a right-most op of an LHS, so DON'T dereference here.\n", field->GetName());
        }
    }
}

void FieldAccess::CheckSemantics() {
    if (DEBUG) printf("Now checking FieldAccess with name %s (could also have a base).\n", field->GetName());

    if (DEBUG) printf("\tabout to check if base is null.\n");
    if (base == NULL) {   /// CHRIS YOU SAID YOU WERE JUST WORKING ON THIS CLAUSE  !!!!!!!!!!!!!!!!
        ScopedDecl *var = SA.curVars.Lookup(field->GetName());
        if (DEBUG) printf("\tbase was null.\n");
        // TODO need to run a test to make sure this scope check is working as intended
        if (var != NULL && var->scopeNumber <= SA.curScopeNum) {
            if (DEBUG) printf("\tvar was found.\n");
            if (DEBUG) printf("\tname of var is %s\n", var->decl->GetName());
	    // TODO if var exists but it is of an undeclared type, then mark this as errorType
            resultType = ((VarDecl *) var->decl)->GetType();
            if (DEBUG) printf("\tvar type is %s\n", resultType->GetName());
        } else {
            if (DEBUG) printf("\tno matching var was found\n");
            resultType = Type::errorType;
            ReportError::IdentifierNotDeclared(field, LookingForVariable);
            if (DEBUG) printf("\treturn type was error\n");
        }
    } else {   /// USE THE FOLLOWING CODE BY ME (PV)
        if (DEBUG) printf("\tbase is not null.\n");
        base->CheckSemantics();
        if (DEBUG) printf("\tchecked internal semantics of base\n");
        if (base->isResultError()) {
            if (DEBUG) printf("\tbase has internal errors\n");
            resultType = Type::errorType;
        } else {
            if (DEBUG) printf("\tbase has no internal errors\n");
            // base isn't null. so first find what class the base is an object of.
            /* Shouldn't need this code anymore because base will check itself.
            char *baseTypeName = base->GetResultType()->GetName();
            ScopedDecl *baseClass = SA.curClasses.Lookup(baseTypeName);
            if(baseClass == NULL) {
                resultType = Type::errorType;
                ReportError::Formatted(base->GetLocation(), "This expression does not evaluate to a class object.");
                return;
            }
            */
            // the base is indeed an object.  let's check if its class is legit.
            Type *baseType = base->GetResultType();
            if (DEBUG) printf("\tbase is of type %s\n", baseType->GetName());
            ScopedDecl *classLookup = SA.curClasses.Lookup(baseType->GetName());
            if (classLookup == NULL) {
                // base's class doesn't exist - we don't report this, since 
                // it should have already been reported at time of base's VarDecl
	      // PV:  ACTUALLY we do have to report this. in case of something like  int pens[];  pens.color = 3;
                resultType = Type::errorType;
		ReportError::FieldNotFoundInBase(field, base->GetResultType());
            } else {
                if (DEBUG) printf("\tfound the type %s, now looking for member field %s\n", baseType->GetName(), field->GetName());
                // let's search for the field in the member variables of the class.
                Hashtable<VarDecl*>* classVars = ((ClassDecl *) classLookup->decl)->GetVarDeclsHashmap();
                VarDecl* memberVar = classVars->Lookup(field->GetName());
                if(memberVar == NULL) {
                    resultType = Type::errorType;
                    ReportError::FieldNotFoundInBase(field, base->GetResultType());
                } else {
                  // this field access is only valid if we are in the class or a subclass
		  if(SA.curClass == NULL) {
		    resultType = Type::errorType;
		    ReportError::InaccessibleField(field, base->GetResultType());
		  }
		  else { // at least we are in a class. now let's see if field is actually a member variable in curClass
		    Hashtable<VarDecl*>* curClassVars = SA.curClass->GetVarDeclsHashmap();
		    VarDecl *varInCurClassLookup = curClassVars->Lookup(field->GetName());
		    if(varInCurClassLookup == NULL) {
		      resultType = Type::errorType;
		      ReportError::InaccessibleField(field, base->GetResultType());
		    }
		    else  // we've found the member variable! extract the type
		      resultType = memberVar->GetType();
		  }
		}
            }
        }
    }
    if (DEBUG) printf("finished FieldAccess check\n");
}

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

void Call::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
    actuals->PrintAll(indentLevel+1, "(actuals) ");
}

void Call::CheckSemantics() {
    if (DEBUG) printf("now checking semantics for Call with field %s\n", field->GetName());
    // before doing anything else, checksemantics of the actuals (parameter list)
    for(int i=0; i<actuals->NumElements(); i++)
      actuals->Nth(i)->CheckSemantics();

    FnDecl* targetFnDecl;

    // do this in 2 stages. (1) check whether there is a function of the
    // required name. (2) check whether the signature matches.

    // stage (1) - note that if an error comes up, we return immediately, to avoid stage (2)
    if (base == NULL) {
        ScopedDecl *func = SA.curFuncs.Lookup(field->GetName());
        // TODO need to run a test to make sure this scope check is working as intended
        if (func != NULL && func->scopeNumber <= SA.curScopeNum) {
	  targetFnDecl = (FnDecl*)func->decl;
	  resultType = targetFnDecl->GetReturnType();
        } else {
            ReportError::IdentifierNotDeclared(field, LookingForFunction);
            resultType = Type::errorType;
	    return;
        }
    } else {
      // TODO if base is of type ArrayType, then field can be length if List is empty 
      if (DEBUG) printf("\tcalling CheckSemantics on the base\n");
        base->CheckSemantics();
        if (base->isResultError()) {
            resultType = Type::errorType;
	    return;
        } else {
            if (DEBUG) printf("\tbase has no errors, we continue\n");
            // This assumes base is some variable, itself without a base.
            // TODO the case where base could be another Call that returns an object
            // or some FieldAccess that itself has a base
            // or does precedence/associativity of calls already take care of this?
            Type *baseType = base->GetResultType();
            
	    if(strcmp(baseType->GetPrintNameForNode(), "ArrayType") == 0) {
	      // if base is an array, field can be length()
	      if(strcmp(field->GetName(), "length")==0 && actuals->NumElements()==0)
		resultType = Type::intType;
	      else {
		ReportError::FieldNotFoundInBase(field, baseType);
		resultType = Type::errorType;
	      }
	      return;
	    }
	    
	    ScopedDecl *classLookup = SA.curClasses.Lookup(baseType->GetName());
            InterfaceDecl *interfaceLookup = SA.hInterfaces.Lookup(baseType->GetName());
	    if (classLookup != NULL || interfaceLookup != NULL) {
	        Hashtable<FnDecl *> *fns;
		// note we can't have an interface and class of the same name
		if(classLookup != NULL)
		  fns = ((ClassDecl *) classLookup->decl)->GetFnDeclsHashmap();
		else
		  fns = interfaceLookup->GetFnDeclsHashmap();
		// match the identifier to an actual function within the class/interface
		targetFnDecl = fns->Lookup(field->GetName());
		if (targetFnDecl == NULL) {
		  resultType = Type::errorType;
		  ReportError::FieldNotFoundInBase(field, baseType);
		  return;
		}
            }
	    else {
                // base's class/interface doesn't exist - we don't report this, since 
                // it should have already been reported at time of base's VarDecl
	        // ** note: we don't report it UNLESS the type is a primitive.
                resultType = Type::errorType;
		if(strcmp(baseType->GetPrintNameForNode(), "Type") == 0) // as opposed to a NamedType
		  ReportError::FieldNotFoundInBase(field, baseType);
		return;
            }
        }
    }
    
    // stage (2) - match the actuals with the formals
    List<VarDecl *> *formals = targetFnDecl->GetFormals();
    if (formals->NumElements() != actuals->NumElements()) {
      ReportError::NumArgsMismatch(field, formals->NumElements(), actuals->NumElements());
      resultType = Type::errorType;
      return;
    } else {
      bool matches = true;
      for (int i = 0; i < actuals->NumElements(); i++) {
	Expr *actual = actuals->Nth(i);
	VarDecl *formal = formals->Nth(i);
	if (actual->isResultError()) {
	  matches = false;
	  continue;
	}
	Type *actualType = actual->GetResultType();
	Type *formalType = formal->GetType();
	if (!formalType->IsEquivalentTo(actualType)) {
	  ReportError::ArgMismatch(actual, i+1, actualType, formalType);
	  matches = false;
	}
      }
      if(matches)
	resultType = targetFnDecl->GetReturnType();
      else
	resultType = Type::errorType;
    }
}

void Call::Emit() {
    if (CGDEBUG) printf("in Call::Emit\n");
    if (CGDEBUG) printf("Call::Emit: the method name is %s\n", field->GetName());

    // Emit actuals so their cgLocs are set.  Note that this is done
    // from left to right.
    if (CGDEBUG) printf("Call::Emit: emitting actuals\n");
    for (int i = 0; i < actuals->NumElements(); i++) {
        if (CGDEBUG) printf("Call::Emit: actual at index %d is node type %s\n", i, actuals->Nth(i)->GetPrintNameForNode());
        actuals->Nth(i)->Emit();
    }

    // Grab the matching function declaration.
    // If this is a call on an object, also grab the address of the function 
    // within the object vtable.
    FnDecl *fnDecl;
    Location *objectLoc = NULL;
    Location *fnAddr = NULL;
    if (base == NULL) {
        if (CGDEBUG) printf("Call::Emit: this function call has no base\n");
        if (cg.curClass == NULL) {
            if (CGDEBUG) printf("\twe're not inside a class, so just looking in existingDecls\n");
            // This is just a regular function that we should look up in the set of existing declarations.
            ScopedDecl *lookup = cg.existingDecls.Lookup(field->GetName());
            Assert(lookup != NULL); // semantic analyzer should have caught this
            fnDecl = (FnDecl *) lookup->decl;
        } else {
            if (CGDEBUG) printf("\twe're currently inside a class, so we check if it's a global function or class method.\n");
            // We're currently inside a class, so this function could either
            // be a global function or a class method.
            ScopedDecl *lookup = cg.existingDecls.Lookup(field->GetName());
            if (lookup != NULL) {
                if (CGDEBUG) printf("\tit's a global function!\n");
                // This is a global function, so we just get the FnDecl.
                fnDecl = (FnDecl *) lookup->decl;
            } else {
                if (CGDEBUG) printf("\tit's a class method!\n");
                // This is a class method.
                ClassDecl *classDecl = cg.curClass;

                // Get the function declaration from the class.
                fnDecl = (FnDecl *) classDecl->GetFnDeclsHashmap()->Lookup(field->GetName());
                Assert(fnDecl != NULL);

                // Grab the function address within the object vtable.
                if (CGDEBUG) printf("\tcalculating the address of the method\n");
                objectLoc = new Location(fpRelative, CodeGenerator::OffsetToFirstParam, "this");
                Location *vtableLoc = cg.GenLoad(objectLoc);
                int methodOffset = classDecl->GetMethodOffset(field->GetName());
                fnAddr = cg.GenLoad(vtableLoc, methodOffset);
            }
        }
    } else {
        if (CGDEBUG) printf("Call::Emit: this function call has a base\n");
        if (CGDEBUG) printf("Call::Emit: emitting the base\n");
        // Emit the base so its cgLoc is set.
        base->Emit();

        // If the base is of ArrayType and method name is length, handle 
        // this specially: Set the cgLoc to the address of the length
        // variable in the array object.
        char *className = base->GetResultType()->GetName();
        if (CGDEBUG) printf("base is of type %s\n", className);
        if (strcmp(className, "ArrayType") == 0 &&
                strcmp(field->GetName(), "length") == 0) {
            cgLoc = cg.GenLoad(base->GetCgLoc(), -CodeGenerator::VarSize);
            return;
        } else {
            // Grab the class declaration of the base object.
            ScopedDecl *lookup = cg.existingDecls.Lookup(className);
            Assert(lookup != NULL);
            ClassDecl *classDecl = (ClassDecl *) lookup->decl;

            // Get the function declaration from the class.
            fnDecl = (FnDecl *) classDecl->GetFnDeclsHashmap()->Lookup(field->GetName());
            Assert(fnDecl != NULL);

            // Grab the function address within the object vtable.
            objectLoc = base->GetCgLoc();
            Location *vtableLoc = cg.GenLoad(objectLoc);
            int methodOffset = classDecl->GetMethodOffset(field->GetName());
            fnAddr = cg.GenLoad(vtableLoc, methodOffset);
        }
    }
    
    // Figure out if the function has a return value.
    if (CGDEBUG) printf("Call::Emit: figuring out return value of function\n");
    bool hasReturnValue = true;
    if (fnDecl->GetReturnType()->IsEquivalentTo(Type::voidType)) {
        hasReturnValue = false;
    }

    // Push parameters onto stack.  Note that we iterate right to left 
    // per Decaf convention, so that the first argument is pushed last.
    if (CGDEBUG) printf("Call::Emit: pushing parameters\n");
    for (int i = actuals->NumElements() - 1; i >= 0; i--) {
        cg.GenPushParam(actuals->Nth(i)->GetCgLoc());
    }
    int numParams = actuals->NumElements();
    // If there is an object location set, that means it's a class method,
    // so we push the object reference onto the stack as an implicit parameter.
    if (objectLoc != NULL) {
        cg.GenPushParam(objectLoc);
        numParams++;
    }

    // Make the LCall or ACall depending on whether there's a base (implicit or not)
    if (objectLoc == NULL) {
        Assert(fnDecl->GetLabel() != NULL);
        cgLoc = cg.GenLCall(fnDecl->GetLabel(), hasReturnValue);
    } else {
        cgLoc = cg.GenACall(fnAddr, hasReturnValue);
    }

    // Pop parameters from stack.
    cg.GenPopParams(numParams * CodeGenerator::VarSize);
}

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}

void NewExpr::PrintChildren(int indentLevel) {	
    cType->Print(indentLevel+1);
}

void NewExpr::CheckSemantics() {
  // check to make sure NamedType* cType is the name of a declared class
  ScopedDecl* curClass = SA.curClasses.Lookup(cType->GetName());
  if(curClass == NULL) {
    ReportError::IdentifierNotDeclared(new Identifier(*(cType->GetLocation()), cType->GetName()), LookingForClass);
    resultType = Type::errorType;
  }
  else
    resultType = cType;
}

void NewExpr::Emit() {
    if (CGDEBUG) printf("in NewExpr::Emit\n");

    // Get the ClassDecl associated with the namedtype cType,
    // and extract its members.
    // TODO think about whether a name conflict with a non-class is possible; if so, we need to maintain a separate hashtable for classdecls
    ScopedDecl *lookup = cg.existingDecls.Lookup(cType->GetName());
    Assert(lookup != NULL); // semantic analyser should have covered this
    ClassDecl *classDecl = (ClassDecl *) lookup->decl;
    List<Decl *> *members = classDecl->GetMembers();

    // We need to allocate space for all the member variables, plus
    // one for the pointer to the vtable.
    Location *numBytes = cg.GenLoadConstant(CodeGenerator::VarSize * (classDecl->GetTotalMemberVars() + 1));

    // Allocate space for the object.
    if (CGDEBUG) printf("NewExpr::Emit: allocating built-in call\n");
    Location *allocAddress = cg.GenBuiltInCall(Alloc, numBytes);
    cgLoc = allocAddress;

    // Set up object's pointer to class vtable.
    Location *vtablePtr = cg.GenLoadLabel(classDecl->GetName());
    cg.GenStore(allocAddress, vtablePtr);

    if (CGDEBUG) printf("done with NewExpr::Emit\n");
}

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

void NewArrayExpr::PrintChildren(int indentLevel) {
    size->Print(indentLevel+1);
    elemType->Print(indentLevel+1);
}

void NewArrayExpr::CheckSemantics() {
    if (DEBUG) printf("Now checking new array expression.\n");
    size->CheckSemantics();
    if (DEBUG) printf("\tCalled size->GetSemantics()\n");
    if(strcmp(size->GetResultType()->GetName(), Type::intType->GetName()) != 0) {
        if (DEBUG) printf("\tsize is not an integer.\n");
        ReportError::NewArraySizeNotInteger(size);
        resultType = Type::errorType;
    } else {
        // check that type exists
        if (DEBUG) printf("\tsize is legit, now checking type\n");
        bool typeGood = false;
        if (strcmp(elemType->GetPrintNameForNode(), "NamedType") == 0) {
            NamedType *namedType = SA.existingNamedTypes.Lookup(elemType->GetName());
            if(namedType == NULL) {
                if (DEBUG) printf("\tnot a valid type!\n");
                ReportError::IdentifierNotDeclared(((NamedType *) elemType)->GetIdentifier(), LookingForType);
                resultType = Type::errorType;
            } else {
                typeGood = true;
            }
        /*
        } else if (strcmp(elemType->GetPrintNameForNode(), "ArrayType") == 0) {
            // TODO
        */
        } else {
            typeGood = true;
        }
        if (typeGood) {
            if (DEBUG) printf("\ttype %s is legit, now creating resultType\n", elemType->GetName());
            resultType = new ArrayType(*(GetLocation()), elemType);
        }
    }
    if (DEBUG) printf("finished new array expression check\n");
}

void NewArrayExpr::Emit() {
    size->Emit();
    Location *lessThanOne = cg.GenBinaryOp("<", size->GetCgLoc(), cg.GenLoadConstant(1));
    char *afterSizeCheck = cg.NewLabel();
    cg.GenIfZ(lessThanOne, afterSizeCheck); // if size >= 1, then skip the error msg
    
    // print error message if size < 1
    Location *errorMsg = cg.GenLoadConstant(err_arr_bad_size);
    cg.GenBuiltInCall(PrintString, errorMsg);
    cg.GenBuiltInCall(Halt);
    
    // instructions for declaring a new array
    // Note that we add one to size for storage of array length
    cg.GenLabel(afterSizeCheck);
    Location *sizePlus1 = cg.GenBinaryOp("+", size->GetCgLoc(), cg.GenLoadConstant(1));
    Location *four = cg.GenLoadConstant(CodeGenerator::VarSize);
    Location *totalSizeInBytes = cg.GenBinaryOp("*", sizePlus1, four);
    Location *allocAddress = cg.GenBuiltInCall(Alloc, totalSizeInBytes);
    cg.GenStore(allocAddress, size->GetCgLoc());

    // Add four to location so you skip over the length and 
    // start at the elements.
    cgLoc = cg.GenBinaryOp("+", allocAddress, four);
}

void AssignExpr::CheckSemantics() {
    if (DEBUG) printf("Now checking an assignment expression.\n");
    if (DEBUG) printf("checking the left side's semantics\n");
    left->CheckSemantics();
    if (DEBUG) printf("checking the right side's semantics\n");
    if (DEBUG) left->Print(5);
    right->CheckSemantics();
    if (DEBUG) right->Print(5);
    if (DEBUG) printf("\n");
    
    if (DEBUG) printf("about to check for errors with left and right sides of AssignExpr\n");
    bool leftError = left->isResultError();
    if (DEBUG) printf("successfully grabbed error info for left\n");
    bool rightError = right->isResultError();
    if (DEBUG) printf("successfully grabbed error info for right\n");
    if(left->isResultError() || right->isResultError()) {
        resultType = Type::errorType;
        if (DEBUG) printf("assignment expression returned early b/c lhs or rhs had internal problems\n");
        return;
    }

    // determine whether the lhs and rhs are of the same type, by using the appropriate IsEquivalentTo function
    if (DEBUG) printf("\tassignment expression good so far, now equating lhs and rhs types\n");
    if (left->GetResultType()->IsEquivalentTo(right->GetResultType())) {
        resultType = left->GetResultType();
    } else {
        ReportError::IncompatibleOperands(op, left->GetResultType(), right->GetResultType());
        resultType = Type::errorType;
    }
}

void AssignExpr::Emit() {
    if (CGDEBUG) printf("in AssignExpr::Emit\n");
    
    if (CGDEBUG) printf("AssignExpr::Emit: first emitting the left\n");
    if (CGDEBUG) printf("AssignExpr::Emit: also note that this 'left' has its isLHS flipped to true now\n");
    left->SetIsLHS(true);
    left->Emit();
    
    if (CGDEBUG) printf("AssignExpr::Emit: second emitting the right\n");
    if (CGDEBUG) printf("\tthe right is of node type %s\n", right->GetPrintNameForNode());
    right->Emit();
    
    // if the right side is a field access or array access, then we need
    // to dereference it to extract the value, and put this in a temp
    Location *rightLoc = right->GetCgLoc();
    /*    if(strcmp(right->GetPrintNameForNode(), "FieldAccess") == 0) {
	if(right->GetOffset() < 0)
	    rightLoc = cg.GenLoad(right->GetCgLoc());
	else
	    rightLoc = cg.GenLoad(right->GetCgLoc(), right->GetOffset());
	    }*/
    /*    if(strcmp(right->GetPrintNameForNode(), "ArrayAccess") == 0)
	rightLoc = cg.GenLoad(right->GetCgLoc());
    else
    rightLoc = right->GetCgLoc();*/
    
    if (strcmp(left->GetPrintNameForNode(), "FieldAccess") == 0) {
	//        if (left->GetOffset() < 0)
	if(!left->GetIsAMemberVariable())
            // It's just a regular variable, so we can just straight-up assign it.
            cg.GenAssign(left->GetCgLoc(), rightLoc);
        else
            // It's a member variable, so we have to dereference it.
            cg.GenStore(left->GetCgLoc(), rightLoc, left->GetOffset());
    } else if(strcmp(left->GetPrintNameForNode(), "ArrayAccess") == 0)
        cg.GenStore(left->GetCgLoc(), rightLoc);
    else
        cg.GenAssign(left->GetCgLoc(), rightLoc);
}

void ReadLineExpr::CheckSemantics() {
  resultType = Type::stringType;
}

void ReadLineExpr::Emit() {
    cgLoc = cg.GenBuiltInCall(ReadLine);
}

void ReadIntegerExpr::CheckSemantics() {
  resultType = Type::intType;
}

void ReadIntegerExpr::Emit() {
    cgLoc = cg.GenBuiltInCall(ReadInteger);
}
