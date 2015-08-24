#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "villager.hpp"
#include "../main.hpp"

ALLEGRO_BITMAP* Villager::sprite = NULL;
ALLEGRO_BITMAP* Villager::flames_spawn = NULL;
ALLEGRO_BITMAP* Villager::flames_loop = NULL;

Villager::Villager(glm::vec2 spawnPosition, Game &game): BoxObject(glm::vec2(30, 30)), game(game) {
	this->position = spawnPosition;
	if (this->sprite == NULL) {
		ALLEGRO_PATH *path;

		path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(path, RESOURCES_DIR);
		al_set_path_filename(path, "villager.png");
		sprite = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
	}
}

void Villager::update() {
	// TODO
}

void Villager::draw(glm::vec2 screen_ul_corner) {
	al_draw_rotated_bitmap(sprite, 16, 16,
	                       this->position.x - screen_ul_corner.x,
	                       this->position.y - screen_ul_corner.y,
	                       orientation+PI, 0);

	drawHull(screen_ul_corner);
}

