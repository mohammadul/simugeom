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
 * @file ObjClass.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of ObjClass.
 */

#ifndef OBJCLASS_H
#define OBJCLASS_H

#include <memory>
#include <string>

namespace simugeom
{

class ObjClassSet;

class ObjClass
{
public:
    enum class GeomType { Ellipsoid = 0, Cuboid = 1 };

    ObjClass(const std::string _name, const GeomType _type, bool _is_fixed = false, const int32_t _cid = -1);
    ObjClass() = delete;
    virtual ~ObjClass();
    double get_prior_prob() const
    {
        return prior_prob;
    }

public:
    std::string name;
    GeomType type;
    bool is_fixed;
    int32_t cid;
    double prior_prob;

private:
    friend std::ostream& operator <<(std::ostream& out, const ObjClass& s);

};

std::ostream& operator <<(std::ostream& out, const ObjClass& s);

}

#endif // OBJCLASS_H
