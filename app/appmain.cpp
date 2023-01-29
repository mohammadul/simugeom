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
 * @file appmain.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of appmain.
 */

#include <iostream>
#include <GeomScene.h>
#include <GeomModel.h>
#include <ObjClass.h>
#include <ObjClassSet.h>

namespace sm = simugeom;

int main()
{
    sm::ObjClassSet obj_types;
    obj_types.insert("wall", true);
    obj_types.insert("bed");
    obj_types.insert("table");

    std::cout<<obj_types<<std::endl;

    sm::GeomScene scn(obj_types);
    scn.insert("wall_0", 0, 1., scn.get_class(0));
    scn.insert("wall_1", 0, 1., scn.get_class("wall"));
    scn.insert("wall_2", 0, 1., scn.get_class(0));
    scn.insert("wall_3", 0, 1., scn.get_class(0));

    scn.insert("bed_0", 0, 1., scn.get_class("bed"));

    scn.insert("table_0", 0, 1., scn.get_class(2));

    std::cout<<scn<<std::endl;

    return 0;
}
