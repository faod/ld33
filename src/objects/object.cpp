#include "object.hpp"

glm::vec2 Object::getPosition() {
	return this->position;
}

void Object::updatePosition(glm::vec2 &delta) {
	this->position += delta;
}

void Object::setPosition(glm::vec2 &pos) {
	this->position = pos;
}

bool Object::collide(Object &other) { // TODO
	return false;
}

ConvexHull* Object::getConvexHull() { // TODO
	return NULL;
}

glm::vec4 Object::getBoundingBox() { // TODO
	glm::vec4 res;
	return res;
}

void Object::update()
{
    //empty
}

void Object::draw()
{
    //empty
}
