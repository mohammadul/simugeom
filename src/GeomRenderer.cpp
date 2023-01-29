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
 * @file GeomRenderer.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomRenderer.
 */

#include "../include/GeomRenderer.h"
#include "../include/GeomScene.h"
#include <array>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <windows.h>


namespace simugeom
{
#define SCREEN_WIDTH   320.
#define SCREEN_HEIGHT  240.

std::array<Colour::components,10> pallete =
{
    Colour::Blue, Colour::Red, Colour::Green,
    Colour::Magenta, Colour::Cyan, Colour::Yellow,
    Colour::Maroon, Colour::LightBlue, Colour::Turquoise,
    Colour::BlueViolet
};

static void set_icon(SDL_Window* w)
{
#if defined(_WIN32) || defined(__WIN32__) ||defined(WIN32) || defined(WINNT)
    HICON icon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
    if(icon)
    {
        ICONINFO ici;
        if(GetIconInfo(icon, &ici))
        {
            HDC dc = CreateCompatibleDC(NULL);
            if(dc)
            {
                SDL_Surface* ic = SDL_CreateRGBSurface(0, 32, 32, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                if(ic)
                {
                    BITMAPINFO bmi;
                    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                    bmi.bmiHeader.biWidth = 32;
                    bmi.bmiHeader.biHeight = -32;
                    bmi.bmiHeader.biPlanes = 1;
                    bmi.bmiHeader.biBitCount = 32;
                    bmi.bmiHeader.biCompression = BI_RGB;
                    bmi.bmiHeader.biSizeImage = 0;
                    SelectObject(dc, ici.hbmColor);
                    GetDIBits(dc, ici.hbmColor, 0, 32, ic->pixels, &bmi, DIB_RGB_COLORS);
                    SDL_SetWindowIcon(w, ic);
                    SDL_FreeSurface(ic);
                }
                DeleteDC(dc);
            }
            DeleteObject(ici.hbmColor);
            DeleteObject(ici.hbmMask);
        }
        DestroyIcon(icon);
    }
#endif
}

GeomRenderer::GeomRenderer(const GeomScene& _gs) : renderer(nullptr), window(nullptr), gs(_gs), m(nullptr), render_ready(false),
    thickness(2), rc(0), gc(0), bc(0), curriter(0), maxiters(0)
{
    scale = 0.35*std::sqrt((SCREEN_WIDTH*SCREEN_WIDTH+SCREEN_HEIGHT*SCREEN_HEIGHT)/(gs.bbox.rad(0,0)*gs.bbox.rad(0,0)+gs.bbox.rad(0,0)*gs.bbox.rad(0,0)));
}
GeomRenderer::~GeomRenderer()
{
    if(render_ready)
    {
        render_ready = false;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        window = nullptr;
        renderer = nullptr;
        SDL_Quit();
    }
    if(m!=nullptr) mesh_free_mesh(m);
}

int GeomRenderer::initSDL()
{
    render_ready = false;
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;
    SDL_Init(SDL_VIDEO_OPENGL);
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        return -1;
    }

    window = SDL_CreateWindow("GeomRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if(!window)
    {
        SDL_Quit();
        return -2;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    set_icon(window);
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    if(!renderer)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
        SDL_Quit();
        return -3;
    }
    render_ready = true;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    return 0;
}

void GeomRenderer::refresh()
{
    if(render_ready)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
    }
}

void GeomRenderer::draw_cuboid(double cx, double cy, double radx, double rady, double angle)
{
    cx = cx*scale;
    cy = cy*scale;
    radx = radx*scale;
    rady = rady*scale;
    double cs = std::cos(angle), sn = std::sin(angle);
    int x0 = (SCREEN_WIDTH/2)+cx+radx*cs-rady*sn;
    int y0 = (SCREEN_HEIGHT/2)-(cy-radx*sn-rady*cs);
    int x1 = (SCREEN_WIDTH/2)+cx+radx*cs+rady*sn;
    int y1 = (SCREEN_HEIGHT/2)-(cy-radx*sn+rady*cs);
    int x2 = (SCREEN_WIDTH/2)+cx-radx*cs+rady*sn;
    int y2 = (SCREEN_HEIGHT/2)-(cy+radx*sn+rady*cs);
    int x3 = (SCREEN_WIDTH/2)+cx-radx*cs-rady*sn;
    int y3 = (SCREEN_HEIGHT/2)-(cy+radx*sn-rady*cs);

//    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
//    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
//    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
//    SDL_RenderDrawLine(renderer, x3, y3, x0, y0);

    thickLineRGBA(renderer, x0, y0, x1, y1, thickness, rc, gc, bc, 255);
    thickLineRGBA(renderer, x1, y1, x2, y2, thickness, rc, gc, bc, 255);
    thickLineRGBA(renderer, x2, y2, x3, y3, thickness, rc, gc, bc, 255);
    thickLineRGBA(renderer, x3, y3, x0, y0, thickness, rc, gc, bc, 255);
}

void GeomRenderer::draw_ellipsoid(double cx, double cy, double radx, double rady, double angle)
{
    cx = cx*scale;
    cy = cy*scale;
    radx = radx*scale;
    rady = rady*scale;
    double cs = std::cos(angle), sn = std::sin(angle);
    int x0, x1, y0, y1;
    x0 = (SCREEN_WIDTH/2)+cx+std::cos(0)*radx*cs+std::sin(0)*rady*sn;
    y0 = (SCREEN_HEIGHT/2)-(cy-std::cos(0)*radx*sn+std::sin(0)*rady*cs);

    for(int32_t i=0; i<12; ++i)
    {
        double th = ((double)(i+1)*MESH_TWOPI)/(12.0);
        x1 = (SCREEN_WIDTH/2)+cx+std::cos(th)*radx*cs+std::sin(th)*rady*sn;
        y1 = (SCREEN_HEIGHT/2)-(cy-std::cos(th)*radx*sn+std::sin(th)*rady*cs);
        // SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
        thickLineRGBA(renderer, x0, y0, x1, y1, thickness, rc, gc, bc, 255);
        x0 = x1;
        y0 = y1;
    }
}

void GeomRenderer::draw_axes()
{
    SDL_SetRenderDrawColor(renderer, 0xAA, 0x0, 0x0, 0xFF);
    SDL_RenderDrawLine(renderer, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), (SCREEN_WIDTH/2)+scale, (SCREEN_HEIGHT/2));
    SDL_SetRenderDrawColor(renderer, 0x0, 0xAA, 0x0, 0xFF);
    SDL_RenderDrawLine(renderer, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)-scale);
}

void GeomRenderer::set_iteration(const int32_t _curriter, const int32_t _maxiters)
{
    curriter = _curriter;
    maxiters = _maxiters;
}

void GeomRenderer::draw_text(double cx, double cy, const char* s)
{
    cx = cx*scale;
    cy = cy*scale;
    const int x = (SCREEN_WIDTH/2)+cx-3;
    const int y = (SCREEN_HEIGHT/2)-cy-3;
    stringRGBA(renderer, x, y, s, rc, gc, bc, 255);
}

void GeomRenderer::render(bool clear)
{
    if(render_ready)
    {
        if(clear)
        {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);
        }
        draw_axes();
        //SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        const auto& gsmodels = gs.get_models();
        int32_t num_models = gsmodels.size();
        for(int32_t i=0; i<num_models; ++i)
        {
            const GeomModel& tmodel = gsmodels[i];
            const auto& rad = tmodel.get_radius();
            int colnum = tmodel.get_class_id()%pallete.size();
            Colour ccol{pallete[colnum]};
            // SDL_SetRenderDrawColor(renderer, ccol.rgb[0]*255, ccol.rgb[1]*255, ccol.rgb[2]*255, 0xFF);
            set_colour(ccol.rgb[0]*255, ccol.rgb[1]*255, ccol.rgb[2]*255);
            mesh_vector3 pos0;
            pos0 = {tmodel.get_pose().pos(0,0), tmodel.get_pose().pos(1,0), tmodel.get_pose().pos(2,0)};
            switch(gs.get_class(tmodel.get_class_id()).type)
            {
            case ObjClass::GeomType::Cuboid:
                draw_cuboid(pos0.x, pos0.y, rad(0,0), rad(1,0), tmodel.get_pose().rot);
                break;
            case ObjClass::GeomType::Ellipsoid:
                draw_ellipsoid(pos0.x, pos0.y, rad(0,0), rad(1,0), tmodel.get_pose().rot);
                break;
            }
            char str[128];
            sprintf(str, "%d", i);
            draw_text(pos0.x, pos0.y, str);
        }
        if(maxiters>0)
        {
            char str[128];
            sprintf(str, "%6d/%6d", curriter, maxiters);
            draw_text(1.2*(gs.bbox.pos(0,0)-gs.bbox.rad(0,0)), 1.2*(gs.bbox.pos(1,0)+gs.bbox.rad(1,0)), str);
        }
        SDL_RenderPresent(renderer);
        refresh();
    }
}

void GeomRenderer::generate_mesh()
{
    if(m!=nullptr) mesh_free_mesh(m);
    m = mesh_create_mesh_new();
    const auto& gsmodels = gs.get_models();
    int32_t num_models = gsmodels.size();
    mesh_vector3 sz0, sz1, pos0, pos1;
    mesh_vector3 nopos = {0.0,0.0,0.0};
    mesh_rotation rot;
    for(int32_t i=0; i<num_models; ++i)
    {
        const GeomModel& tmodel = gsmodels[i];
        const auto& rad = tmodel.get_radius();
        pos0 = {tmodel.get_pose().pos(0,0), tmodel.get_pose().pos(1,0), tmodel.get_pose().pos(2,0)};
        pos1 = {rad(0,0), 0.0, tmodel.get_pose().pos(2,0)};
        MESH m0 = nullptr;
        switch(gs.get_class(tmodel.get_class_id()).type)
        {
        case ObjClass::GeomType::Cuboid:
            sz0 = {rad(0,0)*2.0, rad(1,0)*2.0, 1e-8};
            sz1 = {sz0.x*0.2, sz0.y*0.2, 1e-8};
            m0 = mesh_create_mesh_new_rectangle_flat(&sz0, &nopos);
            break;
        case ObjClass::GeomType::Ellipsoid:
            sz0 = {rad(0,0), rad(1,0), 1e-8};
            sz1 = {sz0.x*0.4, sz0.y*0.4, 1e-8};
            m0 = mesh_create_mesh_new_ellipse_flat(&sz0, &nopos);
            break;
        }
        MESH m1 = mesh_create_mesh_new_rectangle_flat(&sz1, &pos1);

        rot.data[0] = std::cos(tmodel.get_pose().rot);
        rot.data[1] = std::sin(tmodel.get_pose().rot);
        rot.data[2] = 0.0;
        rot.data[3] = -std::sin(tmodel.get_pose().rot);
        rot.data[4] = std::cos(tmodel.get_pose().rot);
        rot.data[5] = 0.0;
        rot.data[6] = 0.0;
        rot.data[7] = 0.0;
        rot.data[8] = 1.0;

        m1 = mesh_combine_mesh(m1, m0);

        // add colour
        mesh_alloc_mesh_props(m1, m1->num_vertices, m1->num_faces, 0, MESH_PROPS_VCOLORS);
        int colnum = tmodel.get_class_id()%pallete.size();
        Colour ccol{pallete[colnum]};

        for(INTDATA i=0; i<m1->num_vertices; ++i)
        {
            m1->vcolors[i].r = static_cast<int>(ccol.rgb[0]*255);
            m1->vcolors[i].g = static_cast<int>(ccol.rgb[1]*255);
            m1->vcolors[i].b = static_cast<int>(ccol.rgb[2]*255);
            m1->vcolors[i].a = 255;
        }

        mesh_rotate(m1, &rot); // first rotate
        mesh_translate_vector(m1, &pos0);// then move whole

        m = mesh_combine_mesh(m, m1);

        mesh_free_mesh(m0);
        mesh_free_mesh(m1);
    }

}

int GeomRenderer::export_mesh(const char* fname)
{
    if(m==nullptr) return -1;
    return mesh_save_file(m, fname);
}

}

