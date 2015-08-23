#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>



#include "ball.hpp"

Ball::Ball(glm::vec2 position, float direction) : BoxObject(glm::vec2(32.f, 32.f)), distleft_(160.)
{
    setPosition(position);
    orientation = direction;
    setSpeed(8.);
}

void Ball::update()
{
    if(distleft_ <= 0.)
    {
       //TODO remove self
    }

    step();
    distleft_-= speed;
}
