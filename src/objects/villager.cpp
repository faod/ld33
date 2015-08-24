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

void Villager::update() {
	// Set various parameters according to environment
	if (game.map_.what(this->position.x, this->position.y) == SWAMP) {
		this->speed = 2;
	} else {
		this->speed = 5;
	}

	// IA code
	if (status == NONE) {
		
	}
}

void Villager::draw(glm::vec2 screen_ul_corner) {
	al_draw_rotated_bitmap(sprite, 16, 16,
	                       this->position.x - screen_ul_corner.x,
	                       this->position.y - screen_ul_corner.y,
	                       orientation+PI, 0);

	drawHull(screen_ul_corner);

	Villager::fov.setPosition(this->position);
	Villager::fov.setOrientation(this->orientation);
	Villager::fov.drawHull(screen_ul_corner);
}

