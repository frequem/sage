#ifndef _SAGE_SCENE_H
#define _SAGE_SCENE_H

#include <sage/view/Node.h>

#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <glm/glm.hpp>

namespace sage{
	class Application;
	
	class Scene : public Node{
	public:
		Scene();
		void render() override;
		Scene* getScene() override;
		void setApplication(Application*);
		Application* getApplication() override;
		
		glm::vec2 getSize() override;
		
		~Scene();
	protected:
		Application* application;
		
		glm::vec2 absPoint(glm::vec2) override;
		int getDepth() override;
	};
}
#endif // _SAGE_SCENE_H
