#include <sage/view/Sprite.h>
#include <sage/util/macros.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sage;

Sprite::Sprite(const std::string& textureFile) : Node(){
	this->textureFile = textureFile;
}

void Sprite::render(){
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, this->getDepth()-1, ~0);
	glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
	
    GLuint p = this->getApplication()->getShaderCache()->get("textured2d");
    glUseProgram(p);
    	
	glm::vec2 windowSize = this->getApplication()->getWindowSize();
	glUniform2fv(glGetUniformLocation(p, "windowSize"), 1, glm::value_ptr(windowSize));
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->getApplication()->getImageCache()->getTexture(this->textureFile));
    glUniform1i(glGetUniformLocation(p, "tex"), 0);
		
	std::vector<glm::vec2> points = this->getAbsPoints();
	GLint position = glGetAttribLocation(p, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, points.data());
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisable(GL_BLEND);
	glUseProgram(0);
	
	glDisableVertexAttribArray(position);
	
	Node::render();
	
	glStencilFunc(GL_EQUAL, this->getDepth(), ~0);
	glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
	
    p = this->getApplication()->getShaderCache()->get("stencil2d");
    glUseProgram(p);
    
	glUniform2fv(glGetUniformLocation(p, "windowSize"), 1, glm::value_ptr(windowSize));
	
	position = glGetAttribLocation(p, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, points.data());
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glUseProgram(0);
	
	glDisableVertexAttribArray(position);
}

void Sprite::update(float delta){
	Node::update(delta);
}

glm::vec2 Sprite::getSize(){
	return this->getApplication()->getImageCache()->getSize(this->textureFile);
}

Sprite::~Sprite(){
}

