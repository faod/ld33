#pragma once
#ifndef OBJ_HPP
#define OBJ_HPP

#include <glm/glm.hpp>


struct ConvexHull {
	int len;
	glm::vec2 *points;
};

class Object {
public:
	/// Returns the global position in pixels
	virtual glm::vec2 getPosition() = 0;

	/// Sets the global position
	virtual void setPosition(glm::vec2 &pos) = 0;

	/// Append the given coordinates to this object's position
	virtual void updatePosition(glm::vec2 &delta) = 0;

	/// Returns `true` if the given object collides with this object
	virtual bool collide(Object &other) = 0;

	/// Returns the convex hull for this object for collision calculation purposes
	virtual ConvexHull* getConvexHull() = 0;
};

class baseObject : public Object {
	glm::vec2 position;
public:
	/// Returns the global position in pixels
	inline glm::vec2 getPosition() { return position; };

	/// Sets the global position
	inline void setPosition(glm::vec2 &pos) { this->position = pos; };

	/// Append the given coordinates to this object's position
	inline void updatePosition(glm::vec2 &delta) { this->position += delta; };

    /// true if given object collides with this
    virtual bool collide(Object &other) = 0;

    /// return the convex hull for this object
    virtual ConvexHull* getConvexHull() = 0;
};

#endif // OBJ_HPP
