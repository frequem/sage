#include <sage/util/NodePtrZCompare.h>

#include <sage/view/Node.h>

using namespace sage;

bool NodePtrZCompare::operator()(const std::shared_ptr<Node>& n1, const std::shared_ptr<Node>& n2) const{		
	return n1->getPositionZ() < n2->getPositionZ();
}
