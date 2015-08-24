#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

#include <memory>
#include <string>

#include "swampman.hpp"
#include "ball.hpp"
#include "../game.hpp"
#include "../misc/defines.hpp"

void Character::step() {
	Object::step();
	if (this->position.x < 16) this->position.x = 16;
	if (this->position.y < 16) this->position.y = 16;

	if (this->position.x > (this->game.map_.getWidth()  * 32 - 16)) this->position.x = (this->game.map_.getWidth()  * 32 - 16);
	if (this->position.y > (this->game.map_.getHeight() * 32 - 16)) this->position.y = (this->game.map_.getHeight() * 32 - 16);

	// Collision with rocks
	Tile t;
	for (int i=-1; i<2; i++) {
		for (int j=-1; j<2; j++) {
			t = game.map_.what(position.x + i*32.f, position.y + j*32.f);
			if (collide(t.asObject()) && t.getBiome() == ROCK) {
				// !!! works only with squares, does not work with rectangles !!!
				glm::vec2 tpos(t.getx()*32+16, t.gety()*32+16);
				glm::vec2 d = tpos - position;
				glm::vec2 d_abs = glm::abs(d);
				if (d_abs.x > d_abs.y) {
					this->position.x -= m_sign(d.x) * (16 + (this->dimensions.x/2.) - d_abs.x);
				}
				else {
					this->position.y -= m_sign(d.y) * (16 + (this->dimensions.y/2.) - d_abs.y);
				}
			}
		}
	}
}

float Swampman::orientations[9] = {PI4_3, -PI, -PI4_3, PI2, 0., -PI2, PI4, 0., -PI4 };

Swampman::Swampman(glm::vec2 pos, Game &g): Character(glm::vec2(28.f, 28.f), g) , up(false), down(false), left(false), right(false), throwing(false), hp_(100), ballammo_(3), throwcd_(0)
{
    setPosition(pos);

    ALLEGRO_PATH *path;

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, RESOURCES_DIR);
    al_set_path_filename(path, "swampman.png");
    sprite_ = al_load_bitmap(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));

    al_destroy_path(path);
}
Swampman::~Swampman()
{
    al_destroy_bitmap(sprite_);
}

void Swampman::update()
{
    Tile t;
    updateOrientation();

    if(throwcd_) throwcd_--;

    if(hp_ <= 0)
        game.stop();

    bool burned = false;
    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            t = game.map_.what(position.x + i*32.f, position.y + j*32.f);
            if (collide(t.asObject())) {
                if(t.getBiome() == SWAMP && t.ignited() && !burned) {
                    hp_ -= 1;
                    burned = true;
                }
            }
        }
    }

    t = game.map_.what(position.x, position.y);
    if(!((up == down) && (right == left))) //if there is movement
    {
        if(t.getBiome() == GRASS)
        {
            setSpeed(2.);
        }
        else if(t.getBiome() == SWAMP)
        {
            setSpeed(2.4);
        }
        step();
    }
    if(throwing && ballammo_ && !throwcd_)
    {
        ballammo_--;
        throwcd_ = 30;

        decltype(position) ballpos = position;
        ballpos += glm::vec2(-sin(this->orientation), cos(this->orientation)) * 32.f;
        auto ptr = std::make_shared<Ball>(ballpos, game, this->orientation);
        ptr->setSpeed(2. * 1.5);
        game.addObject(ptr);
    }
}

void Swampman::updateOrientation()
{
    if(up && !down && !right && left)
        orientation = orientations[0];
    if(up && (left == right))
        orientation = orientations[1];
    if(up && !down && right && !left)
        orientation = orientations[2];
    if(left && (up == down))
        orientation = orientations[3];
    if(right && (up == down))
        orientation = orientations[5];
    if(left && down && !up && !right)
        orientation = orientations[6];
    if(down && (left == right))
        orientation = orientations[7];
    if(right && !left && down && !up)
        orientation = orientations[8];
}

void Swampman::draw(glm::vec2 screen_ul_corner)
{
    al_draw_rotated_bitmap(sprite_,
                           15,
                           10,
                           this->position.x - screen_ul_corner.x,
                           this->position.y - screen_ul_corner.y,
                           orientation + PI,
                            0);

#ifdef _DEBUG
    drawHull(screen_ul_corner);
#endif // _DEBUG
}


void Swampman::processInput(ALLEGRO_EVENT ev)
{
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:    up       = true; break;
            case ALLEGRO_KEY_DOWN:  down     = true; break;
            case ALLEGRO_KEY_LEFT:  left     = true; break;
            case ALLEGRO_KEY_RIGHT: right    = true; break;
            case ALLEGRO_KEY_LCTRL: throwing = true; break;
            default: break;
        };
    }
    if(ev.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:    up       = false; break;
            case ALLEGRO_KEY_DOWN:  down     = false; break;
            case ALLEGRO_KEY_LEFT:  left     = false; break;
            case ALLEGRO_KEY_RIGHT: right    = false; break;
            case ALLEGRO_KEY_LCTRL: throwing = false; break;
            default: break;
        };

    }
}

void Swampman::drawHUD(int topx, int topy)
{
    //draw health bar rect
    al_draw_rectangle(topx + 20., topy + 20., topx + 80., topy + 30., al_map_rgb(186, 54, 54), 1.);

    al_draw_filled_rectangle(topx + 20. , topy + 20. , topx + 20. + (hp_ / 100. * 60), topy + 30., al_map_rgb(241, 31, 31));

    al_draw_textf(Main::main_font, al_map_rgb(241, 31, 31), topx + 110., topy + 22., 0, "%d",hp_);
}

