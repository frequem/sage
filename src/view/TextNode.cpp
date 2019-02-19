#include <sage/view/TextNode.h>
#include <sage/util/macros.h>
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
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, this->getDepth()-1, ~0);
	glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
	
    GLuint p = this->getApplication()->getShaderCache()->get("textured2d");
    glUseProgram(p);
    	
	glm::vec2 windowSize = this->getApplication()->getWindowSize();
	glUniform2fv(glGetUniformLocation(p, "windowSize"), 1, glm::value_ptr(windowSize));
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
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
	
	Node::render(); //render children
	
	//undo stencil
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

glm::vec2 TextNode::getSize(){ return glm::vec2(this->surface->w, this->surface->h); }

TextNode::~TextNode(){
	glDeleteTextures(1, &this->texture);
	SDL_FreeSurface(this->surface);
}

