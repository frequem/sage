#include <sage/view/Sprite.h>
#include <sage/util/macros.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sage;

Sprite::Sprite(const std::string& textureFile) : Node(){
	this->textureFile = textureFile;
}

void Sprite::init(){
	auto ic = this->getApplication()->getImageCache();
	
	this->texture = ic->getTexture(this->textureFile);
	this->size = ic->getSize(this->textureFile);
	
	Node::init();
}

void Sprite::render(){
    GLuint p = this->getApplication()->getShaderCache()->get("textured2d");
    
    glUseProgram(p);
    
    GLint projection = glGetUniformLocation(p, "projection");
    GLint modelview = glGetUniformLocation(p, "modelview");
    
	GLint tex = glGetUniformLocation(p, "tex");
	GLint texCoord = glGetAttribLocation(p, "texCoord");
	GLint position = glGetAttribLocation(p, "position");
	
	glm::vec2 windowSize = this->getApplication()->getWindowSize();
	glm::mat4 mProjection = glm::ortho(0.f, windowSize.x, 0.f, windowSize.y, 0.f, 1.f);
	glm::mat4 mModelView = glm::mat4(1.f);
	
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(mProjection));
	glUniformMatrix4fv(modelview, 1, GL_FALSE, glm::value_ptr(mModelView));
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex, 0);
		
	glm::vec2 d[] = {
		this->absPoint(this->size*(glm::vec2(0, 0)-this->anchor)), 	glm::vec2(0,1),
		this->absPoint(this->size*(glm::vec2(1, 0)-this->anchor)), 	glm::vec2(1,1),
		this->absPoint(this->size*(glm::vec2(0, 1)-this->anchor)), 	glm::vec2(0,0),
		this->absPoint(this->size*(glm::vec2(1, 1)-this->anchor)),  glm::vec2(1,0)
	};
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(texCoord);
	
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(d[0]), glm::value_ptr(d[0]));
	glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 2*sizeof(d[0]), glm::value_ptr(d[1]));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(texCoord);
	
	glDisable(GL_BLEND);
	glUseProgram(0);
	
	Node::render();
}

void Sprite::update(float delta){
	Node::update(delta);
}

Sprite::~Sprite(){
}

