#ifndef _SAGE_DEPTHPEELRENDERER_H
#define _SAGE_DEPTHPEELRENDERER_H

#include <sage/renderer/Renderer.h>
#include <sage/view/TexturedNode.h>
#include <sage/view/Sprite.h>
#include <glm/glm.hpp>

namespace sage{
	class Application;
	class DepthPeelRenderer : public Renderer{
	public:
		DepthPeelRenderer(Application&);
		DepthPeelRenderer(Application&, int peels);
		
		void render() override;
		
		void renderSingle(TexturedNode& tn) override;
		void renderSingle(ColorNode& tn) override;
		~DepthPeelRenderer();
	protected:		
		void initBuffers();
		void deinitBuffers();
		
		int peels;
		int currentPass;
		
		glm::vec2 oldFrameSize;
		GLuint* frameBuffers;
		GLuint* frameTextures;
		GLuint* depthTextures;
	};
}

#endif // _SAGE_DEPTHPEELRENDERER_H
