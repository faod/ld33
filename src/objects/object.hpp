#pragma once
#ifndef OBJ_HPP
#define OBJ_HPP

#include <glm/glm.hpp>

class ConvexHull {
public:
	int len;
	glm::vec2 *points;

	ConvexHull(int len, glm::vec2 *points);
	ConvexHull(int len);
	ConvexHull(ConvexHull &toCopy);
	~ConvexHull();

	void translate(glm::vec2 tr);
	void rotate(float orient); /// North counter-clock-wise rotation in radians

	bool intersects(ConvexHull &other);
};

// ----

/// Interactive objects (move and/or collide)
class Object {
protected:
	ConvexHull convexHull; /// Convex hull for this object

	glm::vec2 position; /// (x, y)

	float orientation; /// North counter-clock-wise rotation in radians

	float speed; /// Velocity of this object in pixels

public:

	/// Construct for objects with no collision
	Object(): convexHull(0), position(0.), speed(0.) {};

	/// Objects with a ConvexHull to compute collisions
	Object(ConvexHull &convexHull): convexHull(convexHull), position(0.), speed(0.) {};

	/// Returns the global position in pixels
	virtual glm::vec2 getPosition();

	/// Append the given coordinates to this object's position
	virtual void updatePosition(glm::vec2 &delta);

	/// Moves this object in the current orientation at the current speed
	virtual void step();

	/// Sets the global position
	virtual void setPosition(glm::vec2 &pos);

	/// Sets the speed for this object
	virtual void setSpeed(float speed);

	/// Returns `true` if the given object collides with this object
	virtual bool collide(Object &other);

	/// Same as `collide`, but use the next position of this object according to its speed, position and orientation
	virtual bool willCollide(Object &other);

	/// Returns the convex hull for this object, rotated and translated
	/// For collision calculation purposes
	virtual ConvexHull* getConvexHull();

	/// Returns the axis aligned bounding box for this object (x, y, width, height)
	virtual glm::vec4 getBoundingBox();

    virtual void update();

    virtual void draw(glm::vec2 screen_ul_corner);

    virtual void drawHull(glm::vec2 screen_ul_corner);

    virtual ~Object() {};
};

// ----

class BoxObject: public Object {
public:
	BoxObject(glm::vec2 wh);
};

#endif // OBJ_HPP
