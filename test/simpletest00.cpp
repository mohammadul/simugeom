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
 * @file simpletest00.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of simpletest00.
 */

#include <GeomScene.h>
#include <GeomModel.h>
#include <ObjClass.h>
#include <ObjClassSet.h>
#include <iostream>
#include <string>
#include <meshlib.h>

namespace sm = simugeom;

int main()
{

    {
        sm::ObjClassSet obj_types;
        obj_types.insert("class0", sm::ObjClass::GeomType::Ellipsoid, true);
        obj_types.insert("class1", sm::ObjClass::GeomType::Ellipsoid);

        std::cout<<obj_types<<std::endl;

        // check distances for different angles

        for (double k=0; k<8; k=k+1)
        {
            sm::GeomScene scn(obj_types);
            {
                sm::GeomPose pose;
                Eigen::Vector3d rad;
                pose.pos(0,0) = 0.0;
                pose.pos(1,0) = 0.0;
                pose.pos(2,0) = 0.0;
                pose.rot = k*MESH_PI/4;
                rad(0,0) = 1.0;
                rad(1,0) = 1.0;
                rad(2,0) = 1.0;
                sm::GeomModel gm("obj0", 0, rad, scn.get_class("class0"));
                gm.set_pose(pose);
                scn.insert(gm);
            }
            {
                sm::GeomPose pose;
                Eigen::Vector3d rad;
                pose.pos(0,0) = 2.0;
                pose.pos(1,0) = 0.0;
                pose.pos(2,0) = 0.0;
                pose.rot = 0.0;
                rad(0,0) = 1.0;
                rad(1,0) = 1.0;
                rad(2,0) = 1.0;
                sm::GeomModel gm("obj1", 0, rad, scn.get_class("class0"));
                gm.set_pose(pose);
                scn.insert(gm);
            }

            std::cout<<"Angle: "<<scn.get_angle(0, 1)<<std::endl;
            std::cout<<"Dist: "<<scn.get_dist(0, 1)<<std::endl;
            std::cout<<"BB-Dist: "<<scn.get_bb_dist(0, 1)<<std::endl;
            std::cout<<"Fixed angle Cost: "<<scn.get_cost_fixed_angle(0, 1)<<std::endl;
        }

        std::cout<<"============================\n";
        // bb_dist cost

        for (double k=0; k<8; k=k+1)
        {
            sm::GeomScene scn(obj_types);
            {
                sm::GeomPose pose;
                Eigen::Vector3d rad;
                pose.pos(0,0) = 0.0;
                pose.pos(1,0) = 0.0;
                pose.pos(2,0) = 0.0;
                pose.rot = 0;//k*M_PI/4;
                rad(0,0) = 1.0;
                rad(1,0) = 1.0;
                rad(2,0) = 1.0;
                sm::GeomModel gm("obj0", 0, rad, scn.get_class("class0"));
                gm.set_pose(pose);
                scn.insert(gm);
            }
            {
                sm::GeomPose pose;
                Eigen::Vector3d rad;
                pose.pos(0,0) = k;
                pose.pos(1,0) = 0.0;
                pose.pos(2,0) = 0.0;
                pose.rot = 0.0;
                rad(0,0) = 1.0;
                rad(1,0) = 1.0;
                rad(2,0) = 1.0;
                sm::GeomModel gm("obj1", 0, rad, scn.get_class("class0"));
                gm.set_pose(pose);
                scn.insert(gm);
            }

            std::cout<<"BB Cost: "<<scn.get_cost_bb_intersect(0, 1)<<std::endl;

            std::cout<<"BB-Dist: "<<scn.get_bb_dist(0, 1)<<std::endl;
        }

        //Polygon2D inside check code
        {
            sm::Polygon2D p;
            p.add(0,0);
            p.add(1,0);
            p.add(1,1);
            p.add(0,1);


            for(double i=-0.5; i<1.5; i=i+0.1)
            {
                for(double j=-0.5; j<1.5; j=j+0.1)
                {

                    bool inside  = (i>=0 && i<=1 && j>=0 && j<=1);
                    if(p.is_inside(i,j) !=inside)
                    {
                        std::cout<<"err: ("<<i<<","<<j<<")\n";
                    }
                }
            }
        }

    }


    return 0;
}
