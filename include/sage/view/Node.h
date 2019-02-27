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
		
		virtual Scene* getScene();
		virtual Application* getApplication();
		
		void addChild(Node*);
		
		void setPosition(float x, float y);
		void setPosition(glm::vec2 pos);
		void setPositionX(float x);
		void setPositionY(float y);
		glm::vec2 getPosition() const;
		float getPositionX() const;
		float getPositionY() const;
		
		void setAnchor(float x, float y);
		void setAnchor(glm::vec2 anchor);
		glm::vec2 getAnchor() const;
		float getAnchorX() const;
		float getAnchorY() const;
		
		void setScale(float);
		void setScale(glm::vec2);
		void setScaleX(float);
		void setScaleY(float);
		glm::vec2 getScale() const;
		float getScaleX() const;
		float getScaleY() const;
		
		void setRotation(float);
		float getRotation() const;
		
		void setZIndex(int);
		int getZIndex() const;
		
		virtual glm::vec2 getSize() = 0;
		float getWidth();
		float getHeight();
				
		std::vector<glm::vec2> getAbsPoints();
		
		virtual ~Node();
	protected:	
		Node* getParentNode() const;
		void setParentNode(Node* parent);
		
		void sortChildren();
		virtual glm::vec2 absPoint(glm::vec2);
		
		virtual int getDepth();
	
		bool initialized = false;
		Node* parentNode = nullptr;
		std::vector<Node*> childNodes;
		glm::vec2 pos;
		glm::vec2 anchor;
		glm::vec2 scale;
		float rotation;
		int z_index;
	};

}

#endif // _SAGE_NODE_H
