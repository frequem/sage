#include <sage/Node.h>
#include <sage/Scene.h>
#include <sage/macros.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace sage;

Node::Node(){
	this->setPosition(0, 0);
	this->setAnchor(0, 0);
	this->setScale(1);
	this->setRotation(0);
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

Node* Node::getParentNode(){
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

glm::vec2 Node::getPosition(){
	return this->pos;
}

float Node::getPositionX(){
	return this->pos.x;
}

float Node::getPositionY(){
	return this->pos.y;
}

void Node::setAnchor(float x, float y){
	this->anchor.x = x;
	this->anchor.y = y;
}

void Node::setAnchor(glm::vec2 anchor){
	this->setAnchor(anchor.x, anchor.y);
}

glm::vec2 Node::getAnchor(){
	return this->anchor;
}

float Node::getAnchorX(){
	return this->anchor.x;
}

float Node::getAnchorY(){
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

glm::vec2 Node::getScale(){
	return this->scale;
}

float Node::getScaleX(){
	return this->scale.x;
}

float Node::getScaleY(){
	return this->scale.y;
}

void Node::setRotation(float r){
	this->rotation = r;
}

float Node::getRotation(){
	return this->rotation;
}

glm::vec2 Node::absPoint(glm::vec2 point){
	glm::mat3 model = glm::mat3(1);
		
	model = glm::translate(model, this->pos);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation);
	
	return this->parentNode->absPoint(glm::vec2(model*glm::vec3(point, 1)));
}

Node::~Node(){
	for(Node* n : this->childNodes){
		delete n;
	}
}

