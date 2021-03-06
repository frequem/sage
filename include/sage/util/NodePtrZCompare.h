#ifndef _SAGE_NODE_POINTER_Z_COMPARE
#define _SAGE_NODE_POINTER_Z_COMPARE

#include <memory>

namespace sage{
	class Node;
	
	/**
	 * @brief Compare for z-position of Node pointers.
	 */
	struct NodePtrZCompare{
		/**
		 * @brief Compares the z-position of two Nodes.
		 * @param n1 the first Node
		 * @param n2 the second Node
		 * @return true if n1 < n2 is given for the z-position
		 */
		bool operator()(const std::shared_ptr<Node>& n1, const std::shared_ptr<Node>& n2) const;
	};
}

#endif //_SAGE_NODE_POINTER_Z_COMPARE

