#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include "../misc/defines.hpp"
#include "ball.hpp"
#include "../game.hpp"

ALLEGRO_BITMAP *Ball::sprite_ball = NULL;
ALLEGRO_BITMAP *Ball::sprite_splash = NULL;

Ball::Ball(glm::vec2 position, Game &g, float direction) : BoxObject(glm::vec2(16.f, 16.f)), game_(g), distleft_(64.)
{
    setPosition(position);
    orientation = direction;
    setSpeed(2.);
    active = true;
    startTime = (long)(al_get_time() * 1000);

    if (!sprite_ball) {
		ALLEGRO_PATH *basedir, *file;
		basedir = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(basedir, RESOURCES_DIR);

		file = al_clone_path(basedir);
		al_set_path_filename(file, "missile.png");
		sprite_ball = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		file = al_clone_path(basedir);
		al_set_path_filename(file, "pof.png");
		sprite_splash = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		al_destroy_path(basedir);
	}
}

bool Ball::isActive() {
	return active;
}

void Ball::deactivate() {
	active = false;
    startTime = (long)(al_get_time() * 1000);
}

void Ball::update()
{
    auto ptr = shared_from_this();
    if(distleft_ <= 0. || !active && ((long)(al_get_time() * 1000) > startTime+1500))
    {
        game_.removeObject(ptr);
        return;
    }

    if (active) {
		step();
		distleft_-= speed;
	}
}

void Ball::draw(glm::vec2 screen_ul_corner)
{
	if (active) {
		float sx = ((((long)(al_get_time() * 1000) - startTime) / 100) % 4) * 32;
		al_draw_tinted_scaled_rotated_bitmap_region(sprite_ball,
			sx, 0, 32, 32,
			al_map_rgb_f(1., 1., 1.),
			8, 8,
			this->position.x - screen_ul_corner.x, this->position.y - screen_ul_corner.y,
			1., 1.,
			PI+orientation, 0);
	}
	else {
		al_draw_rotated_bitmap(sprite_splash,
							   8,
							   8,
							   this->position.x - screen_ul_corner.x,
							   this->position.y - screen_ul_corner.y,
							   orientation,
							   0);
	}
#ifdef _DEBUG
    drawHull(screen_ul_corner);
#endif //_DEBUG
}
