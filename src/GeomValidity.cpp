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
 * @file GeomValidity.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomValidity.
 */

#include "../include/GeomValidity.h"

namespace simugeom
{

Polygon2D::Polygon2D()
{
}
Polygon2D::Polygon2D(const std::vector<Eigen::Vector2d>& _pts) : pts(_pts) {}
Polygon2D::Polygon2D(std::vector<Eigen::Vector2d>&& _pts) : pts(_pts) {}

Polygon2D::Polygon2D(const AABB& bbox)
{
    const auto mnm = bbox.pos-bbox.rad;
    const auto mxm = bbox.pos+bbox.rad;
    add(mnm(0), mnm(1));
    add(mxm(0), mnm(1));
    add(mxm(0), mxm(1));
    add(mnm(0), mxm(1));
}

Polygon2D::~Polygon2D()
{
}

void Polygon2D::add(double _x, double _y)
{
    Eigen::Vector2d pt;
    pt(0,0) = _x;
    pt(1,0) = _y;
    pts.emplace_back(pt);
}

void Polygon2D::add(const Eigen::Vector2d& _pt)
{
    pts.emplace_back(_pt);
}

bool Polygon2D::is_inside(double _x, double _y) const
{
    const int  npts = pts.size();
    bool c = false;
    for(int i=0, j=npts-1; i<npts; ++i)
    {
        const auto& pti = pts[i];
        const auto& ptj = pts[j];
        if(((pti(1,0)>_y)!=(ptj(1,0)>_y))
                && (_x<(ptj(0,0)-pti(0,0))*(_y-pti(1,0))/(ptj(1,0)-pti(1,0))+pti(0,0)))
        {
            c = !c;
        }
        j = i;
    }
    return c;
}

bool Polygon2D::is_inside(const Eigen::Vector2d& _pt) const
{
    return is_inside(_pt(0,0), _pt(1,0));
}

bool Polygon2D::is_inside(const Eigen::Vector3d& _pt) const
{
    return is_inside(_pt(0,0), _pt(1,0));
}

GeomValidity::GeomValidity()
{
}

GeomValidity::~GeomValidity()
{
}


}
