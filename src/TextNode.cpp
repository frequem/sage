#include <sage/TextNode.h>
#include <sage/macros.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sage;

TextNode::TextNode(const std::string& font, int ptsize) : 
	TextNode(font, ptsize, ""){
}

TextNode::TextNode(const std::string& font, int ptsize, const std::string& text) : 
	TextNode(font, ptsize, text, {255, 255, 255, 0}){	
}

TextNode::TextNode(const std::string& font, int ptsize, const std::string& text, SDL_Color color) : 
	Node(), fontFile(font), ptsize(ptsize), text(text), color(color){
	glGenTextures(1, &this->texture);
}

void TextNode::init(){
	this->rebuild();
}

void TextNode::render(){
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
		
	glm::vec2 size = glm::vec2(this->surface->w, this->surface->h);
	glm::vec2 d[] = {
		this->absPoint(size*(glm::vec2(0, 0)-this->anchor)), 	glm::vec2(0,1),
		this->absPoint(size*(glm::vec2(1, 0)-this->anchor)), 	glm::vec2(1,1),
		this->absPoint(size*(glm::vec2(0, 1)-this->anchor)), 	glm::vec2(0,0),
		this->absPoint(size*(glm::vec2(1, 1)-this->anchor)),  	glm::vec2(1,0)
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

void TextNode::update(float delta){
	Node::update(delta);
}

void TextNode::rebuild(){
	TTF_Font* font = this->getApplication()->getFontCache()->get(this->fontFile, this->ptsize);
	auto s = TTF_RenderText_Blended(font, this->text.c_str(), this->color);
	this->surface = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_RGBA32, 0);
	
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->surface->w, this->surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->surface->pixels);
}

void TextNode::setText(const std::string& text){
	this->text = text;
	this->rebuild();
}

void TextNode::setColor(SDL_Color color){
	this->color = color;
	this->rebuild();
}

TextNode::~TextNode(){
	glDeleteTextures(1, &this->texture);
	SDL_FreeSurface(this->surface);
}

