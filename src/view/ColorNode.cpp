#include <sage/view/ColorNode.h>

using namespace sage;

ColorNode::ColorNode(glm::vec2 size) : ColorNode(size, {0, 0, 0, 255}){}
ColorNode::ColorNode(glm::vec2 size, SDL_Color color) : size(size), color(color){}

void ColorNode::init(){
	Node::init();
}

void ColorNode::setColor(SDL_Color color){
	this->color = color;
}

glm::vec3 ColorNode::getSize(){ return glm::vec3(this->size, 0); }

ColorNode::~ColorNode(){}

