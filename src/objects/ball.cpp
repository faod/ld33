#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>



#include "ball.hpp"
#include "../game.hpp"

Ball::Ball(glm::vec2 position, Game &g, float direction) : BoxObject(glm::vec2(32.f, 32.f)), game_(g), distleft_(160.)
{
    setPosition(position);
    orientation = direction;
    setSpeed(8.);
}

void Ball::update()
{
    auto ptr = shared_from_this();
    if(distleft_ <= 0.)
    {
        game_.removeObject(ptr);
    }

    step();
    distleft_-= speed;
}
