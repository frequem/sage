#ifndef _SAGE_NODE_POINTER_Z_COMPARE
#define _SAGE_NODE_POINTER_Z_COMPARE

namespace sage{
	class Node;
	
	struct NodePtrZCompare{
		bool operator()(const Node* lhs, const Node* rhs) const;
	};
}

#endif //_SAGE_NODE_POINTER_Z_COMPARE

