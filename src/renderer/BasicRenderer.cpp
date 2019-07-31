#include <sage/renderer/BasicRenderer.h>
#include <sage/util/ogl.h>
#include <sage/util/NodePtrZCompare.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

using namespace sage;

void BasicRenderer::render(){
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	Renderer::render();
}

void BasicRenderer::render(Node& n){
	if(n.getChildOrderChanged()){
		n.sortChildren();
	}
	Renderer::render(n);
}

void BasicRenderer::renderSingle(TexturedNode& tn){
    GLuint p = this->application->getShaderCache().get("basicTexture");
    glUseProgram(p);
    	
	glm::vec2 windowSize = this->application->getWindowSize();
	std::vector<glm::vec3> points = tn.getAbsPoints();
	std::vector<glm::vec2> textureCoordinates = tn.getTexCoords();
	GLuint texture = tn.getTexture();
	
	glUniform2fv(glGetUniformLocation(p, "windowSize"), 1, glm::value_ptr(windowSize));
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(p, "tex"), 0);
    
	GLint position = glGetAttribLocation(p, "position");
	GLint texCoord = glGetAttribLocation(p, "texCoord");
	
	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(texCoord);
	
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, points.data());
	glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates.data());
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);
	
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(texCoord);
	
	glUseProgram(0);
}

void BasicRenderer::renderSingle(ColorNode& cn){
}

BasicRenderer::~BasicRenderer(){}
