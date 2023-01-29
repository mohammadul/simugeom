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
 * @file ObjClassSet.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of ObjClassSet.
 */

#ifndef OBJCLASSSET_H
#define OBJCLASSSET_H

#include <memory>
#include <vector>
#include <map>
#include <string>

namespace simugeom
{

class ObjClass;
class GeomSceneReader;

class ObjClassSet
{
public:
    ObjClassSet();
    virtual ~ObjClassSet();
    int insert(ObjClass& objcls);
    int insert(ObjClass&& objcls);
    template<typename ...Args>
    int insert(Args&&... args)
    {
        return this->insert(ObjClass(std::forward<Args>(args)...));
    }

    const ObjClass& get_class(const int cid) const
    {
        return classes[cid];
    }

    const ObjClass& get_class(const std::string& name) const
    {
        return classes[searcher.find(name)->second];
    }

    double get_max_reco_dist(int32_t cid0, int32_t cid1) const;
    double get_reco_dist(int32_t cid0, int32_t cid1) const;
	const std::vector<double>& get_reco_angles(int32_t cid0, int32_t cid1) const;

    std::vector<std::vector<double>>& get_all_max_reco_dists() { return max_reco_dist; }
    std::vector<std::vector<double>>& get_all_reco_dists() { return reco_dist; }
    std::vector<std::vector<std::vector<double>>>& get_all_reco_angles() { return reco_angles; }

    int32_t get_num_classes() const
    {
        return classes.size();
    }

private:
    std::vector<ObjClass> classes;
    std::vector<std::vector<double>> max_reco_dist;
    std::vector<std::vector<double>> reco_dist;
	std::vector<std::vector<std::vector<double>>> reco_angles;

    std::map<const std::string, int32_t> searcher;

    friend std::ostream& operator <<(std::ostream& out, const ObjClassSet& s);
    friend class GeomSceneReader;
    friend class GeomSceneWriter;
};


std::ostream& operator <<(std::ostream& out, const ObjClassSet& s);


}

#endif // OBJCLASSSET_H
