#ifndef _SAGE_SCENE_H
#define _SAGE_SCENE_H

#include <sage/Node.h>

#include <SDL2/SDL.h>
#include <sage/ogl.h>
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
		~Scene();
	protected:
		glm::vec2 absPoint(glm::vec2) override;
		Application* application;
	};
}
#endif // _SAGE_SCENE_H
