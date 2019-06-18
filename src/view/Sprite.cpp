#include <sage/view/Sprite.h>
#include <sage/util/macros.h>
#include <algorithm>

using namespace sage;


Sprite::Sprite(const std::string& textureFile, glm::vec2 size) : TexturedNode(), textureFile(textureFile), frameSize(size){}

Sprite::Sprite(const std::string& textureFile) : Sprite(textureFile, glm::vec2(0.0, 0.0)){ //0x0 is initialized to full image size
	this->frames.push_back(glm::vec2(0.0, 0.0)); //top left
}

Sprite::Sprite(const std::string& textureFile, glm::vec2 size, std::initializer_list<glm::vec2> coords) : Sprite(textureFile, size){
	this->frames.insert(this->frames.end(), coords.begin(), coords.end());
}

void Sprite::init(){
	if(glm::any(glm::lessThanEqual(this->frameSize, glm::vec2(0.0, 0.0)))){
		this->frameSize = this->getApplication().getImageCache().getSize(this->textureFile);
	}
	
	Node::init();
}

int Sprite::addFrame(glm::vec2 pos){
	this->frames.push_back(pos);
	return this->frames.size()-1;
}

void Sprite::setFrame(int frameId){ this->currentFrame = std::clamp(frameId, 0, static_cast<int>(this->frames.size()-1)); }

int Sprite::getFrame(){ return this->currentFrame; }

glm::vec3 Sprite::getSize(){ return glm::vec3(this->frameSize, 0); }

GLuint Sprite::getTexture(){ return this->getApplication().getImageCache().getTexture(this->textureFile); }

std::vector<glm::vec2> Sprite::getTexCoords(){
	glm::vec2 is = this->getApplication().getImageCache().getSize(this->textureFile); //overall image size
	glm::vec2 fs = this->getSize(); //frame size
	glm::vec2 p = this->frames[this->currentFrame]; //position
	
	std::vector<glm::vec2> temp;
	
	temp.push_back(glm::vec2(p.x/is.x, 			(p.y+fs.y)/is.y));
	temp.push_back(glm::vec2((p.x+fs.x)/is.x, 	(p.y+fs.y)/is.y));
	temp.push_back(glm::vec2(p.x/is.x, 			p.y/is.y));
	temp.push_back(glm::vec2((p.x+fs.x)/is.x, 	p.y/is.y));
	
	return temp;
}

Sprite::~Sprite(){}

