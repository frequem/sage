#ifndef _SAGE_SPRITE_H
#define _SAGE_SPRITE_H

#include <sage/view/Node.h>

#include <string>
#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SOIL/SOIL.h>

namespace sage{
	class Sprite : public Node{
	public:
		Sprite(const std::string&);
		void render() override;
		void update(float) override;
		void init() override;
		~Sprite();
	private:
		GLuint texture;
		glm::vec2 size;
		std::string textureFile;
	};
}

#endif // _SAGE_SPRITE_H
