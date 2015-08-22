#pragma once
#ifndef OBJ_HPP
#define OBJ_HPP

#include <glm/glm.hpp>

struct ConvexHull {
	int len;
	glm::vec2 *points;
};

class Object {
protected:
	glm::vec2 position;
public:
	/// Returns the global position in pixels
	virtual glm::vec2 getPosition();

	/// Append the given coordinates to this object's position
	virtual void updatePosition(glm::vec2 &delta);

	/// Sets the global position
	virtual void setPosition(glm::vec2 &pos);

	/// Returns `true` if the given object collides with this object
	virtual bool collide(Object &other);

	/// Returns the convex hull for this object for collision calculation purposes
	virtual ConvexHull* getConvexHull();

	/// Returns the axis aligned bounding box for this object
	virtual glm::vec4 getBoundingBox();
};

#endif // OBJ_HPP
