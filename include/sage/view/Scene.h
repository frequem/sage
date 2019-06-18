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
		
		Scene& getScene() override;
		Application& getApplication() override;
		
		glm::vec3 getSize() override;
		
		~Scene();
	protected:
		void setApplication(Application&);
		glm::vec3 absPoint(glm::vec3) override;
		glm::vec3 relPoint(glm::vec3) override;
		int getTreeDepth() override;
		
		Application* application;
		
		friend class Application;
	};
}
#endif // _SAGE_SCENE_H
