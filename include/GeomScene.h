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
 * @file GeomScene.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of GeomScene.
 */

#ifndef GEOMSCENE_H
#define GEOMSCENE_H

#include <memory>
#include <vector>
#include <iostream>
#include <random>
#include "GeomModel.h"
#include "ObjClass.h"
#include "ObjClassSet.h"
#include "GeomValidity.h"

namespace simugeom
{

class GeomScene
{
public:
    GeomScene();
    GeomScene(const ObjClassSet& _classes);
    GeomScene(ObjClassSet&& _classes);
    virtual ~GeomScene();

    int insert(const GeomModel& _model);
    int insert(GeomModel&& _model);
    template<typename ...Args>
    int insert(Args&&... args)
    {
        return this->insert(GeomModel(std::forward<Args>(args)...));
    }

    void set_seed(const uint32_t _seed)
    {
        seed = _seed;
    }

    void set_boundary(const AABB& _bbox)
    {
        bbox = _bbox;
        boundary = Polygon2D(bbox);
    }

    int32_t get_num_classes() const
    {
        return classes.get_num_classes();
    }

    const ObjClassSet& get_classes() const
    {
        return classes;
    }

    const ObjClass& get_class(int cid) const
    {
        return classes.get_class(cid);
    }

    const ObjClass& get_class(const std::string& name) const
    {
        return classes.get_class(name);
    }

    const std::vector<GeomModel>& get_models() const
    {
        return models;
    }

    const std::vector<GeomModel>& get_models()
    {
        return models;
    }

    const GeomModel& get_model(const int i) const
    {
        return models[i];
    }

    GeomModel& get_model(const int i)
    {
        return models[i];
    }

    double get_dist(int oid0, int oid1) const;
    double get_bb_dist(int oid0, int oid1) const;
    double get_bb_dist_diag(int oid0, int oid1) const;
    double get_max_reco_dist(int oid0, int oid1) const;
    double get_angle(int oid0, int oid1) const;
    double get_dist(int oid0, int oid1, int oid2) const;
    double get_bb_dist(int oid0, int oid1, int oid2) const;

    double get_reco_dist(int oid0, int oid1) const;
	const std::vector<double>& get_reco_angles(int oid0, int oid1) const;

    double get_cost_bb_intersect(int oid0, int oid1) const;
    double get_cost_bb_intersect_diag(int oid0, int oid1) const;
    double get_cost_pairwise_dist(int oid0, int oid1) const;
    double get_cost_visibility(int oid0, int oid1, int oid2) const;
    double get_cost_fixed_dist(int oid0, int oid1) const;
    double get_cost_fixed_angle(int oid0, int oid1) const;

    double get_cost_total() const;

    void scatter();

protected:
     const GeomPose generate_random_pose();

private:
    ObjClassSet classes;
    double param_alpha;
    std::vector<GeomModel> models;

    uint32_t seed;
    std::mt19937 rng;
    std::uniform_real_distribution<double> unidist;
    AABB bbox;
    Polygon2D boundary;
    friend class GeomSceneReader;
    friend class GeomSceneWriter;
    friend class GeomAnnealer;
    friend class GeomModel;
    friend class GeomRenderer;
};

std::ostream& operator <<(std::ostream& out, const GeomScene& s);

}

#endif // GEOMSCENE_H
