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
 * @file GeomModel.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of GeomModel.
 */

#ifndef GEOMMODEL_H
#define GEOMMODEL_H

#include <vector>
#include <memory>
#include <string>
#include <random>
#include "ObjClass.h"
#include "GeomPose.h"


namespace simugeom
{

class GeomScene;

class GeomModel
{
public:
    GeomModel(const std::string _name, const int _oid, const double _rad, const ObjClass& _cls);
    GeomModel(const std::string _name, const int _oid, const Eigen::Vector3d& _rad, const ObjClass& _cls);
    virtual ~GeomModel();

    const std::string& get_name() const
    {
        return name;
    }

    int32_t get_class_id() const
    {
        return cid;
    }

    int32_t get_object_id() const
    {
        return oid;
    }

    const GeomPose& get_pose() const
    {
        return pose;
    }

    const Eigen::Vector3d& get_radius() const
    {
        return radius;
    }


    GeomPose& get_pose()
    {
        return pose;
    }

    void set_object_id(const int _oid)
    {
        oid = _oid;
    }

    void set_pose(const GeomPose& _pose)
    {
        pose = _pose;
    }

    void set_radius(const double _radius)
    {
        radius(0,0) = _radius;
        radius(1,0) = _radius;
        radius(2,0) = _radius;
    }

    void set_radius(const double _r0, const double _r1, const double _r2)
    {
        radius(0,0) = _r0;
        radius(1,0) = _r1;
        radius(2,0) = _r2;
    }

    void set_radius(const Eigen::Vector3d& _radius)
    {
        radius = _radius;
    }

    double get_bb_radius_from(const double x, const double y) const;
    double get_bb_radius_from(const Eigen::Vector2d xy) const;
    //double get_bb_radius_from(const Eigen::Vector3d xyz) const;

    void perturb(const Eigen::Vector3d delpos, const double delrot);

protected:
    void propose_perturb(const int n, const double sigmpos,
                  const double sigmrot, GeomScene& gsn);

private:
    std::string name;
    int oid;

    GeomPose pose;
    std::vector<GeomPose> proposed_poses;

    ObjClass::GeomType type;
    Eigen::Vector3d radius; // to use bbox later
    int cid;

    friend class GeomScene;
    friend class GeomAnnealer;
    friend std::ostream& operator <<(std::ostream& out, const GeomModel& m);
};

std::ostream& operator <<(std::ostream& out, const GeomModel& m);

}

#endif // GEOMMODEL_H



