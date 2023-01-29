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
 * @file GeomSceneIO.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomSceneIO.
 */

#include "../include/GeomSceneIO.h"
#include <fstream>

namespace simugeom
{

GeomSceneReader::GeomSceneReader(GeomScene& _gs) : gs(_gs)
{
}

GeomSceneReader::~GeomSceneReader()
{
}

int GeomSceneReader::read(const char* fname)
{
    std::ifstream ifs(fname, std::ios::binary);
    if(!ifs.is_open()) return -1;

    // param_alpha
    double param_alpha;
    ifs.read(reinterpret_cast<char *>(&param_alpha), sizeof(double));

    // seed
    uint32_t seed;
    ifs.read(reinterpret_cast<char *>(&seed), sizeof(uint32_t));

    // classes
    int32_t num_classes;
    ifs.read(reinterpret_cast<char *>(&num_classes), sizeof(int32_t));

    for(int32_t i=0; i<num_classes; ++i)
    {
        char name[256];
        ObjClass::GeomType type;
        uint8_t is_fixed;
        int32_t cid;

        int32_t str_sz;
        ifs.read(reinterpret_cast<char *>(&str_sz), sizeof(int32_t));
        ifs.read(reinterpret_cast<char *>(name), sizeof(char)*str_sz);
        name[str_sz] = 0;

        ifs.read(reinterpret_cast<char *>(&type), sizeof(ObjClass::GeomType));

        ifs.read(reinterpret_cast<char *>(&is_fixed), sizeof(uint8_t));
        ifs.read(reinterpret_cast<char *>(&cid), sizeof(uint32_t));

        ObjClass tcls(name, type, is_fixed, cid);
        gs.classes.insert(tcls);
    }

    // now read the class parameters
    for(int32_t i=0; i<num_classes; ++i)
    {
        for(int32_t j=i; j<num_classes; ++j)
        {
            double val;
            ifs.read(reinterpret_cast<char *>(&val), sizeof(double));
            gs.classes.max_reco_dist[i][j] = val;
            gs.classes.max_reco_dist[j][i] = val;
            ifs.read(reinterpret_cast<char *>(&val), sizeof(double));
            gs.classes.reco_dist[i][j] = val;
            gs.classes.reco_dist[j][i] = val;
			int32_t valn;
			ifs.read(reinterpret_cast<char*>(&valn), sizeof(int32_t));
			std::vector<double> tmpval;
			for(int32_t k=0; k<valn; ++k)
			{
				ifs.read(reinterpret_cast<char*>(&val), sizeof(double));
				tmpval.push_back(val);
			}
			gs.classes.reco_angles[i][j] = tmpval;
            gs.classes.reco_angles[j][i] = tmpval;
        }
    }

    // models

    int32_t num_models;
    ifs.read(reinterpret_cast<char *>(&num_models), sizeof(int32_t));

    for(int32_t i=0; i<num_models; ++i)
    {
        char name[256];
        int32_t cid, oid;
        Eigen::Vector3d radius;

        int32_t str_sz;
        ifs.read(reinterpret_cast<char *>(&str_sz), sizeof(int32_t));
        ifs.read(reinterpret_cast<char *>(name), sizeof(char)*str_sz);
        name[str_sz] = 0;

        ifs.read(reinterpret_cast<char *>(&cid), sizeof(uint32_t));
        ifs.read(reinterpret_cast<char *>(&oid), sizeof(uint32_t));
        ifs.read(reinterpret_cast<char *>(&radius(0,0)), sizeof(double));
        ifs.read(reinterpret_cast<char *>(&radius(1,0)), sizeof(double));
        ifs.read(reinterpret_cast<char *>(&radius(2,0)), sizeof(double));

        // pose
        double x, y, z, rot;
        ifs.read(reinterpret_cast<char *>(&x), sizeof(double));
        ifs.read(reinterpret_cast<char *>(&y), sizeof(double));
        ifs.read(reinterpret_cast<char *>(&z), sizeof(double));
        ifs.read(reinterpret_cast<char *>(&rot), sizeof(double));
        GeomModel tmodel(name, oid, radius, gs.get_class(cid));
        GeomPose& tp = tmodel.get_pose();
        tp.pos(0,0) = x;
        tp.pos(1,0) = y;
        tp.pos(2,0) = z;
        tp.rot = rot;
        gs.insert(tmodel);
    }
    return 0;
}

GeomSceneWriter::GeomSceneWriter(const GeomScene& _gs) : gs(_gs)
{
}

GeomSceneWriter::~GeomSceneWriter()
{

}

int GeomSceneWriter::write(const char* fname)
{
    std::ofstream ofs(fname, std::ios::binary);
    if(!ofs.is_open()) return -1;

    // param_alpha
    ofs.write(reinterpret_cast<const char *>(&gs.param_alpha), sizeof(double));

    // seed
    ofs.write(reinterpret_cast<const char *>(&gs.seed), sizeof(uint32_t));

    // classes
    const int32_t num_classes = gs.get_num_classes();
    ofs.write(reinterpret_cast<const char *>(&num_classes), sizeof(int32_t));

    for(int32_t i=0; i<num_classes; ++i)
    {
        const ObjClass tcls = gs.get_class(i);
        const ObjClass::GeomType type = tcls.type;
        const uint8_t is_fixed = tcls.is_fixed;

        const int32_t str_sz = tcls.name.size();
        ofs.write(reinterpret_cast<const char *>(&str_sz), sizeof(int32_t));
        ofs.write(reinterpret_cast<const char *>(tcls.name.c_str()), sizeof(char)*str_sz);

        ofs.write(reinterpret_cast<const char *>(&type), sizeof(ObjClass::GeomType));
        ofs.write(reinterpret_cast<const char *>(&is_fixed), sizeof(uint8_t));
        ofs.write(reinterpret_cast<const char *>(&tcls.cid), sizeof(uint32_t));

    }

    // now write the class parameters
    for(int32_t i=0; i<num_classes; ++i)
    {
        for(int32_t j=i; j<num_classes; ++j)
        {
            double val = gs.classes.max_reco_dist[i][j];
            ofs.write(reinterpret_cast<char *>(&val), sizeof(double));
            val = gs.classes.reco_dist[i][j];
            ofs.write(reinterpret_cast<char *>(&val), sizeof(double));
            auto& tmpval = gs.classes.reco_angles[i][j];
			int32_t valn = tmpval.size();
			ofs.write(reinterpret_cast<char*>(&valn), sizeof(int32_t));
			for(int32_t k=0; k<valn; ++k)
			{
				val = tmpval[k];
				ofs.write(reinterpret_cast<char*>(&val), sizeof(double));
			}
        }
    }

    // models

    const int32_t num_models = gs.get_models().size();
    ofs.write(reinterpret_cast<const char *>(&num_models), sizeof(int32_t));

    for(int32_t i=0; i<num_models; ++i)
    {
        const GeomModel tmodel = gs.get_model(i);
        const int32_t cid = tmodel.get_class_id();
        const int32_t oid = tmodel.get_object_id();

        const auto& radius = tmodel.get_radius();

        const int32_t str_sz = tmodel.get_name().size();
        ofs.write(reinterpret_cast<const char *>(&str_sz), sizeof(int32_t));
        ofs.write(reinterpret_cast<const char *>(tmodel.get_name().c_str()), sizeof(char)*str_sz);

        ofs.write(reinterpret_cast<const char *>(&cid), sizeof(uint32_t));
        ofs.write(reinterpret_cast<const char *>(&oid), sizeof(uint32_t));
        ofs.write(reinterpret_cast<const char *>(&radius(0,0)), sizeof(double));
        ofs.write(reinterpret_cast<const char *>(&radius(1,0)), sizeof(double));
        ofs.write(reinterpret_cast<const char *>(&radius(2,0)), sizeof(double));

        // pose
        const GeomPose& gp = tmodel.get_pose();
        const double x = gp.pos(0,0);
        const double y = gp.pos(1,0);
        const double z = gp.pos(2,0);
        const double rot = gp.rot;

        ofs.write(reinterpret_cast<const char *>(&x), sizeof(double));
        ofs.write(reinterpret_cast<const char *>(&y), sizeof(double));
        ofs.write(reinterpret_cast<const char *>(&z), sizeof(double));
        ofs.write(reinterpret_cast<const char *>(&rot), sizeof(double));
    }
    return 0;
}

}

