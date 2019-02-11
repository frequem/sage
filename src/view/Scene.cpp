#include <sage/view/Scene.h>
#include <sage/view/Application.h>
#include <sage/util/macros.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace sage;

Scene::Scene(){
}

void Scene::render(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	Node::render();
}

Scene* Scene::getScene(){
	return this;
}

void Scene::setApplication(Application* application){
	this->application = application;
}

Application* Scene::getApplication(){
	return this->application;
}

glm::vec2 Scene::absPoint(glm::vec2 point){
	glm::mat3 model = glm::mat3(1);
		
	model = glm::translate(model, -this->pos);
	model = glm::translate(model, this->getApplication()->getWindowSize()*this->anchor);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation);
	
	return glm::vec2(model*glm::vec3(point, 1));
}

Scene::~Scene(){
}

