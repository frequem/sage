#ifndef _SAGE_DEPTHPEELRENDERER_H
#define _SAGE_DEPTHPEELRENDERER_H

#include <sage/renderer/Renderer.h>
#include <sage/view/TexturedNode.h>
#include <sage/view/Sprite.h>
#include <sage/util/config.h>

namespace sage{
	class Application;
	class DepthPeelRenderer : public Renderer{
	public:
		DepthPeelRenderer(Application&);
		
		void render() override;
		
		void renderSingle(TexturedNode& tn) override;
		~DepthPeelRenderer();
	private:		
		int currentPass;
		GLuint frameBuffers[DEPTH_PEELING_PASSES];
		GLuint frameTextures[DEPTH_PEELING_PASSES];
		GLuint depthTextures[2];
	};
}

#endif // _SAGE_DEPTHPEELRENDERER_H
