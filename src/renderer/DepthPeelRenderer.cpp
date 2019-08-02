#include <sage/renderer/DepthPeelRenderer.h>
#include <sage/util/ogl.h>
#include <sage/util/NodePtrZCompare.h>
#include <sage/view/TexturedNode.h>
#include <sage/view/ColorNode.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <sage/util/config.h>

using namespace sage;

DepthPeelRenderer::DepthPeelRenderer(Application& application) : DepthPeelRenderer(application, DEPTH_PEELING_PASSES){}

DepthPeelRenderer::DepthPeelRenderer(Application& application, int peels) : Renderer(application), peels(peels){
	this->frameBuffers = new GLuint[peels];
	this->frameTextures = new GLuint[peels];
	this->depthTextures = new GLuint[peels];
	
	this->initBuffers();
}

void DepthPeelRenderer::initBuffers(){
	LOG("peels:%i", this->peels);
	glGenFramebuffers(this->peels, frameBuffers);
	glGenTextures(this->peels, frameTextures);
	glGenTextures(this->peels, depthTextures);
	
	glm::vec2 s = this->application->getWindowSize();
	this->oldFrameSize = s;
	
	glActiveTexture(GL_TEXTURE0);
	for(int i=0; i<this->peels; i++){
		glBindTexture(GL_TEXTURE_2D, frameTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s.x, s.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
	glActiveTexture(GL_TEXTURE1);
	for(int i=0; i<this->peels; i++){
		glBindTexture(GL_TEXTURE_2D, depthTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, s.x, s.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void DepthPeelRenderer::deinitBuffers(){
	glDeleteTextures(this->peels, depthTextures);
	glDeleteTextures(this->peels, frameTextures);
	glDeleteFramebuffers(this->peels, frameBuffers);
}

void DepthPeelRenderer::render(){
	if(!glm::all(glm::equal(this->oldFrameSize, this->application->getWindowSize()))){
		this->deinitBuffers();
		this->initBuffers();
	}
	
	for(this->currentPass = 0; this->currentPass<this->peels; this->currentPass++){
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffers[this->currentPass]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->frameTextures[this->currentPass], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTextures[this->currentPass], 0);
		
		if(this->currentPass > 0){
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthTextures[this->currentPass-1]);
		}
		
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
		Renderer::render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	GLuint p = this->application->getShaderCache().get("fullscreenTexture");
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
	for(int i=this->peels-1; i>=0; i--){
		glBindTexture(GL_TEXTURE_2D, frameTextures[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(idx);
	
	glUseProgram(0);
}

void DepthPeelRenderer::renderSingle(TexturedNode& tn){
	GLuint p = this->application->getShaderCache().get("depthpeelTexture");
    glUseProgram(p);
    	
	glm::vec2 windowSize = this->application->getWindowSize();
	GLuint texture = tn.getTexture();
	std::vector<glm::vec3> points = tn.getAbsPoints();
	std::vector<glm::vec2> textureCoordinates = tn.getTexCoords();
	
	glUniform2fv(glGetUniformLocation(p, "windowSize"), 1, glm::value_ptr(windowSize));
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(p, "tex"), 0);
    
    glUniform1i(glGetUniformLocation(p, "peel"), this->currentPass>0);
	glUniform1i(glGetUniformLocation(p, "depthTex"), 1);
	
	GLint position = glGetAttribLocation(p, "position");
	GLint texCoord = glGetAttribLocation(p, "texCoord");
	
	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(texCoord);
	
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, points.data());
	glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates.data());
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_DEPTH_TEST);
	
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(texCoord);
	
	glUseProgram(0);
}

void DepthPeelRenderer::renderSingle(ColorNode& cn){
	GLuint p = this->application->getShaderCache().get("depthpeelColor");
    glUseProgram(p);
    	
	glm::vec2 windowSize = this->application->getWindowSize();
	glm::vec4 color = cn.getColor();
	std::vector<glm::vec3> points = cn.getAbsPoints();
	
	glUniform2fv(glGetUniformLocation(p, "windowSize"), 1, glm::value_ptr(windowSize));
	glUniform4fv(glGetUniformLocation(p, "color"), 1, glm::value_ptr(color));
	    
    glUniform1i(glGetUniformLocation(p, "peel"), this->currentPass>0);
	glUniform1i(glGetUniformLocation(p, "depthTex"), 1);
	
	GLint position = glGetAttribLocation(p, "position");
	
	glEnableVertexAttribArray(position);
	
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, points.data());
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_DEPTH_TEST);
	
	glDisableVertexAttribArray(position);
	
	glUseProgram(0);
}

DepthPeelRenderer::~DepthPeelRenderer(){
	this->deinitBuffers();
	delete [] this->frameTextures;
	delete [] this->frameBuffers;
}
