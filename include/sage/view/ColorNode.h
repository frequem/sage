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
		ColorNode(glm::vec2 size, glm::vec4 color);
		
		glm::vec3 getSize() override;
		void setColor(glm::vec4 color);
		glm::vec4 getColor();
		
		void render(Renderer& r) override;
		
		~ColorNode();
	protected:
		void init() override;
	private:
		glm::vec2 size;
		glm::vec4 color;
	};
}

#endif // _SAGE_COLORNODE_H
