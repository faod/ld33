#include <allegro5/allegro_primitives.h>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cfloat>
#define GLM_SWIZZLE
#include <glm/glm.hpp>

#include "../failure.hpp"
#include "../misc/defines.hpp"
#include "object.hpp"

using std::malloc;
using std::memcpy;
using std::free;
using std::cos;
using std::sin;
using std::tan;

ConvexHull::ConvexHull(int len) {
	this->len = len;
	this->points = NULL;
	if (len) {
		this->points = (glm::vec2*)malloc(len * sizeof(glm::vec2));
		if (!this->points) {
			throw Failure("Failed to alloc");
		}
	}
}

ConvexHull::ConvexHull(int len, glm::vec2 *points): ConvexHull(len) {
	if (len) {
		memcpy(this->points, points, len * sizeof(glm::vec2));
	}
}

ConvexHull::ConvexHull(ConvexHull &toCopy): ConvexHull(toCopy.len, toCopy.points) { }

ConvexHull::~ConvexHull() {
	free(this->points);
}

void ConvexHull::translate(glm::vec2 tr) {
	for (int i=0; i<this->len; i++) {
		this->points[i] += tr;
	}
}

void ConvexHull::rotate(float orient) {
	if (orient != 0) {
		float s, c;
		c = cos(orient);
		s = sin(orient);
		glm::mat2 rotm(c, s, -s, c);

		for (int i=0; i<this->len; i++) {
			this->points[i] = rotm * this->points[i];
		}
	}
}

bool ConvexHull::intersects(ConvexHull &other) {
	bool isIn;
	float sign;
	int i, j;
	glm::vec2 pA, pB, pC;
	glm::vec2 vAB, vAC;

	// for each points of the other hull
	for (i=0; i<other.len; i++) {
		// test if the point is at the right (-1) or at the left (+1)
		// of a segment made from two points of this hull
		pA = this->points[this->len-1];
		pB = this->points[0];
		pC = other.points[i];

		vAB = pB - pA;
		vAC = pC - pA;

		sign = m_sign(vAB.x * vAC.y - vAB.y * vAC.x);
		isIn = true;

		// for each segment of this hull
		for (j=0; j<this->len-1; j++) {
			// if the point is not at the same side of each segment
			pA = this->points[j];
			pB = this->points[j+1];

			vAB = pB - pA;
			vAC = pC - pA;

			if (sign != m_sign(vAB.x * vAC.y - vAB.y * vAC.x)) {
				isIn = false; // it is not inside this hull
				break;
			}
		}
		if (isIn) {
			return true;
		}
	}
	return false;
}

// ----

glm::vec2 Object::getPosition() {
	return this->position;
}

void Object::updatePosition(glm::vec2 delta) {
	this->position += delta;
}

void Object::gotoPosition(glm::vec2 togo) {
	glm::vec2 v = glm::normalize(togo - this->position);
	this->orientation = glm::acos(glm::dot(glm::vec2(0., 1.), v));
	this->orientation *= -m_sign(v.x);
}

void Object::step() {
	this->position += glm::vec2(-sin(this->orientation), cos(this->orientation)) * speed;
}

void Object::setPosition(glm::vec2 pos) {
	this->position = pos;
}

void Object::setSpeed(float speed) {
	this->speed = speed;
}

void Object::setOrientation(float orient) {
	this->orientation = orient;
}

float Object::getOrientation() {
	return this->orientation;
}

bool Object::collide(Object &other) {
	ConvexHull *c1 = getConvexHull();
	ConvexHull *c2 = other.getConvexHull();
	bool res = c1->intersects(*c2);
	delete c1; delete c2;
	return res;
}

bool Object::willCollide(Object &other) {
	ConvexHull *c1 = new ConvexHull(this->convexHull);
	ConvexHull *c2 = other.getConvexHull();
	c1->rotate(this->orientation);
	c1->translate(this->position + glm::vec2(-sin(this->orientation), cos(this->orientation)) * speed);
	bool res = c1->intersects(*c2);
	delete c1; delete c2;
	return res;
}

ConvexHull* Object::getConvexHull() {
	ConvexHull *res = new ConvexHull(this->convexHull);
	res->rotate(this->orientation);
	res->translate(this->position);
	return res;
}

glm::vec4 Object::getBoundingBox() {
	if (!this->convexHull.len) {
		return glm::vec4(0.);
	}

	float min_x, max_x, min_y, max_y;
	max_x = max_y = FLT_MIN;
	min_x = min_y = FLT_MAX;

	glm::vec2 v;
	for (int i=0; i<this->convexHull.len; i++) {
		v = this->convexHull.points[i];
		if (v.x > max_x) max_x = v.x;
		if (v.x < min_x) min_x = v.x;

		if (v.y > max_y) max_y = v.y;
		if (v.y < min_y) min_y = v.y;
	}

	glm::vec4 res(min_x,         min_y,
	              max_x - min_x, max_y - min_y);
	return res;
}

void Object::update()
{
    //empty
}

void Object::draw(glm::vec2 screen_ul_corner)
{
    //empty
}

void Object::drawHull(glm::vec2 screen_ul_corner) {
	ConvexHull *hull = getConvexHull();
	ALLEGRO_COLOR col = al_map_rgb(0, 0, 255);

	for (int i=0; i<hull->len-1; i++) {
		glm::vec2 p1 = hull->points[i]   - screen_ul_corner,
		          p2 = hull->points[i+1] - screen_ul_corner;
		al_draw_line(p1.x, p1.y, p2.x, p2.y, col, 2.);
	}
	glm::vec2 p1 = hull->points[0]           - screen_ul_corner,
	          p2 = hull->points[hull->len-1] - screen_ul_corner;
	al_draw_line(p1.x, p1.y, p2.x, p2.y, col, 2.);

	delete hull;
}

// ----

BoxObject::BoxObject(glm::vec2 v) {
	this->convexHull.len = 4;
	this->convexHull.points = (glm::vec2*)malloc(4*sizeof(glm::vec2));
	this->convexHull.points[0] = -(v/2.f);
	this->convexHull.points[1] = glm::vec2(v.x/2., -v.y/2.);
	this->convexHull.points[2] = v/2.f;
	this->convexHull.points[3] = glm::vec2(-v.x/2., v.y/2.);
}

// ----

// Isoscele triangle with its first point at (0, 0)
TriObject::TriObject(float width, float height, float rotation) {
	this->convexHull.len = 3;
	this->convexHull.points = (glm::vec2*)malloc(3*sizeof(glm::vec2));
	this->convexHull.points[0] = glm::vec2(0.);
	this->convexHull.points[1] = glm::vec2(-(width/2.), height);
	this->convexHull.points[2] = glm::vec2( (width/2.), height);
}

