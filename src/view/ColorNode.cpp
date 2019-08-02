#include <sage/view/ColorNode.h>
#include <sage/renderer/Renderer.h>

using namespace sage;

ColorNode::ColorNode(glm::vec2 size) : ColorNode(size, glm::vec4(0, 0, 0, 1)){}
ColorNode::ColorNode(glm::vec2 size, glm::vec4 color) : size(size), color(color){}

void ColorNode::init(){
	Node::init();
}

void ColorNode::setColor(glm::vec4 color){ this->color = color; }
glm::vec4 ColorNode::getColor(){ return this->color; }

glm::vec3 ColorNode::getSize(){ return glm::vec3(this->size, 0); }

void ColorNode::render(Renderer& r){ r.renderSingle(*this); }


ColorNode::~ColorNode(){}

