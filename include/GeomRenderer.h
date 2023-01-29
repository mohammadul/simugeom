/*
 *    simugeom - program package for geometry simulation 
 *    Copyright (C) 2019, 2023 Sk. Mohammadul Haque
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */	

/**
 * @file GeomRenderer.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of GeomRenderer.
 */

#ifndef GEOMRENDERER_H
#define GEOMRENDERER_H

#include <initializer_list>
#include <array>
#include <meshlib.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace simugeom
{

class Colour
{
public:
    using components = std::array<double,3>;
    Colour(const components& c) : rgb(c) {}
    components rgb;

    static constexpr components
    Black{0.0,0.0,0.0}, White{1.0,1.0,1.0}, Red{1.0,0.0,0.0},
          Green{0.0,1.0,0.0}, Blue{0.0,0.0,1.0}, Yellow{1.0,1.0,0.0},
          Magenta{1.0,0.0,1.0}, Cyan{0.0,1.0,1.0}, Orange{1.0,0.65,0.0},
          Gray{0.5,0.5,0.5}, BlueViolet{0.54,0.17,0.89}, Brown{0.65,0.16,0.16},
          DarkRed{0.55,0.0,0.0}, DarkGreen{0.0,0.39,0.0}, DarkBlue{0.0,0.0,0.55},
          DarkGray{0.66,0.66,0.66}, Maroon{0.69,0.19,0.38}, Pink{1.0,0.75,0.80},
          Purple{0.63,0.13,0.94}, FireBrick{0.7,0.13,0.13}, LightBlue{0.68,0.85,0.9},
          LightGray{0.83,0.83,0.83}, LightGreen{0.56,0.93,0.56}, RoyalBlue{0.25,0.41,0.88},
          Turquoise{0.25,0.88,0.82};
};

extern std::array<Colour::components,10> pallete;

class GeomScene;

class GeomRenderer
{
public:
    GeomRenderer(const GeomScene& _gs);
    virtual ~GeomRenderer();

    void render(bool clear=true);
    void generate_mesh();

    void display();
    int save(const char* fname);
    int export_mesh(const char* fname);

private:
    int initSDL();
    void draw_cuboid(double cx, double cy, double radx, double rady, double angle);
    void draw_ellipsoid(double cx, double cy, double radx, double rady, double angle);
    void draw_axes();
    void set_iteration(const int32_t curriter, const int32_t maxiters);
    void draw_text(double cx, double cy, const char* s);
    void set_colour(const uint8_t _r, const uint8_t _g, const uint8_t _b)
    { rc = _r; gc = _g; bc = _b; }
    void set_thickness(const uint8_t _t) { thickness = _t; }
    void refresh();

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    const GeomScene& gs;
    MESH m;
    bool render_ready;
    double scale;
    uint8_t thickness, rc, gc, bc;
    int32_t curriter;
    int32_t maxiters;
    friend class GeomAnnealer;
};




}

#endif // GEOMRENDERER_H
