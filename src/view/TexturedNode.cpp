#include <sage/view/TexturedNode.h>
#include <sage/renderer/Renderer.h>
#include <sage/util/macros.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sage;

TexturedNode::TexturedNode() : Node(){}

std::vector<glm::vec2> TexturedNode::getTexCoords(){
	std::vector<glm::vec2> temp;
	temp.push_back(glm::vec2(0.0, 1.0));
	temp.push_back(glm::vec2(1.0, 1.0));
	temp.push_back(glm::vec2(0.0, 0.0));
	temp.push_back(glm::vec2(1.0, 0.0));
	
	return temp;
};

void TexturedNode::render(Renderer& r){ r.renderSingle(*this); }

TexturedNode::~TexturedNode(){}

