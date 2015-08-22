#pragma once

#ifndef SWAMPMAN_HPP
#define SWAMPMAN_HPP

#include "object.hpp"

class Swampman : public Object
{
    public: 
        Swampman(glm::vec2 position);
        ~Swampman();
        virtual void refresh();
        virtual void draw();


    private:
        ALLEGRO_BITMAP *sprite_;
};

#endif 
