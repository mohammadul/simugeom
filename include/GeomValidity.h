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
 * @file GeomValidity.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of GeomValidity.
 */

#ifndef GEOMVALIDITY_H
#define GEOMVALIDITY_H

#include <Eigen/Dense>
#include <vector>

namespace simugeom
{

struct AABB
{
    AABB() {}

    AABB(const Eigen::Vector3d& _pos, const Eigen::Vector3d& _rad)
    : pos(_pos), rad(_rad) {}

    Eigen::Vector3d pos, rad;
};

class Polygon2D
{
public:
    Polygon2D();
    Polygon2D(const std::vector<Eigen::Vector2d>& _pts);
    Polygon2D(std::vector<Eigen::Vector2d>&& _pts);
    Polygon2D(const AABB& bbox);

    virtual ~Polygon2D();

    void add(double _x, double _y);
    void add(const Eigen::Vector2d& _pt);

    bool is_inside(double _x, double _y) const;
    bool is_inside(const Eigen::Vector2d& _pt) const;
    bool is_inside(const Eigen::Vector3d& _pt) const;

    const std::vector<Eigen::Vector2d>& get_points() const { return pts; }
    std::vector<Eigen::Vector2d>& get_points() { return pts; }

    private:

    std::vector<Eigen::Vector2d> pts;

};


class GeomValidity
{
    public:
        GeomValidity();
        virtual ~GeomValidity();

    private:
};


}

#endif // GEOMVALIDITY_H
