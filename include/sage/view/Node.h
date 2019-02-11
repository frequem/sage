#ifndef _SAGE_NODE_H
#define _SAGE_NODE_H

#include <sage/view/Application.h>
#include <vector>
#include <glm/glm.hpp>

namespace sage{
	class Scene;
	
	class Node{
	public:
		Node();
		virtual void render();
		virtual void update(float);
		
		virtual void init();
		
		Node* getParentNode();
		void setParentNode(Node* parent);
		
		virtual Scene* getScene();
		virtual Application* getApplication();
		
		void addChild(Node*);
		
		void setPosition(float x, float y);
		void setPosition(glm::vec2 pos);
		void setPositionX(float x);
		void setPositionY(float y);
		glm::vec2 getPosition();
		float getPositionX();
		float getPositionY();
		
		void setAnchor(float x, float y);
		void setAnchor(glm::vec2 anchor);
		glm::vec2 getAnchor();
		float getAnchorX();
		float getAnchorY();
		
		void setScale(float);
		void setScale(glm::vec2);
		void setScaleX(float);
		void setScaleY(float);
		glm::vec2 getScale();
		float getScaleX();
		float getScaleY();
		
		void setRotation(float);
		float getRotation();
		
		virtual ~Node();
	protected:
		virtual glm::vec2 absPoint(glm::vec2);
	
		bool initialized = false;
		Node* parentNode;
		std::vector<Node*> childNodes;
		glm::vec2 pos;
		glm::vec2 anchor;
		glm::vec2 scale;
		float rotation;
	};

}

#endif // _SAGE_NODE_H
