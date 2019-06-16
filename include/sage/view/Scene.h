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
		void render(int pass) override;
		using Node::render; //get default render function
		
		Scene* getScene() override;
		void setApplication(Application*);
		Application* getApplication() override;
		
		glm::vec3 getSize() override;
		
		~Scene();
	protected:
		Application* application;
		
		glm::vec3 absPoint(glm::vec3) override;
		glm::vec3 relPoint(glm::vec3) override;
		int getTreeDepth() override;
	};
}
#endif // _SAGE_SCENE_H
