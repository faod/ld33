#pragma once
#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include "object.hpp"
#include "swampman.hpp"
#include "../misc/defines.hpp"

#include <memory>
#include <allegro5/allegro.h>

/// A flamethrower's flame
class Flame: public BoxObject, public std::enable_shared_from_this<Flame> {
	static ALLEGRO_BITMAP *flames_spawn, *flames_loop;

	Game& game;
	long startTime;

public:
	Flame(glm::vec2 position, Game &g, float orientation);

	virtual void draw(glm::vec2 screen_ul_corner);
	virtual void update();
};

// ----

class Villager: public Character, public std::enable_shared_from_this<Villager> {
	static ALLEGRO_BITMAP *sprite;
	static TriObject fov;

	enum Status {
		NONE,
		DRYING,
		CHASING,
		FLEEING,
		ROAMING,
		FIRING
	} status;
	long statusDate; // since date of the current status in milliseconds
	int hp; // health
	std::shared_ptr<Flame> flame; // Not null if there is a flame

	void setStatus(Villager::Status status);
	void chaseSwampman();
	void fleeSwampman();

public:
	Villager(glm::vec2 spawnPosition, Game &game);
	virtual void update();
	virtual void draw(glm::vec2 screen_ul_corner);
};

#endif // VILLAGER_HPP
