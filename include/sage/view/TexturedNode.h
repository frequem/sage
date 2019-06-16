#ifndef _SAGE_TEXTURED_NODE_H
#define _SAGE_TEXTURED_NODE_H

#include <sage/view/Node.h>

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SOIL/SOIL.h>

namespace sage{
	class TexturedNode : public Node{
	public:
		TexturedNode();
		void render(int pass) override;
		~TexturedNode();
	protected:
		virtual std::vector<glm::vec2> getTexCoords();
		virtual GLuint getTexture() = 0;
	};
}

#endif // _SAGE_TEXTURED_NODE_H
