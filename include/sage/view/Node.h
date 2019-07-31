#ifndef _SAGE_NODE_H
#define _SAGE_NODE_H

#include <sage/view/Application.h>

#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace sage{
	class Scene;
	
	/**
	 * @brief Abstract class that describes all things drawn by the Scene.
	 * The Scene itself, which is also a Node, is drawn by the Application.
	 * Every Node has children (Nodes) which are affected by the Node's properties. 
	 */
	class Node{
	public:
		/**
		 * @brief Node constructor.
		 * Initializes the Node with position (0,0,0), anchor (0,0,0), scale (1,1,1), rotation (0,0,0)
		 */
		Node();
		/**
		 * @brief Updates the Node and its children.
		 * Updates are required to set the Node's postion, scale, etc.
		 * @param delta the time since the last update
		 */
		virtual void update(float delta);
		/**
		 * @brief Fetches the Scene that contains this Node.
		 * @return the Scene
		 */
		virtual Scene& getScene();
		/**
		 * @brief Fetches the Application.
		 * @return the Application
		 */
		virtual Application& getApplication();
		/**
		 * @brief Adds the given Node as a child.
		 * @param node the new child-Node
		 */
		void addChild(std::shared_ptr<Node> node);
		/**
		 * @brief Sets the position of the Node.
		 * @param x the new x-coordinate
		 * @param y the new y-coordinate
		 * @param z the new z-coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPosition(float x, float y)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see setPositionZ(float z)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		void setPosition(float x, float y, float z);
		/**
		 * @brief Sets the position of the Node.
		 * @param pos the new position
		 * @see pos
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPosition(float x, float y)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see setPositionZ(float z)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 */
		void setPosition(glm::vec3 pos);
		/**
		 * @brief Sets the position of the Node.
		 * @param x the new x-coordinate
		 * @param y the new y-coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see setPositionZ(float z)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		void setPosition(float x, float y);
		/**
		 * @brief Sets the position of the Node.
		 * @param pos the new position
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see setPositionZ(float z)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		void setPosition(glm::vec2 pos);
		/**
		 * @brief Sets the x-coordinate of the Node.
		 * @param x the new x-coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionY(float y)
		 * @see setPositionZ(float z)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		void setPositionX(float x);
		/**
		 * @brief Sets the y-coordinate of the Node.
		 * @param y the new y-coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionZ(float z)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 */
		void setPositionY(float y);
		/**
		 * @brief Sets the z-coordinate of the Node.
		 * @param z the new z-coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		void setPositionZ(float z);
		/**
		 * @brief Fetches the position of the Node.
		 * @return the position
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see getPositionX()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		glm::vec3 getPosition() const;
		/**
		 * @brief Fetches the x-coordinate of the Node.
		 * @return the x coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see getPosition()
		 * @see getPositionY()
		 * @see getPositionZ()
		 */
		float getPositionX() const;
		/**
		 * @brief Fetches the y-coordinate of the Node.
		 * @return the y coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionZ()
		 */
		float getPositionY() const;
		/**
		 * @brief Fetches the y-coordinate of the Node.
		 * @return the y coordinate
		 * @see pos
		 * @see setPosition(glm::vec3 pos)
		 * @see setPosition(float x, float y, float z)
		 * @see setPosition(float x, float y)
		 * @see setPosition(glm::vec2 pos)
		 * @see setPositionX(float x)
		 * @see setPositionY(float y)
		 * @see getPosition()
		 * @see getPositionX()
		 * @see getPositionY()
		 */
		float getPositionZ() const;
		/**
		 * @brief Sets the anchor of the Node.
		 * See anchor for further details.
		 * @param x the relative anchor x-coordinate
		 * @param y the relative anchor y-coordinate
		 * @param z the relative anchor z-coordinate
		 * @see anchor
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see setAnchor(float x, float y)
		 * @see getAnchor()
		 * @see getAnchorX()
		 * @see getAnchorY()
		 * @see getAnchorZ()
		 */
		void setAnchor(float x, float y, float z);
		/**
		 * @brief Sets the anchor of the Node.
		 * See anchor for further details.
		 * @param anchor the anchor
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see setAnchor(float x, float y)
		 * @see getAnchor()
		 * @see getAnchorX()
		 * @see getAnchorY()
		 * @see getAnchorZ()
		 */
		void setAnchor(glm::vec3 anchor);
		/**
		 * @brief Sets the anchor of the Node.
		 * See anchor for further details.
		 * @param x the relative anchor x-coordinate
		 * @param y the relative anchor y-coordinate
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see getAnchor()
		 * @see getAnchorX()
		 * @see getAnchorY()
		 * @see getAnchorZ()
		 */
		void setAnchor(float x, float y);
		/**
		 * @brief Sets the anchor of the Node.
		 * See anchor for further details.
		 * @param anchor the anchor
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(float x, float y)
		 * @see getAnchor()
		 * @see getAnchorX()
		 * @see getAnchorY()
		 * @see getAnchorZ()
		 */
		void setAnchor(glm::vec2 anchor);
		/**
		 * @brief Fetches the anchor of the Node.
		 * See anchor for further details.
		 * @return the anchor
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(float x, float y)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see getAnchorX()
		 * @see getAnchorY()
		 */
		glm::vec3 getAnchor() const;
		/**
		 * @brief Fetches the relative anchor x-coordinate of the Node.
		 * See anchor for further details.
		 * @return the anchor's x-coordinate
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(float x, float y)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see getAnchor()
		 * @see getAnchorY()
		 * @see getAnchorZ()
		 */
		float getAnchorX() const;
		/**
		 * @brief Fetches the relative anchor y-coordinate of the Node.
		 * See anchor for further details.
		 * @return the anchor's y-coordinate
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(float x, float y)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see getAnchor()
		 * @see getAnchorX()
		 * @see getAnchorZ()
		 */
		float getAnchorY() const;
		/**
		 * @brief Fetches the relative anchor y-coordinate of the Node.
		 * See anchor for further details.
		 * @return the anchor's y-coordinate
		 * @see anchor
		 * @see setAnchor(float x, float y, float z)
		 * @see setAnchor(glm::vec3 anchor)
		 * @see setAnchor(float x, float y)
		 * @see setAnchor(glm::vec2 anchor)
		 * @see getAnchor()
		 * @see getAnchorX()
		 * @see getAnchorY()
		 */
		float getAnchorZ() const;
		/**
		 * @brief Sets the scale-factor of the Node.
		 * @param s the x- and y- scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see setScaleZ(float z)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 */
		void setScale(float s);
		/**
		 * @brief Sets the scale-factor of the Node.
		 * @param s the scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see setScaleZ(float z)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		void setScale(glm::vec2 s);
		/**
		 * @brief Sets the scale-factor of the Node.
		 * @param s the scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see setScaleZ(float z)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		void setScale(glm::vec3 s);
		/**
		 * @brief Sets the scale-factor in x direction of the Node.
		 * @param x the x scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleY(float y)
		 * @see setScaleZ(float z)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		void setScaleX(float x);
		/**
		 * @brief Sets the scale-factor in y direction of the Node.
		 * @param y the y scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleZ(float z)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		void setScaleY(float y);
		/**
		 * @brief Sets the scale-factor in z direction of the Node.
		 * @param z the z scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		void setScaleZ(float z);
		/**
		 * @brief Fetches the scale-factor of the Node.
		 * @return the scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see getScaleX()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		glm::vec3 getScale() const;
		/**
		 * @brief Fetches the scale-factor in x direction of the Node.
		 * @return the x scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see getScale()
		 * @see getScaleY()
		 * @see getScaleZ()
		 */
		float getScaleX() const;
		/**
		 * @brief Fetches the scale-factor in y direction of the Node.
		 * @return the y scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleZ()
		 */
		float getScaleY() const;
		/**
		 * @brief Fetches the scale-factor in y direction of the Node.
		 * @return the y scale
		 * @see scale
		 * @see setScale(float s)
		 * @see setScale(glm::vec2 s)
		 * @see setScale(glm::vec3 s)
		 * @see setScaleX(float x)
		 * @see setScaleY(float y)
		 * @see getScale()
		 * @see getScaleX()
		 * @see getScaleY()
		 */
		float getScaleZ() const;
		/**
		 * @brief Sets the rotation of the Node around the z-axis.
		 * @param r the rotation
		 * @see rotation
		 * @see setRotation(glm::vec3 r)
		 * @see getRotation()
		 */
		void setRotation(float r);
		/**
		 * @brief Sets the rotation of the Node around the z-axis.
		 * @param r the rotation
		 * @see rotation
		 * @see setRotation(float r)
		 * @see getRotation()
		 */
		void setRotation(glm::vec3 r);
		/**
		 * @brief Fetches the rotation of the Node.
		 * @return the rotation
		 * @see rotation
		 * @see setRotation(float r)
		 * @see setRotation(glm::vec3 r)
		 */
		glm::vec3 getRotation() const;
		/**
		 * @brief Fetches the size of the Node.
		 * @return the size
		 * @see getWidth()
		 * @see getHeight()
		 * @see getDepth()
		 */
		virtual glm::vec3 getSize() = 0;
		/**
		 * @brief Fetches the width of the Node.
		 * @return the width
		 * @see getSize()
		 * @see getHeight()
		 * @see getDepth()
		 */
		float getWidth();
		/**
		 * @brief Fetches the height of the Node.
		 * @return the height
		 * @see getSize()
		 * @see getWidth()
		 * @see getDepth()
		 */
		float getHeight();
		/**
		 * @brief Fetches the depth of the Node.
		 * @return the depth
		 * @see getSize()
		 * @see getHeight()
		 * @see getWidth()
		 */
		float getDepth();
		/**
		 * @brief Fetches the corner points of the Node in absolute coordinates (Application coordinates).
		 * @return vector of 8 corner points
		 */
		std::vector<glm::vec3> getAbsPoints();
		/**
		 * @brief Checks if the Node contains a point given in absolute coordinates.
		 * @param point the point to check
		 */
		bool containsAbs(glm::vec3 point);
		/**
		 * @brief Checks if the Node contains a point given in absolute x- and y-coordinates.
		 * @param point the point to check
		 */
		bool containsAbs(glm::vec2 point);
		/**
		 * @brief Fetches the child-Nodes.
		 * @return a vector containing child-Nodes
		 */
		std::vector<std::shared_ptr<Node>> getChildren();
		/**
		 * @brief Node destructor.
		 * Deletes children.
		 */
		virtual ~Node();
	protected:	
		/**
		 * @brief Is called every time the Node's Scene is set as the top Scene.
		 * Recursively initializes its children.
		 */ 
		virtual void init();
		/**
		 * @brief Is called before the Node's Scene is removed from the top.
		 * Recursively deinitializes its children.
		 */ 
		virtual void deinit();
		/**
		 * @brief Sorts the children by their z values.
		 */
		void sortChildren();
		/**
		 * @brief Fetches the parent-Node.
		 * @return the parent Node
		 */
		Node& getParentNode() const;
		/**
		 * @brief Sets the parent-Node.
		 * @param parent the new parent Node
		 */
		void setParentNode(Node& parent);
		/**
		 * @brief Converts a point in relative (Node) coordinates to absolute (Application) coordinates.
		 * @param point a point in relative coordinates
		 */
		virtual glm::vec3 absPoint(glm::vec3 point);
		/**
		 * @brief Converts a point in absolute (Application) coordinates to relative (Node) coordinates.
		 * @param point a point in absolute coordinates
		 */
		virtual glm::vec3 relPoint(glm::vec3 point);
		/**
		 * @brief Fetches the depth of a Node under the Scene.
		 * e.g. (Scene: depth 0)->(Node: depth 1)->(Node: depth 2) etc.
		 * @return the depth
		 */
		virtual int getTreeDepth();
		/**
		 * @brief Fetches childOrderChanged
		 * @see setChildOrderChanged()
		 * @see childOrderChanged
		 */
		bool getChildOrderChanged();
		/**
		 * @brief Sets childOrderChanged to true
		 * @see getChildOrderChanged();
		 * @see childOrderChanged
		 */
		void setChildOrderChanged();
		
		bool initialized = false; /**< Whether the Node is initialized and therefore the Application available */
		Node* parentNode = nullptr; /**< The parent Node, is set as soon as the Node is added as a child of another Node */
		std::vector<std::shared_ptr<Node>> childNodes; /**< The vector of children */
		glm::vec3 pos; /**< The position of the Node in parent-Node coordinates*/
		glm::vec3 anchor; /**< The anchor is a relative point around which all transformations and other manipulations take place. (0,0,0) is near-bottom-left and (1,1,1) is far-top-right. */
		glm::vec3 scale; /**< The scale of the Node */
		glm::vec3 rotation; /**< The rotation of the Node around the x-,y- and z-axes*/
		bool childOrderChanged = false; /**< Whether the order of the child-Nodes has potentially changed (child's z-position changed or new child added)*/
		
		friend class BasicRenderer;
	};

}

#endif // _SAGE_NODE_H
