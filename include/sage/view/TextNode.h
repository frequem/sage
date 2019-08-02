#ifndef _SAGE_TEXTNODE_H
#define _SAGE_TEXTNODE_H

#include <sage/view/TexturedNode.h>
#include <string>

#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <glm/glm.hpp>

namespace sage{
	class TextNode : public TexturedNode{
	public:
		TextNode(const std::string& font, int ptsize);
		TextNode(const std::string& font, int ptsize, const std::string& text);
		TextNode(const std::string& font, int ptsize, const std::string& text, glm::vec4 color);
		
		glm::vec3 getSize() override;
		
		void setText(const std::string& text);
		void setColor(glm::vec4 color);
		~TextNode();
	protected:
		void init() override;
		GLuint getTexture() override;
		void rebuild();
	private:
		std::string fontFile;
		int ptsize;
		std::string text;
		glm::vec4 color;
		
		SDL_Surface* surface;
		GLuint texture;
	};
}

#endif // _SAGE_TEXTNODE_H
