/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "semantic_analyzer.h"
#include "codegen.h"

extern SemanticAnalyzer SA;
extern CodeGenerator cg;
extern bool DEBUG;

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

void Program::CheckSemantics() {
    SA.curScopeNum = 0;
    SA.curClass = NULL;
    SA.curFunc = NULL;
    SA.curInsideLoop = false;
    SA.AddDecls(decls);

    for (int i = 0; i < decls->NumElements(); i++) {
        if (DEBUG) printf("checking semantics for %s %s\n", decls->Nth(i)->GetPrintNameForNode(), decls->Nth(i)->GetName());
        decls->Nth(i)->CheckSemantics();
    }

    // (note we don't care about removing vars/funcs from scope
    //  at the end of the program, but we'll do it anyway for testing
    //  purposes)
    SA.RemoveDecls(decls);
}

void Program::Emit() {
    /* pp4: here is where the code generation is kicked off.
     *      The general idea is perform a tree traversal of the
     *      entire program, generating instructions as you go.
     *      Each node can have its own way of translating itself,
     *      which makes for a great use of inheritance and
     *      polymorphism in the node classes.
     */
    // TODO

    cg.AddDecls(decls);

    // make sure there is a main function. if there isn't, report error and quit
    ScopedDecl* mainLookup = cg.existingDecls.Lookup("main");
    if(mainLookup == NULL) {
	ReportError::NoMainFound();
	return;
    }
    else {
	// we found a main, but we have to make sure its return type is int or void
	FnDecl* mainFn = (FnDecl*) mainLookup->decl;
	if(!mainFn->GetReturnType()->IsEquivalentTo(Type::voidType)
	   && !mainFn->GetReturnType()->IsEquivalentTo(Type::intType)) {
	    ReportError::NoMainFound();
	    return;
	}
    }
    
    // Before doing anything else, we need the vtables of each class.
    // (any method access will need this information)
    for (int i = 0; i < decls->NumElements(); i++) {
        const char *nodeType = decls->Nth(i)->GetPrintNameForNode();
        // if this is a ClassDecl, generate a vtable
        if (strcmp(nodeType, "ClassDecl") == 0) {
            ClassDecl *curClassDecl = (ClassDecl*)decls->Nth(i);
            curClassDecl->AddToOffsets(curClassDecl);
            cg.GenVTable(curClassDecl->GetName(), curClassDecl->GetMethodLabels());

            // Set labels for all functions within the class.
            cg.curClass = curClassDecl;
            List<Decl *> *classDecls = curClassDecl->GetMembers();
            for (int j = 0; j < classDecls->NumElements(); j++) {
                Decl *cur = classDecls->Nth(j);
                if (strcmp(cur->GetPrintNameForNode(), "FnDecl") == 0) {
                    ((FnDecl *) cur)->SetLabel();
                }
            }
            cg.curClass = NULL;
        } else if (strcmp(nodeType, "FnDecl") == 0) {
            FnDecl *curFnDecl = (FnDecl *) decls->Nth(i);
            curFnDecl->SetLabel();
        }
    }
    /*printf("\n\nQUITTING after vtable attempt! THIS is in ast_stmt.cc ~line 70.\n\n");
    exit(0);*/

    for (int i = 0; i < decls->NumElements(); i++) {
    decls->Nth(i)->Emit();
    }
    cg.RemoveDecls(decls);
    cg.DoFinalCodeGen();
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

void StmtBlock::CheckSemantics() {
    if (DEBUG) printf("Now checking semantics for statement block.\n");
    SA.curScopeNum++;
    SA.AddDecls((List<Decl*>*) decls);

    for (int i = 0; i < decls->NumElements(); i++)
        decls->Nth(i)->CheckSemantics();
    for (int i = 0; i < stmts->NumElements(); i++) {
        if (DEBUG) printf("\tChecking for statement %s.\n", stmts->Nth(i)->GetName());
        stmts->Nth(i)->CheckSemantics();
    }

    SA.RemoveDecls((List<Decl*>*) decls);
    SA.curScopeNum--;
}

void StmtBlock::Emit() {
    if (CGDEBUG) printf("in StmtBlock::Emit\n");
    cg.curScopeNum++;
    cg.AddDecls((List<Decl *> *) decls);

    if (CGDEBUG) printf("StmtBlock::Emit: emitting the decls\n");
    for (int i = 0; i < decls->NumElements(); i++) {
        decls->Nth(i)->Emit();
    }
    if (CGDEBUG) printf("StmtBlock::Emit: emitting the stmts\n");
    for (int i = 0; i < stmts->NumElements(); i++) {
        stmts->Nth(i)->Emit();
    }

    cg.RemoveDecls((List<Decl *> *) decls);
    cg.curScopeNum--;
}

CaseStmt::CaseStmt(IntConstant *c, List<Stmt*> *s) {
	condition=c;
	if(condition != NULL)
	  condition->SetParent(this);
	stmts = s;
	if(stmts != NULL)
		stmts->SetParentAll(this);
}

void CaseStmt::PrintChildren(int indentLevel) {
	if(condition != NULL)
	  condition->Print(indentLevel+1);
	if(stmts != NULL)
		stmts->PrintAll(indentLevel+1);
}

void CaseStmt::CheckSemantics() {
  // the IntConstant condition can be anything, so that's fine.
  // we just need to check the validity of the statements in this case.
  for(int i=0; i<stmts->NumElements(); i++)
    stmts->Nth(i)->CheckSemantics();
}

SwitchStmt::SwitchStmt(Expr *t, List<CaseStmt*> *c, CaseStmt *d) {
	Assert(t != NULL && c != NULL);
	(test=t)->SetParent(this);
	(cases=c)->SetParentAll(this);
	defaultStmt = d;
	if(defaultStmt != NULL)
	  defaultStmt->SetParent(this);
}

void SwitchStmt::PrintChildren(int indentLevel) {
	test->Print(indentLevel);
	cases->PrintAll(indentLevel+1);
	if(defaultStmt != NULL)
	  defaultStmt->Print(indentLevel+1);
}

void SwitchStmt::CheckSemantics() {
  test->CheckSemantics();
  if (strcmp(test->GetResultType()->GetName(), Type::boolType->GetName()) != 0
      && strcmp(test->GetResultType()->GetName(), Type::errorType->GetName()) != 0) {
    ReportError::TestNotBoolean(test);
    SA.numErrors++;
  }
  // TODO from pras: chris, i think you forgot to checksemantics of the cases (+ default case). confirm this though.
  for(int i=0; i<cases->NumElements(); i++)
    cases->Nth(i)->CheckSemantics();
  if(defaultStmt != NULL)
    defaultStmt->CheckSemantics();
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

void ConditionalStmt::CheckSemantics() {
    if (DEBUG) printf("Now checking conditional statement.\n");
    if (DEBUG) Print(0);

    // Check test and body.  Note that scope is increased within the body.
    test->CheckSemantics();
    SA.curScopeNum++;
    body->CheckSemantics();
    SA.curScopeNum--;
    
    // Make sure test returns a boolean
    // TODO should we only do this if test->CheckSemantics() comes out okay?
    // e.g. if the test is just some variable that hasn't been declared yet,
    // maybe we should just report that instead of also complaining about 
    // no boolean return
    if (DEBUG) printf("\tChecking whether test returns boolean.\n");
    //if (!test->GetResultType()->IsEquivalentTo(Type::boolType)) {
    if(strcmp(test->GetResultType()->GetName(), Type::boolType->GetName()) != 0
       && strcmp(test->GetResultType()->GetName(), Type::errorType->GetName()) != 0) {
      ReportError::TestNotBoolean(test);
      SA.numErrors++;
    }
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

void ForStmt::PrintChildren(int indentLevel) {
    init->Print(indentLevel+1, "(init) ");
    test->Print(indentLevel+1, "(test) ");
    step->Print(indentLevel+1, "(step) ");
    body->Print(indentLevel+1, "(body) ");
}

void ForStmt::CheckSemantics() {
  init->CheckSemantics(); //TODO: any other checks on the init expr?
  //  test->CheckSemantics();
  // TODO - from pras:  chris, ConditionalStmt::CheckSemantics() already takes care of checking the test and body

  bool prevInsideLoop = SA.curInsideLoop;
  SA.curInsideLoop = true;
  
  ConditionalStmt::CheckSemantics();
  
  SA.curInsideLoop = prevInsideLoop;

  step->CheckSemantics(); //TODO: any other checks on the step expr?
  //  body->CheckSemantics();
}

void ForStmt::Emit() {
    if (CGDEBUG) printf("in ForStmt::Emit\n");
    if (CGDEBUG) printf("ForStmt::Emit: emitting init expr\n");
    init->Emit();
    
    // label before the evaluation of the test expression
    char *testLabel = cg.NewLabel();
    cg.GenLabel(testLabel);
    
    // evaluation of the test expression
    if (CGDEBUG) printf("ForStmt::Emit: emitting test expr\n");
    test->Emit();
    
    // if we fail the test, skip past the body
    if (CGDEBUG) printf("ForStmt::Emit: emitting ifz/goto expr\n");
    endLabel = cg.NewLabel();
    cg.GenIfZ(test->GetCgLoc(), endLabel);
    
    // else, run the body
    if (CGDEBUG) printf("ForStmt::Emit: emitting body\n");
    body->Emit();
    
    // run the step
    if (CGDEBUG) printf("ForStmt::Emit: emitting step\n");
    step->Emit();
    
    // then go to the test to see if we loop again
    if (CGDEBUG) printf("ForStmt::Emit: emitting bottom goto\n");
    cg.GenGoto(testLabel);
    
    // label after the whole while loop
    if (CGDEBUG) printf("ForStmt::Emit: emitting end label\n");
    cg.GenLabel(endLabel);
}

void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::CheckSemantics() {
  bool prevInsideLoop = SA.curInsideLoop;
  SA.curInsideLoop = true;
  
  ConditionalStmt::CheckSemantics();
  
  SA.curInsideLoop = prevInsideLoop;
}

void WhileStmt::Emit() {
    // label before the evaluation of the test expression
    char *testLabel = cg.NewLabel();
    cg.GenLabel(testLabel);
    
    // evaluation of the test expression
    test->Emit();
    
    // if we fail the test, skip past the body
    endLabel = cg.NewLabel();
    cg.GenIfZ(test->GetCgLoc(), endLabel);
    
    // else, run the body
    body->Emit();
    
    // then go to the test to see if we loop again
    cg.GenGoto(testLabel);
    
    // label after the whole while loop
    cg.GenLabel(endLabel);
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(then) ");
    if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}

void IfStmt::CheckSemantics() {
  ConditionalStmt::CheckSemantics();
  if(elseBody)
    elseBody->CheckSemantics();
}

void IfStmt::Emit() {
    test->Emit();
    char *afterIfLabel = cg.NewLabel();
    cg.GenIfZ(test->GetCgLoc(), afterIfLabel);
    body->Emit();
    
    if(elseBody == NULL)
        cg.GenLabel(afterIfLabel);
    else {
        char *afterElseLabel = cg.NewLabel();
        cg.GenGoto(afterElseLabel);
        cg.GenLabel(afterIfLabel);
        elseBody->Emit();
        cg.GenLabel(afterElseLabel);
    }
}

void BreakStmt::CheckSemantics() {
  // a break stmt MUST be inside of a loop
  if(DEBUG)  printf("\nChecking BreakStmt semantics.");
  if(!SA.curInsideLoop)  // so if we are not, report an error
    ReportError::BreakOutsideLoop(this);
}

void BreakStmt::Emit() {
    // keep going up parents until we get to a While or For loop
    Node *p = GetParent();
    while(strcmp(p->GetPrintNameForNode(), "ForStmt") != 0  &&  strcmp(p->GetPrintNameForNode(), "WhileStmt") != 0)
        p = p->GetParent();
    cg.GenGoto(((LoopStmt*)p)->GetEndLabel());
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    expr->Print(indentLevel+1);
}

void ReturnStmt::CheckSemantics() {
  expr->CheckSemantics();
  
  if(SA.curFunc == NULL) // we are not in a function, so this is totally invalid
    ReportError::ReturnMismatch(this, expr->GetResultType(), Type::emptyType);
  else {
    Type *expectedType = SA.curFunc->GetReturnType();
    if(!expectedType->IsEquivalentTo(expr->GetResultType()))
      ReportError::ReturnMismatch(this, expr->GetResultType(), expectedType);
  }
}

void ReturnStmt::Emit() {
    expr->Emit();
    cg.GenReturn(expr->GetCgLoc());
}

PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

void PrintStmt::PrintChildren(int indentLevel) {
    args->PrintAll(indentLevel+1, "(args) ");
}

void PrintStmt::CheckSemantics() {
  // loop through each of the args, and make sure the expressions are of
  // type int, bool, or double
  for(int i=0; i<args->NumElements(); i++) {
    args->Nth(i)->CheckSemantics();
    Type *curArgType = args->Nth(i)->GetResultType();
    if(!Type::errorType->IsEquivalentTo(curArgType)) { // if error, don't double-report
      if(!Type::intType->IsEquivalentTo(curArgType)
       && !Type::boolType->IsEquivalentTo(curArgType)
       && !Type::stringType->IsEquivalentTo(curArgType))
	ReportError::PrintArgMismatch(args->Nth(i), i+1, curArgType);
    }
  }
}

void PrintStmt::Emit() {
    if (CGDEBUG) printf("in PrintStmt::Emit\n");
    // Generate a call for each argument.
    for (int i = 0; i < args->NumElements(); i++) {
        // Call emit on the argument.
        Expr *arg = args->Nth(i);
        arg->Emit();

        // Figure out the argument type.
        Type *argType = arg->GetResultType();
        BuiltIn printType;
        if (argType->IsEquivalentTo(Type::intType)) {
            printType = PrintInt;
        } else if (argType->IsEquivalentTo(Type::boolType)) {
            printType = PrintBool;
        } else if (argType->IsEquivalentTo(Type::stringType)) {
            printType = PrintString;
        } else {
            // Something went wrong
            Assert(false);
        }

        // Generate the print instruction.
        cg.GenBuiltInCall(printType, arg->GetCgLoc());
    }
}

