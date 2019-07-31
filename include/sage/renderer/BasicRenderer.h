#ifndef _SAGE_BASICRENDERER_H
#define _SAGE_BASICRENDERER_H

#include <sage/renderer/Renderer.h>
#include <sage/view/TexturedNode.h>
#include <sage/view/Sprite.h>

namespace sage{
	class BasicRenderer : public Renderer{
	public:
		using Renderer::Renderer;
		
		void render() override;
		
		~BasicRenderer();
	protected:
		void render(Node& n) override;
		void renderSingle(TexturedNode& tn) override;
		void renderSingle(ColorNode& tn) override;
	};
}

#endif // _SAGE_BASICRENDERER_H
