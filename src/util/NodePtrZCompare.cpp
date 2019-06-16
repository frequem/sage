#include <sage/util/NodePtrZCompare.h>

#include <sage/view/Node.h>

using namespace sage;

bool NodePtrZCompare::operator()(const Node* n1, const Node* n2) const{		
	return n1->getPositionZ() < n2->getPositionZ();
}
