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
 * @file ObjClassSet.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of ObjClassSet.
 */

#include "../include/ObjClassSet.h"
#include "../include/ObjClass.h"
#include <iostream>

namespace simugeom
{

ObjClassSet::ObjClassSet()
{
}

ObjClassSet::~ObjClassSet()
{
}

int ObjClassSet::insert(ObjClass& objcls)
{
    if(searcher.find(objcls.name)!=searcher.end()) return -1;
    const int32_t old_num_classes = classes.size();
    objcls.cid = old_num_classes; /* force set oid */
    classes.push_back(objcls);
    searcher.emplace(objcls.name, old_num_classes);
    for(int32_t i=0; i<old_num_classes; ++i)
    {
        max_reco_dist[i].push_back(0.0);
        reco_dist[i].push_back(0.0);
        reco_angles[i].push_back(std::vector<double>());
    }
    max_reco_dist.push_back(std::vector<double>((old_num_classes+1), 0.0));
    reco_dist.push_back(std::vector<double>((old_num_classes+1), 0.0));
    reco_angles.push_back(std::vector< std::vector<double>>((old_num_classes+1), std::vector<double>()));
    return 0;
}

int ObjClassSet::insert(ObjClass&& objcls)
{
    if(searcher.find(objcls.name)!=searcher.end()) return -1;
    const int32_t old_num_classes = classes.size();
    objcls.cid = old_num_classes; /* force set oid */
    classes.push_back(objcls);
    searcher.emplace(objcls.name, old_num_classes);
    for(int32_t i=0; i<old_num_classes; ++i)
    {
        max_reco_dist[i].push_back(0.0);
        reco_dist[i].push_back(0.0);
		reco_angles[i].push_back(std::vector<double>());
	}
    max_reco_dist.push_back(std::vector<double>((old_num_classes+1), 0.0));
    reco_dist.push_back(std::vector<double>((old_num_classes+1), 0.0));
	reco_angles.push_back(std::vector< std::vector<double>>((old_num_classes + 1), std::vector<double>()));
	return 0;
}

double ObjClassSet::get_max_reco_dist(int32_t cid0, int32_t cid1) const
{
    return max_reco_dist[cid0][cid1];
}

double ObjClassSet::get_reco_dist(int32_t cid0, int32_t cid1) const
{
    return reco_dist[cid0][cid1];
}

const std::vector<double>& ObjClassSet::get_reco_angles(int32_t cid0, int32_t cid1) const
{
    return reco_angles[cid0][cid1];
}

std::ostream& operator <<(std::ostream& out, const ObjClassSet& s)
{
    out<<"ObjClassSet[";
    const int32_t n = s.get_num_classes();
    for(int32_t i=0; i<n; ++i)
    {
        out<<s.classes[i]<<", ";
    }
    out<<"]";
    return out;
}

}


