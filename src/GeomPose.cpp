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
 * @file GeomPose.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomPose.
 */

#include "../include/GeomPose.h"

namespace simugeom
{

GeomPose::GeomPose()
{
}

GeomPose::~GeomPose()
{
}

std::ostream& operator <<(std::ostream& out, const GeomPose& p)
{
    out<<"GeomPose[P:(";
    out<<p.pos(0,0)<<", "<<p.pos(1,0)<<", "<<p.pos(2,0);
    out<<"), rot:"<<p.rot<<"]";
    return out;
}

}
