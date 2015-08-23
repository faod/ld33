#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "object.hpp"

class Ball : public BoxObject
{
    public:
        Ball(glm::vec2 position, float direction);

    private:
        float distleft_;
};

#endif
