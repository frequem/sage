#ifndef _SAGE_RENDERER_H
#define _SAGE_RENDERER_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace sage{
	class Node;
	class TexturedNode;
	class ColorNode;
	class Application;
	
	class Renderer{
	public:
		Renderer(Application&);
		virtual void render();
		
		virtual void renderSingle(TexturedNode& tn) = 0;
		virtual void renderSingle(ColorNode& tn) = 0;
		
		virtual ~Renderer();
	protected:
		virtual void render(Node& n);
		void render(std::vector<std::shared_ptr<Node>>);
		Application* application;
	};
}


#endif // _SAGE_RENDERER_H
