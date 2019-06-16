#include <sage/view/Scene.h>
#include <sage/view/Application.h>
#include <sage/util/config.h>
#include <sage/util/macros.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace sage;

Scene::Scene(){
}

void Scene::render(int pass){
	//glClearStencil(0);
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//|GL_STENCIL_BUFFER_BIT
	
	GLuint frameBuffers[DEPTH_PEELING_PASSES];
	glGenFramebuffers(DEPTH_PEELING_PASSES, frameBuffers);
	GLuint frameTextures[DEPTH_PEELING_PASSES];
	glGenTextures(DEPTH_PEELING_PASSES, frameTextures);
	GLuint depthTextures[2];
	glGenTextures(2, depthTextures);
	
	glm::vec2 s = this->getApplication()->getWindowSize();
	
	for(int i=0; i<DEPTH_PEELING_PASSES; i++){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s.x, s.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
	
	for(int i=0; i<2; i++){
		glActiveTexture(GL_TEXTURE1 + i);//1,2 (0 is used for actual texturing)
		glBindTexture(GL_TEXTURE_2D, depthTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, s.x, s.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
	
	for(int i=0; i<DEPTH_PEELING_PASSES; i++){
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTextures[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextures[i%2], 0);
		
		
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		Node::render(i);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	GLuint p = this->getApplication()->getShaderCache()->get("fullscreenTexture");
    glUseProgram(p);

	glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(p, "tex"), 0);

	float indices[4] = {0.f,1.f,2.f,3.f};
	GLint idx = glGetAttribLocation(p, "idx");
	glEnableVertexAttribArray(idx);
	glVertexAttribPointer(idx, 1, GL_FLOAT, GL_FALSE, 0, indices);
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(int i=DEPTH_PEELING_PASSES-1; i>=0; i--){
		glBindTexture(GL_TEXTURE_2D, frameTextures[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(idx);
	
	glUseProgram(0);
	
	glDeleteTextures(DEPTH_PEELING_PASSES, frameTextures);
	glDeleteTextures(2, depthTextures);
	glDeleteFramebuffers(DEPTH_PEELING_PASSES, frameBuffers);
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

glm::vec3 Scene::absPoint(glm::vec3 point){
	glm::mat4 model = glm::mat4(1);
		
	model = glm::translate(model, -this->pos);
	model = glm::translate(model, glm::vec3(this->getApplication()->getWindowSize(),0)*this->anchor);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.z, glm::vec3(1,0,0));
	model = glm::rotate(model, this->rotation.x, glm::vec3(0,0,1));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0,1,0));
	
	return glm::vec3(model*glm::vec4(point, 1));
}

glm::vec3 Scene::relPoint(glm::vec3 point){
	glm::mat4 model = glm::mat4(1);
		
	model = glm::translate(model, -this->pos);
	model = glm::translate(model, glm::vec3(this->getApplication()->getWindowSize(),0)*this->anchor);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.z, glm::vec3(1,0,0));
	model = glm::rotate(model, this->rotation.x, glm::vec3(0,0,1));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0,1,0));
	
	model = glm::inverse(model);
	
	return glm::vec3(model*glm::vec4(point, 1));
}

glm::vec3 Scene::getSize(){ return glm::vec3(getApplication()->getWindowSize(), 0); }

int Scene::getTreeDepth(){ return 0; }

Scene::~Scene(){
}

