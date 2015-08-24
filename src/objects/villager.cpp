#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "villager.hpp"
#include "../main.hpp"
#include "../game.hpp"

ALLEGRO_BITMAP* Villager::sprite = NULL;
ALLEGRO_BITMAP* Villager::flames_spawn = NULL;
ALLEGRO_BITMAP* Villager::flames_loop = NULL;
TriObject Villager::fov(10*32., 5*32., PI);

Villager::Villager(glm::vec2 spawnPosition, Game &game): BoxObject(glm::vec2(30, 30)), game(game) {
	this->position = spawnPosition;
	this->status = NONE;
	this->statusDate = 0L;

	if (this->sprite == NULL) {
		ALLEGRO_PATH *basedir;
		basedir = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(basedir, RESOURCES_DIR);

		ALLEGRO_PATH *file = al_clone_path(basedir);
		al_set_path_filename(file, "villager.png");
		sprite = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		file = al_clone_path(basedir);
		al_set_path_filename(file, "flames-spawn.png");
		flames_spawn = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		file = al_clone_path(basedir);
		al_set_path_filename(file, "flames_loop.png");
		flames_loop = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		al_destroy_path(basedir);
	}
}

void Villager::setStatus(Villager::Status status) {
	this->status = status;
	this->statusDate = (long)(al_get_time() * 1000);
}

void Villager::chaseSwampman() {
	gotoPosition(game.swampman_->getPosition());
	step();
}

void Villager::fleeSwampman() {
	gotoPosition(this->position - (game.swampman_->getPosition() - this->position));
	step();
}

void Villager::update() {
	// Set various parameters according to environment
	if (game.map_.what(this->position.x, this->position.y) == SWAMP) {
		this->speed = 1;
	} else {
		this->speed = 2;
	}

	Villager::fov.setPosition(this->position);
	Villager::fov.setOrientation(this->orientation);

	// IA code
	if (status == NONE) {
		if (Villager::fov.collide(*(this->game.swampman_))) {
			if ((char)(al_get_time()*1000.) & 0x01) {
				setStatus(CHASING);
				chaseSwampman();
			}
			else {
				setStatus(FLEEING);
				fleeSwampman();
			}
		}
	}

	else if (status == CHASING) {
		if (Villager::fov.collide(*(this->game.swampman_))) {
			chaseSwampman();
		}
		else {
			setStatus(NONE);
		}
	}

	else if (status == FLEEING) {
		if ((long)(al_get_time() * 1000) > this->statusDate+2000) {
			setStatus(NONE);
		}
		else {
			fleeSwampman();
		}
	}
}

void Villager::draw(glm::vec2 screen_ul_corner) {
	al_draw_rotated_bitmap(sprite, 16, 16,
	                       this->position.x - screen_ul_corner.x,
	                       this->position.y - screen_ul_corner.y,
	                       orientation+PI, 0);
#ifdef _DEBUG
	drawHull(screen_ul_corner);

	Villager::fov.setPosition(this->position);
	Villager::fov.setOrientation(this->orientation);
	Villager::fov.drawHull(screen_ul_corner);
#endif // _DEBUG
}

void Villager::step() {
	Object::step();
	if (this->position.x < 16) this->position.x = 16;
	if (this->position.y < 16) this->position.y = 16;

	if (this->position.x > (this->game.map_.getWidth()  * 32 - 16)) this->position.x = (this->game.map_.getWidth()  * 32 - 16);
	if (this->position.y > (this->game.map_.getHeight() * 32 - 16)) this->position.y = (this->game.map_.getHeight() * 32 - 16);
}

