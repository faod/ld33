#pragma once
#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include "object.hpp"
#include "swampman.hpp"
#include "../misc/defines.hpp"

#include <allegro5/allegro.h>

class Villager: public BoxObject {
	static ALLEGRO_BITMAP *sprite, *flames_spawn, *flames_loop;
	static TriObject fov;
	Game &game;
	enum {
		NONE,
		DRYING,
		CHASING,
		ELOPING,
		ROAMING,
		FIRING
	} status;

public:
	Villager(glm::vec2 spawnPosition, Game &game);
	virtual void update();
	virtual void draw(glm::vec2 screen_ul_corner);
};

#endif // VILLAGER_HPP
