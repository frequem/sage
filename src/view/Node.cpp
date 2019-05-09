#include <sage/view/Node.h>
#include <sage/view/Scene.h>
#include <sage/util/NodePtrZCompare.h>
#include <sage/util/macros.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <algorithm>

using namespace sage;

Node::Node(){
	this->setPosition(0, 0);
	this->setAnchor(0, 0);
	this->setScale(1);
	this->setRotation(0);
	this->setZIndex(0);
}

void Node::render(){
	for(Node* n : this->childNodes){
		n->render();
	}
}

void Node::update(float delta){
	for(Node* n : this->childNodes){
		n->update(delta);
	}
}

void Node::init(){
	for(Node* n : this->childNodes){
		n->init();
	}
	this->initialized = true;
}

Node* Node::getParentNode() const{
	return this->parentNode;
}

void Node::setParentNode(Node* parent){
	this->parentNode = parent;
}

Scene* Node::getScene(){
	return this->getParentNode()->getScene();
}

Application* Node::getApplication(){
	return this->getParentNode()->getApplication();
}

void Node::addChild(Node* node){
	node->setParentNode(this);
	this->childNodes.push_back(node);
	this->sortChildren();
	
	if(this->initialized){
		node->init();
	}
}

void Node::setPosition(float x, float y){
	this->pos.x = x;
	this->pos.y = y;
}

void Node::setPosition(glm::vec2 pos){
	this->setPosition(pos.x, pos.y);
}

void Node::setPositionX(float x){
	this->pos.x = x;
}

void Node::setPositionY(float y){
	this->pos.y = y;
}

glm::vec2 Node::getPosition() const{
	return this->pos;
}

float Node::getPositionX() const{
	return this->pos.x;
}

float Node::getPositionY() const{
	return this->pos.y;
}

void Node::setAnchor(float x, float y){
	this->anchor.x = x;
	this->anchor.y = y;
}

void Node::setAnchor(glm::vec2 anchor){
	this->setAnchor(anchor.x, anchor.y);
}

glm::vec2 Node::getAnchor() const{
	return this->anchor;
}

float Node::getAnchorX() const{
	return this->anchor.x;
}

float Node::getAnchorY() const{
	return this->anchor.y;
}

void Node::setScale(float s){
	this->scale.x = s;
	this->scale.y = s;
}

void Node::setScale(glm::vec2 s){
	this->scale.x = s.x;
	this->scale.y = s.y;
}

void Node::setScaleX(float x){
	this->scale.x = x;
}

void Node::setScaleY(float y){
	this->scale.y = y;
}

glm::vec2 Node::getScale() const{
	return this->scale;
}

float Node::getScaleX() const{
	return this->scale.x;
}

float Node::getScaleY() const{
	return this->scale.y;
}

void Node::setRotation(float r){
	this->rotation = r;
}

float Node::getRotation() const{
	return this->rotation;
}

void Node::setZIndex(int z){
	this->z_index = z;
	
	if(this->parentNode != nullptr){
		this->parentNode->sortChildren();
	}
}

int Node::getZIndex() const{ return this->z_index; }

void Node::sortChildren(){ std::sort(this->childNodes.begin(), this->childNodes.end(), NodePtrZCompare()); }

float Node::getWidth(){ return this->getSize().x; }
float Node::getHeight(){ return this->getSize().y; }

glm::vec2 Node::absPoint(glm::vec2 point){
	glm::mat3 model = glm::mat3(1);
		
	model = glm::translate(model, this->pos);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation);
	
	return this->parentNode->absPoint(glm::vec2(model*glm::vec3(point, 1)));
}

glm::vec2 Node::relPoint(glm::vec2 point){
	glm::mat3 model = glm::mat3(1);
	
	model = glm::translate(model, this->pos);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation);
	
	model = glm::inverse(model);
	
	return glm::vec2(model*glm::vec3(this->parentNode->relPoint(point), 1));
}

std::vector<glm::vec2> Node::getAbsPoints(){
	glm::vec2 size = this->getSize();
	
	std::vector<glm::vec2> temp;
	temp.push_back(this->absPoint(size*(glm::vec2(0, 0)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec2(1, 0)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec2(0, 1)-this->anchor)));
	temp.push_back(this->absPoint(size*(glm::vec2(1, 1)-this->anchor)));
	
	return temp;
}

int Node::getDepth(){ return this->getParentNode()->getDepth()+1; }

bool Node::containsAbs(glm::vec2 point){
	glm::vec2 size = this->getSize();
	glm::vec2 rp = this->relPoint(point)+(size*this->anchor);
	
	return (rp.x >= 0 && rp.x <= size.x) && (rp.y >= 0 && rp.y <= size.y);
}

Node::~Node(){
	for(Node* n : this->childNodes){
		delete n;
	}
}

