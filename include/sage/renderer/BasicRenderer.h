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
		
		void renderSingle(TexturedNode& tn) override;
		~BasicRenderer();
	};
}

#endif // _SAGE_BASICRENDERER_H
