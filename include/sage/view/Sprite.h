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
		
		glm::vec2 getSize() override;
		
		~Sprite();
	private:
		std::string textureFile;
	};
}

#endif // _SAGE_SPRITE_H
