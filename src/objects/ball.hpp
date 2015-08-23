#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "object.hpp"

class Game;

class Ball : public BoxObject
{
    public:
        Ball(glm::vec2 position, Game &g, float direction);

        virtual void update();
    private:
        Game& game_;
        float distleft_;
};

#endif
