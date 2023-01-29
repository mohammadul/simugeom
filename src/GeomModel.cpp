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
 * @file GeomModel.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomModel.
 */

#include "../include/GeomModel.h"
#include "../include/GeomScene.h"

#include <meshlib.h>
#include <cmath>

namespace simugeom
{

GeomModel::GeomModel(const std::string _name, const int _oid, const double _rad, const ObjClass& _cls):
    name(_name), oid(_oid), type(_cls.type), cid(_cls.cid)
{
    set_radius(_rad);
    pose.pos(0,0) = 0.0;
    pose.pos(1,0) = 0.0;
    pose.pos(2,0) = 0.0;
    pose.rot = 0.0;
}

GeomModel::GeomModel(const std::string _name, const int _oid, const Eigen::Vector3d& _rad, const ObjClass& _cls):
    name(_name), oid(_oid), type(_cls.type), radius(_rad), cid(_cls.cid)
{
    pose.pos(0,0) = 0.0;
    pose.pos(1,0) = 0.0;
    pose.pos(2,0) = 0.0;
    pose.rot = 0.0;
}

GeomModel::~GeomModel()
{
}

double GeomModel::get_bb_radius_from(const double x, const double y) const
{
    double bbrad = std::max(radius(0,0), radius(1,0));
    double reltheta = std::atan2((y-pose.pos(1,0)), (x-pose.pos(0,0)))-pose.rot;
    if(reltheta>=MESH_PI) reltheta -= MESH_TWOPI;
    else if(reltheta<=-MESH_PI) reltheta += MESH_TWOPI;

    switch(type)
    {
    case ObjClass::GeomType::Cuboid:
    {
        const double phi = std::atan2(radius(1,0), radius(0,0));
        // phi = [-180 to 180 deg] in radian
        // based on side of rectangle
        if(std::abs(reltheta)<=std::abs(phi))
        {
            // right side
            bbrad = radius(0,0)/std::cos(reltheta);
        }
        else if(std::abs(reltheta)>=(MESH_PI-std::abs(phi)))
        {
            // left side
            bbrad = -radius(0,0)/std::cos(reltheta);
        }
        else if(reltheta>=phi && reltheta<=(MESH_PI-phi))
        {
            // top side
            bbrad = radius(1,0)/std::sin(reltheta);
        }
        else
        {
            // bottom side
            bbrad = -radius(1,0)/std::sin(reltheta);
        }
    }
    break;

    case ObjClass::GeomType::Ellipsoid:
    {
        const auto tmp1 = radius(0,0)*std::cos(-reltheta);
        const auto tmp2 = radius(1,0)*std::sin(-reltheta);
        bbrad = std::sqrt(tmp1*tmp1+tmp2*tmp2);
    }
    break;
    }
    return bbrad;
}

double GeomModel::get_bb_radius_from(const Eigen::Vector2d xy) const
{
    return get_bb_radius_from(xy(0,0), xy(1,0));
}

/*
double GeomModel::get_bb_radius_from(const Eigen::Vector3d xyz) const
{
    return get_bb_radius_from(xyz(0,0), xyz(1,0));
} */

void GeomModel::propose_perturb(const int n, const double sigmpos,
                                const double sigmrot, GeomScene& gsn)
{
    proposed_poses.resize(n);
    for(int i=0; i<n; ++i)
    {
        GeomPose& cpose = proposed_poses[i];
        while(true)
        {
            Eigen::Vector3d delpos;
            delpos(0,0) = sigmpos*gsn.unidist(gsn.rng);
            delpos(1,0) = sigmpos*gsn.unidist(gsn.rng);
            delpos(2,0) = 0.0;
            cpose.pos = pose.pos+delpos;
            double delrot = sigmrot*gsn.unidist(gsn.rng);
            cpose.rot = pose.rot+delrot;
            if(gsn.boundary.is_inside(cpose.pos(0,0), cpose.pos(1,0)))
                break;
        }
    }
}

std::ostream& operator <<(std::ostream& out, const GeomModel& m)
{
    out<<"GeomModel[";
    out<<m.name<<", ";
    out<<m.oid<<", ";
    out<<m.cid<<", Rad(";
    out<<m.radius(0,0)<<", ";
    out<<m.radius(1,0)<<", ";
    out<<m.radius(2,0)<<"), ";
    out<<m.pose<<"]";
    return out;
}

}
