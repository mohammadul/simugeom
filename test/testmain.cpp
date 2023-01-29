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
 * @file testmain.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of testmain.
 */

#include <GeomScene.h>
#include <GeomModel.h>
#include <ObjClass.h>
#include <ObjClassSet.h>
#include <GeomSceneIO.h>
#include <GeomRenderer.h>
#include <GeomAnnealer.h>
#include <iostream>
#include <string>
#include <memory>

namespace sm = simugeom;

int main()
{
    // first setup walls (in metres)
    const double wall_thickness = 1.0;
    const double room_length = 6.0;
    const double room_breadth = 4.0;
    {
        std::unique_ptr<sm::ObjClassSet> obj_types = std::make_unique<sm::ObjClassSet>();
        obj_types->insert("wall", sm::ObjClass::GeomType::Cuboid, true);
        obj_types->insert("bed", sm::ObjClass::GeomType::Cuboid);
        obj_types->insert("table", sm::ObjClass::GeomType::Ellipsoid);
        obj_types->insert("chair", sm::ObjClass::GeomType::Cuboid);

        std::cout<<*obj_types<<std::endl;

        // add the interaction properties between the classes
        {
            auto& mxrds = obj_types->get_all_max_reco_dists();
            auto& rds = obj_types->get_all_reco_dists();
            auto& ras = obj_types->get_all_reco_angles();
            // -------
            // wall with bed
            mxrds[0][1] = 2.0;
            mxrds[1][0] = 2.0;
            rds[0][1] = 0.1;
            rds[1][0] = 0.1;
			ras[0][1] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
            ras[1][0] = ras[0][1];

            // wall with table
            mxrds[0][2] = 2.0;
            mxrds[2][0] = 2.0;
            rds[0][2] = 0.5;
            rds[2][0] = 0.5;
            ras[0][2] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
			ras[2][0] = ras[0][2];

            // wall with chair
            mxrds[0][3] = 3.0;
            mxrds[3][0] = 3.0;
            rds[0][3] = 0.5;
            rds[3][0] = 0.5;
            ras[0][3] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
            ras[3][0] = ras[0][3];

            // -------
            // bed with bed
            mxrds[1][1] = 8.0;
            rds[1][1] = 5.0;
            ras[1][1] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };;

            // bed with table
            mxrds[1][2] = 6.0;
            mxrds[2][1] = 6.0;
            rds[1][2] = 2.5;
            rds[2][1] = 2.5;
            ras[1][2] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
            ras[2][1] = ras[1][2];

            // bed with chair
            mxrds[1][3] = 3.0;
            mxrds[3][1] = 3.0;
            rds[1][3] = 2.5;
            rds[3][1] = 2.5;
            ras[1][3] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
            ras[3][1] = ras[1][3];

            // -------
            // table with table
            mxrds[2][2] = 5.0;
            rds[2][2] = 3.5;
            ras[2][2] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };

            // -------
            // table with chair
            mxrds[2][3] = 1.0;
            mxrds[3][2] = 1.0;
            rds[2][3] = 0.48;
            rds[3][2] = 0.48;
            ras[2][3] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
            ras[3][2] = ras[2][3];

            // -------
            // chair with chair
            mxrds[3][3] = 5.0;
            rds[3][3] = 2.25;
            ras[3][3] = { 0.0, 0.5*MESH_PI, MESH_PI, 1.5*MESH_PI };
        }


        std::unique_ptr<sm::GeomScene> scn = std::make_unique<sm::GeomScene>(*obj_types);

        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            pose.pos(0,0) = 0.5*(room_length+wall_thickness);
            pose.pos(1,0) = -0.5*wall_thickness;
            pose.pos(2,0) = 0.0;
            pose.rot = 0.5*MESH_PI;
            rad(0,0) = 0.5*(room_breadth+wall_thickness);
            rad(1,0) = 0.5*wall_thickness;
            rad(2,0) = 1.0;
            sm::GeomModel gm("wall_0", 0, rad, scn->get_class("wall"));
            gm.set_pose(pose);
            scn->insert(gm);
        }
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            pose.pos(0,0) = 0.5*wall_thickness;
            pose.pos(1,0) = 0.5*(room_breadth+wall_thickness);
            pose.pos(2,0) = 0.0;
            pose.rot = MESH_PI;
            rad(0,0) = 0.5*(room_length+wall_thickness);
            rad(1,0) = 0.5*wall_thickness;
            rad(2,0) = 1.0;
            sm::GeomModel gm("wall_1", 0, rad, scn->get_class("wall"));
            gm.set_pose(pose);
            scn->insert(gm);
        }
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            pose.pos(0,0) = -0.5*(room_length+wall_thickness);
            pose.pos(1,0) = 0.5*wall_thickness;
            pose.pos(2,0) = 0.0;
            pose.rot = 1.5*MESH_PI;
            rad(0, 0) = 0.5*(room_breadth+wall_thickness);
            rad(1, 0) = 0.5*wall_thickness;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("wall_2", 0, rad, scn->get_class("wall"));
            gm.set_pose(pose);
            scn->insert(gm);
        }
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            pose.pos(0, 0) = -0.5*wall_thickness;
            pose.pos(1, 0) = -0.5*(room_breadth+wall_thickness);
            pose.pos(2, 0) = 0.0;
            pose.rot = 0.0 * MESH_PI;
            rad(0, 0) = 0.5 * (room_length+wall_thickness);
            rad(1, 0) = 0.5 * wall_thickness;
            rad(2, 0) = 1.0;

            sm::GeomModel gm("wall_3", 0, rad, scn->get_class("wall"));
            gm.set_pose(pose);
            scn->insert(gm);
        }
        // now bed
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            rad(0, 0) = 1.0; // head-to-toe
            rad(1, 0) = 0.7;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("bed_0", 0, rad, scn->get_class("bed"));
            scn->insert(gm);
        }

		{
			sm::GeomPose pose;
			Eigen::Vector3d rad;
			rad(0, 0) = 1.0; // head-to-toe
			rad(1, 0) = 0.7;
			rad(2, 0) = 1.0;
			sm::GeomModel gm("bed_1", 0, rad, scn->get_class("bed"));
			//scn.insert(gm);
		}

		// now table
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            rad(0, 0) = 0.5; // circular radius
            rad(1, 0) = 0.5;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("table_0", 0, rad, scn->get_class("table"));
            scn->insert(gm);
        }
        // now chairs
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            rad(0, 0) = 0.25; // rectangular
            rad(1, 0) = 0.25;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("chair_0", 0, rad, scn->get_class("chair"));
            scn->insert(gm);
        }
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            rad(0, 0) = 0.25; // rectangular
            rad(1, 0) = 0.25;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("chair_1", 0, rad, scn->get_class("chair"));
            scn->insert(gm);
        }
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            rad(0, 0) = 0.25; // rectangular
            rad(1, 0) = 0.25;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("chair_2", 0, rad, scn->get_class("chair"));
            scn->insert(gm);
        }
        {
            sm::GeomPose pose;
            Eigen::Vector3d rad;
            rad(0, 0) = 0.25; // rectangular
            rad(1, 0) = 0.25;
            rad(2, 0) = 1.0;
            sm::GeomModel gm("chair_3", 0, rad, scn->get_class("chair"));
            scn->insert(gm);
        }

        {
            // set BBox and then scatter
            Eigen::Vector3d pos, rad;
            pos(0, 0) = 0.0;
            pos(1, 0) = 0.0;
            pos(2, 0) = 0.0;
            rad(0, 0) = 0.5*room_length;
            rad(1, 0) = 0.5*room_breadth;
            rad(2, 0) = 1.0;
            scn->set_boundary(sm::AABB(pos, rad));
            scn->scatter();
        }

        std::cout<<*scn<<std::endl;
        {
            sm::GeomSceneWriter gsw(*scn);
            gsw.write("gscn.gscn");
        }
    }

    std::cout<<"-----------------------" << std::endl;

    {
        sm::ObjClassSet obj_types;
        obj_types.insert("class_0", sm::ObjClass::GeomType::Cuboid, true);

        sm::GeomScene scn(obj_types);

        sm::GeomPose pose;
        Eigen::Vector3d rad;
        rad(0, 0) = 1.0;
        rad(1, 0) = 1.0;
        rad(2, 0) = 1.0;
        sm::GeomModel gm("obj_0", 0, rad, scn.get_class("class_0"));
        scn.insert(gm);
    }

    std::cout<<"-----------------------"<<std::endl;

    {
        sm::GeomScene scn;
        sm::GeomSceneReader gsr(scn);
        gsr.read("gscn.gscn");

        std::cout<<scn.get_classes()<<std::endl;
        std::cout<<scn<<std::endl;

        {
            sm::GeomRenderer grdr(scn);
            grdr.generate_mesh();
            grdr.export_mesh("gscn.ply");
        }
        {
            // again-set BBox and then scatter
            Eigen::Vector3d pos, rad;
            pos(0, 0) = 0.0;
            pos(1, 0) = 0.0;
            pos(2, 0) = 0.0;
            rad(0, 0) = 0.5*room_length;
            rad(1, 0) = 0.5*room_breadth;
            rad(2, 0) = 1.0;
            scn.set_boundary(sm::AABB(pos, rad));
        }
        {
            sm::GeomAnnealer gan(scn);
            sm::GeomRenderer grdr0(scn);
            gan.set_renderer(grdr0);
            gan.set_num_proposals(15);
            gan.initialise();
            gan.set_maxiters(1200);
            gan.solve();
            gan.export_cost_graph("costs.txt");

            std::cout<<scn<<std::endl;

            sm::GeomRenderer grdr(scn);
            grdr.generate_mesh();
            grdr.export_mesh("gscn.optimised.ply");

        }
    }


    return 0;
}
