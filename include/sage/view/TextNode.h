#ifndef _SAGE_TEXTNODE_H
#define _SAGE_TEXTNODE_H

#include <sage/view/Node.h>
#include <string>

#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <glm/glm.hpp>

namespace sage{
	class TextNode : public Node{
	public:
		TextNode(const std::string& font, int ptsize);
		TextNode(const std::string& font, int ptsize, const std::string& text);
		TextNode(const std::string& font, int ptsize, const std::string& text, SDL_Color color);
		void render() override;
		void update(float) override;
		void init() override;
		
		glm::vec2 getSize() override;
		
		void setText(const std::string& text);
		void setColor(SDL_Color color);
		~TextNode();
	protected:
		void rebuild();
		std::string fontFile;
		int ptsize;
		std::string text;
		SDL_Color color;
		
		SDL_Surface* surface;
		GLuint texture;
	};
}

#endif // _SAGE_TEXTNODE_H
