#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "ball.hpp"
#include "villager.hpp"
#include "../main.hpp"
#include "../game.hpp"
#include "../noise/perlin.hpp"

ALLEGRO_BITMAP* Flame::flames_spawn = NULL;
ALLEGRO_BITMAP* Flame::flames_loop = NULL;

Flame::Flame(glm::vec2 position, Game &g, float orientation):
	BoxObject(glm::vec2(48.f, 115.f), glm::vec2(0., 48.)), game(g), startTime((long)al_get_time()*1000)
{
	this->position = position;
	this->orientation = orientation;

	this->speed = 16;
	step();

	if (!flames_spawn) {
		ALLEGRO_PATH *basedir, *file;
		basedir = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(basedir, RESOURCES_DIR);

		file = al_clone_path(basedir);
		al_set_path_filename(file, "flames-spawn.png");
		flames_spawn = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		file = al_clone_path(basedir);
		al_set_path_filename(file, "flames-loop.png");
		flames_loop = al_load_bitmap(al_path_cstr(file, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(file);

		al_destroy_path(basedir);
	}
}

void Flame::draw(glm::vec2 screen_ul_corner) {
	float delta = ((long)(al_get_time() * 1000)) - this->startTime;
	ALLEGRO_BITMAP *source;
	int sx;

	if (delta < 900.) {
		sx = ((int)(delta / 150) % 6) * 64;
		source = Flame::flames_spawn;
	}
	else {
		delta -= 900.;
		sx = ((int)(delta / 150) % 4) * 64;
		source = Flame::flames_loop;
	}

	al_draw_tinted_scaled_rotated_bitmap_region(source,
		sx, 0, 64, 128,
		al_map_rgb_f(1., 1., 1.),
		32, 128,
		this->position.x - screen_ul_corner.x, this->position.y - screen_ul_corner.y,
		1., 1.,
		orientation+PI, 0);
#ifdef _DEBUG
	drawHull(screen_ul_corner);
#endif //_DEBUG
}

void Flame::update() {
	auto ptr = shared_from_this();

	if ((long)(al_get_time() * 1000) > this->startTime + 4000) {
		glm::vec2 dir = getUnitDirectionVector();

		glm::vec2 pos = position + dir * 20.f;
		Tile *t = game.map_.pwhat(pos.x, pos.y);
		if (!t->ignited()) t->ignite(1000);

		pos = position + dir * 40.f;
		t = game.map_.pwhat(pos.x, pos.y);
		if (!t->ignited()) t->ignite(1000);

		this->game.removeObject(ptr);
	}
}

// ----

ALLEGRO_BITMAP* Villager::sprite = NULL;
TriObject Villager::fov(6*32., 5*32., PI);

Villager::Villager(glm::vec2 spawnPosition, Game &game): Character(glm::vec2(30, 30), game) {
	this->position = spawnPosition;
	this->status = NONE;
	this->statusDate = 0L;
	this->hp = 100;
	this->flame = NULL;

	if (this->sprite == NULL) {
		ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(path, RESOURCES_DIR);
		al_set_path_filename(path, "villager.png");
		sprite = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(path);
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
	auto ptr = shared_from_this();

	// Check for projectiles
	for(int i=0; i < game.objects_.size(); ++i) {
		if (game.objects_[i]) {
			if (std::dynamic_pointer_cast<Ball>(game.objects_[i])) {
				auto ball = std::dynamic_pointer_cast<Ball>(game.objects_[i]);
				if (ball->isActive()) {
					if (collide(*(game.objects_[i]))) {
						ball->deactivate();
						hp -= 30;
						this->game.removeObject(this->flame);
						this->flame = NULL;
						if (hp <= 0) {
							game.removeObject(ptr);
							return;
						}
						this->orientation = PI + ball->getOrientation();
						break;
					}
				}
			}
		}
	}

	// Set various parameters according to environment
	if (game.map_.what(this->position.x, this->position.y) == SWAMP) {
		this->speed = .8;
	} else {
		this->speed = 1.5;
	}

	Villager::fov.setPosition(this->position);
	Villager::fov.setOrientation(this->orientation);

	// IA code
	if (status == NONE || status == ROAMING) {
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
		else {
			setStatus(ROAMING);
			this->speed = 1; // is not in a hurry

			// if we are in front of a swamp tile, sets it on FIRE!!
			glm::vec2 dir = getUnitDirectionVector();
			dir *= 48.f;
			glm::vec2 frontTilePos = this->position + dir;
			Tile *t = this->game.map_.pwhat(frontTilePos.x, frontTilePos.y);
			if (t->getBiome() == SWAMP && !(t->ignited())) {
				setStatus(DRYING);
				auto ptr = std::make_shared<Flame>(this->position, game, this->orientation);
				this->flame = ptr;
				game.addObject(ptr);
			}
			else {
				this->orientation += noise1((float)(al_get_time()/50.));
				step();
			}
		}
	}

	else if (status == CHASING) {
		if (Villager::fov.collide(*(this->game.swampman_))) {
			glm::vec2 dv = this->game.swampman_->getPosition() - this->position;
			float dist = dv.x * dv.x + dv.y * dv.y;
			if (dist < 64*64) {
				setStatus(FIRING);
				auto ptr = std::make_shared<Flame>(this->position, game, this->orientation);
				this->flame = ptr;
				game.addObject(ptr);
			}
			else {
				chaseSwampman();
			}
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

	else if (status == FIRING || status == DRYING) {
		if ((long)(al_get_time() * 1000) > this->statusDate+4000) {
			setStatus(NONE);
			this->flame = NULL;
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

