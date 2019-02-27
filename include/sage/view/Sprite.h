#ifndef _SAGE_SPRITE_H
#define _SAGE_SPRITE_H

#include <sage/view/TexturedNode.h>

#include <string>
#include <vector>
#include <initializer_list>
#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>

namespace sage{
	class Sprite : public TexturedNode{
	public:
		Sprite(const std::string&);
		Sprite(const std::string&, glm::vec2 size);
		Sprite(const std::string&, glm::vec2 size, std::initializer_list<glm::vec2> coords);
		
		void init() override;
		
		glm::vec2 getSize() override;
		
		int addFrame(glm::vec2 pos);
		void setFrame(int frameId);
		int getFrame();
		
		~Sprite();
	protected:
		GLuint getTexture() override;
		std::vector<glm::vec2> getTexCoords() override;
	private:
		std::string textureFile;
		glm::vec2 frameSize;
		int currentFrame;
		std::vector<glm::vec2> frames;
	};
}

#endif // _SAGE_SPRITE_H
