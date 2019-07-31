#ifndef _SAGE_COLORNODE_H
#define _SAGE_COLORNODE_H

#include <sage/view/Node.h>
#include <string>

#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <glm/glm.hpp>

namespace sage{
	class ColorNode : public Node{
	public:
		ColorNode(glm::vec2 size);
		ColorNode(glm::vec2 size, SDL_Color color);
		
		glm::vec3 getSize() override;
		
		void setColor(SDL_Color color);
		~ColorNode();
	protected:
		void init() override;
	private:
		glm::vec2 size;
		SDL_Color color;
	};
}

#endif // _SAGE_COLORNODE_H
