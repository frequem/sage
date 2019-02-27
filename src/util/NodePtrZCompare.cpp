#include <sage/util/NodePtrZCompare.h>

#include <sage/view/Node.h>

using namespace sage;

bool NodePtrZCompare::operator()(const Node* lhs, const Node* rhs) const{		
	return lhs->getZIndex() < rhs->getZIndex();
}
