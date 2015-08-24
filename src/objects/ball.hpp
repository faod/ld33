#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include <memory>

#include "object.hpp"


class Game;

class Ball : public BoxObject, public std::enable_shared_from_this<Ball>
{
    public:
        Ball(glm::vec2 position, Game &g, float direction);

        virtual void draw(glm::vec2 screen_ul_corner);
        virtual void update();
        bool isActive();
        void deactivate();
    private:
        Game& game_;
        float distleft_;
        long startTime;
        bool active;
        static ALLEGRO_BITMAP *sprite_ball, *sprite_splash;
};

#endif
