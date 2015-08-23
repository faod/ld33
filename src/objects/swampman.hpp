#pragma once

#ifndef SWAMPMAN_HPP
#define SWAMPMAN_HPP

#include "object.hpp"

class Swampman : public BoxObject
{
    public: 
        Swampman(glm::vec2 position);
        ~Swampman();
        virtual void update();
        virtual void draw(glm::vec2 screen_ul_corner);

        void processInput(ALLEGRO_EVENT ev);

    private:
        ALLEGRO_BITMAP *sprite_;

        bool up;
        bool down;
        bool left;
        bool right;
};

#endif 
