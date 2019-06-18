#include <sage/view/Scene.h>
#include <sage/view/Application.h>
#include <sage/util/config.h>
#include <sage/util/macros.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace sage;

Scene::Scene(){}

Scene& Scene::getScene(){ return *this; }

void Scene::setApplication(Application& application){ this->application = &application; }
Application& Scene::getApplication(){ return *(this->application); }

glm::vec3 Scene::absPoint(glm::vec3 point){
	glm::mat4 model = glm::mat4(1);
		
	model = glm::translate(model, -this->pos);
	model = glm::translate(model, glm::vec3(this->getApplication().getWindowSize(),0)*this->anchor);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.z, glm::vec3(1,0,0));
	model = glm::rotate(model, this->rotation.x, glm::vec3(0,0,1));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0,1,0));
	
	return glm::vec3(model*glm::vec4(point, 1));
}

glm::vec3 Scene::relPoint(glm::vec3 point){
	glm::mat4 model = glm::mat4(1);
		
	model = glm::translate(model, -this->pos);
	model = glm::translate(model, glm::vec3(this->getApplication().getWindowSize(),0)*this->anchor);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.z, glm::vec3(1,0,0));
	model = glm::rotate(model, this->rotation.x, glm::vec3(0,0,1));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0,1,0));
	
	model = glm::inverse(model);
	
	return glm::vec3(model*glm::vec4(point, 1));
}

glm::vec3 Scene::getSize(){ return glm::vec3(this->getApplication().getWindowSize(), 0); }

int Scene::getTreeDepth(){ return 0; }

Scene::~Scene(){}

