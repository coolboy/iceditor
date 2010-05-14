#include "StackObject.h"

#include <assert.h>

StackObject StackObject::fromNode( tree nd )
{
	StackObject ret;
	int nodeKind = NodeKind(nd);
	if (nodeKind == STNode){
		ret.lexVal = getname(ReverseLookUp(IntVal(nd)));
		ret.argNum = GetArguNumAttr(IntVal(nd));
		ret.symbolType = GetTypeAttr(IntVal(nd));
		ret.preDefined = GetPredefAttr(IntVal(nd));
		ret.nestedLevel = GetNestedLevelAttr(IntVal(nd));
	}
	else if (nodeKind == STRINGNode)
		ret.lexVal = getstring(IntVal(nd));

	switch (NodeKind (nd))
	{
	case IDNode:
		ret.nodeType = "IDNode";
		break;

	case STNode:
		ret.nodeType = "STNode";
		break;

	case INTEGERTNode:
		ret.nodeType = "INTEGERTNode";
		break;

	case NUMNode:   
		ret.nodeType = "NUMNode";
		break;

	case CHARNode: 
		ret.nodeType = "CHARNode";
		break;

	case STRINGNode:
		ret.nodeType = "STRINGNode";
		break;

	case EXPRNode:
		ret.nodeType = GetNodeOpTypeStr(nd);
		break;

	case  DUMMYNode:
		break;

	default:	
		assert(0);
		break;
	}
	return ret;
}
