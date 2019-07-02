#ifndef _SAGE_RENDERER_H
#define _SAGE_RENDERER_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace sage{
	class Node;
	class TexturedNode;
	class Application;
	class Renderer{
	public:
		Renderer(Application&);
		virtual void render();
		virtual ~Renderer();
	protected:
		template <typename T>
		bool tryRenderSingle(Node& n);
		void tryRenderSingles(Node& n);
		virtual void render(Node& n);
		void render(std::vector<std::shared_ptr<Node>>);
		
		virtual void renderSingle(TexturedNode& tn) = 0;
		
		Application* application;
	};
	
	#include <sage/renderer/Renderer.tpp>
}


#endif // _SAGE_RENDERER_H
