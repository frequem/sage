#include <sage/view/TextNode.h>
#include <sage/util/macros.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sage;

TextNode::TextNode(const std::string& font, int ptsize) : TextNode(font, ptsize, ""){}

TextNode::TextNode(const std::string& font, int ptsize, const std::string& text) : TextNode(font, ptsize, text, glm::vec4(0, 0, 0, 1)){}

TextNode::TextNode(const std::string& font, int ptsize, const std::string& text, glm::vec4 color) : 
	TexturedNode(), fontFile(font), ptsize(ptsize), text(text), color(color){
	glGenTextures(1, &this->texture);
}

void TextNode::init(){
	this->rebuild();
	Node::init();
}

void TextNode::rebuild(){
	TTF_Font* font = this->getApplication().getFontCache().get(this->fontFile, this->ptsize);
	SDL_Color c = { 
		static_cast<uint8_t>(this->color.x*255),
		static_cast<uint8_t>(this->color.y*255),
		static_cast<uint8_t>(this->color.z*255),
		static_cast<uint8_t>(this->color.w*255)
	};
	auto s = TTF_RenderText_Blended(font, this->text.c_str(), c);
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

void TextNode::setColor(glm::vec4 color){
	this->color = color;
	this->rebuild();
}

glm::vec3 TextNode::getSize(){ return glm::vec3(this->surface->w, this->surface->h, 0); }

GLuint TextNode::getTexture(){ return this->texture; }

TextNode::~TextNode(){
	glDeleteTextures(1, &this->texture);
	SDL_FreeSurface(this->surface);
}

