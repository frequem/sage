#include <sage/view/Node.h>
#include <sage/view/Scene.h>
#include <sage/util/NodePtrZCompare.h>
#include <sage/util/macros.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <utility>

using namespace sage;

Node::Node(){
	this->setPosition(0, 0, 0);
	this->setAnchor(0, 0, 0);
	this->setScale(1);
	this->setRotation(glm::vec3(0,0,0));
}

void Node::update(float delta){
	for(auto&& n : this->childNodes){
		n->update(delta);
	}
}

void Node::init(){
	for(auto&& n : this->childNodes){
		n->init();
	}
	this->initialized = true;
}

void Node::deinit(){
	for(auto&& n : this->childNodes){
		n->deinit();
	}
	this->initialized = false;
}

Node& Node::getParentNode() const{ return *(this->parentNode); }
void Node::setParentNode(Node& parent){ this->parentNode = &parent; }
std::vector<std::shared_ptr<Node>> Node::getChildren(){ return this->childNodes; }
Scene& Node::getScene(){ return this->getParentNode().getScene(); }

Application& Node::getApplication(){ return this->getParentNode().getApplication(); }

void Node::addChild(std::shared_ptr<Node> node){
	this->childNodes.push_back(node);
	node->setParentNode(*this);
	this->setChildOrderChanged();
	
	if(this->initialized){
		node->init();
	}
}

void Node::setPosition(float x, float y, float z){ this->setPositionX(x); this->setPositionY(y); this->setPositionZ(z); }
void Node::setPosition(glm::vec3 pos){ this->setPosition(pos.x, pos.y, pos.z); }
void Node::setPosition(float x, float y){ this->setPositionX(x); this->setPositionY(y); }
void Node::setPosition(glm::vec2 pos){ this->setPosition(pos.x, pos.y); }
void Node::setPositionX(float x){ this->pos.x = x; }
void Node::setPositionY(float y){ this->pos.y = y; }
void Node::setPositionZ(float z){
	this->pos.z = z;
	if(this->parentNode != nullptr){
		this->parentNode->setChildOrderChanged();
	}
}

glm::vec3 Node::getPosition() const{ return this->pos; }
float Node::getPositionX() const{ return this->pos.x; }
float Node::getPositionY() const{ return this->pos.y; }
float Node::getPositionZ() const{ return this->pos.z; }

void Node::setAnchor(float x, float y, float z){ this->anchor.x = x; this->anchor.y = y; this->anchor.z = z; }
void Node::setAnchor(glm::vec3 anchor){ this->anchor = anchor; }
void Node::setAnchor(float x, float y){ this->anchor.x = x;	this->anchor.y = y; }
void Node::setAnchor(glm::vec2 anchor){	this->setAnchor(anchor.x, anchor.y); }

glm::vec3 Node::getAnchor() const{ return this->anchor; }
float Node::getAnchorX() const{ return this->anchor.x; }
float Node::getAnchorY() const{	return this->anchor.y; }
float Node::getAnchorZ() const{	return this->anchor.z; }

void Node::setScale(float s){ this->scale.x = s; this->scale.y = s; this->scale.z = s; }
void Node::setScale(glm::vec2 s){ this->scale.x = s.x; this->scale.y = s.y; }
void Node::setScale(glm::vec3 s){ this->scale = s; }
void Node::setScaleX(float x){ this->scale.x = x; }
void Node::setScaleY(float y){ this->scale.y = y; }
void Node::setScaleZ(float z){ this->scale.z = z; }

glm::vec3 Node::getScale() const{ return this->scale; }
float Node::getScaleX() const{ return this->scale.x; }
float Node::getScaleY() const{ return this->scale.y; }
float Node::getScaleZ() const{ return this->scale.z; }

void Node::setRotation(float r){ this->rotation.x = r; }
void Node::setRotation(glm::vec3 r){ this->rotation = r; }

glm::vec3 Node::getRotation() const{ return this->rotation; }

float Node::getWidth(){ return this->getSize().x; }
float Node::getHeight(){ return this->getSize().y; }
float Node::getDepth(){ return this->getSize().z; }

bool Node::getChildOrderChanged(){ return this->childOrderChanged; }
void Node::setChildOrderChanged(){ this->childOrderChanged = true; }

void Node::sortChildren(){ 
	std::sort(this->childNodes.begin(), this->childNodes.end(), NodePtrZCompare());
	this->childOrderChanged = false;
}

glm::vec3 Node::absPoint(glm::vec3 point){
	glm::mat4 model = glm::mat4(1);
		
	model = glm::translate(model, this->pos);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.z, glm::vec3(1,0,0));
	model = glm::rotate(model, this->rotation.x, glm::vec3(0,0,1));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0,1,0));
	
	return this->parentNode->absPoint(glm::vec3(model*glm::vec4(point, 1)));
}

glm::vec3 Node::relPoint(glm::vec3 point){
	glm::mat4 model = glm::mat4(1);
	
	model = glm::translate(model, this->pos);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.z, glm::vec3(1,0,0));
	model = glm::rotate(model, this->rotation.x, glm::vec3(0,0,1));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0,1,0));
	
	model = glm::inverse(model);
	
	return glm::vec3(model*glm::vec4(this->parentNode->relPoint(point), 1));
}

std::vector<glm::vec3> Node::getAbsPoints(){
	glm::vec3 size = this->getSize();
	
	std::vector<glm::vec3> temp;
	temp.push_back(this->absPoint(size*(glm::vec3(0, 0, 0)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(1, 0, 0)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(0, 1, 0)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(1, 1, 0)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(0, 0, 1)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(1, 0, 1)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(0, 1, 1)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec3(1, 1, 1)-this->anchor)));
	
	return temp;
}

int Node::getTreeDepth(){ return this->getParentNode().getTreeDepth()+1; }

bool Node::containsAbs(glm::vec3 point){
	glm::vec3 size = this->getSize();
	glm::vec3 rp = this->relPoint(point)+(size*this->anchor);
	
	return (rp.x >= 0 && rp.x <= size.x) && (rp.y >= 0 && rp.y <= size.y) && (rp.z >= 0 && rp.z <= size.z);
}
bool Node::containsAbs(glm::vec2 point){
	glm::vec3 size = this->getSize();
	glm::vec3 rp = this->relPoint(glm::vec3(point, 0))+(size*this->anchor);
	
	return (rp.x >= 0 && rp.x <= size.x) && (rp.y >= 0 && rp.y <= size.y);
}

Node::~Node(){}

