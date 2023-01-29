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
 * @file GeomScene.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomScene.
 */

#include "../include/GeomScene.h"
#include "../include/GeomModel.h"
#include "../include/ObjClassSet.h"
#include <cmath>
#include <ctime>
#include <chrono>
#include <meshlib.h>

namespace simugeom
{

GeomScene::GeomScene()
    : param_alpha(2.0), seed(std::chrono::system_clock::now().time_since_epoch().count()), rng(seed), unidist(-1.0, 1.0)
{
}

GeomScene::GeomScene(const ObjClassSet& _classes)
    : classes(_classes), param_alpha(2.0), seed(std::chrono::system_clock::now().time_since_epoch().count()), rng(seed), unidist(-1.0, 1.0)
{
}

GeomScene::GeomScene(ObjClassSet&& _classes)
    : classes(_classes), param_alpha(2.0), seed(std::chrono::system_clock::now().time_since_epoch().count()), rng(seed), unidist(-1.0, 1.0)
{
}

GeomScene::~GeomScene()
{
}

int GeomScene::insert(const GeomModel& gm)
{
    if(gm.get_class_id()<0 ||gm.get_class_id()>classes.get_num_classes()) return -1;
    const int old_num_models = models.size();
    models.push_back(gm);
    models[old_num_models].set_object_id(old_num_models); /* force set oid */
    return 0;
}

int GeomScene::insert(GeomModel&& gm)
{
    if(gm.get_class_id()<0 ||gm.get_class_id()>classes.get_num_classes()) return -1;
    const int old_num_models = models.size();
    models.push_back(gm);
    models[old_num_models].set_object_id(old_num_models); /* force set oid */
    return 0;
}

double GeomScene::get_dist(int oid0, int oid1) const
{
    const auto d = (models[oid0].get_pose().pos-models[oid1].get_pose().pos);
    return std::sqrt(d(0,0)*d(0,0)+d(1,0)*d(1,0));
}

double GeomScene::get_bb_dist(int oid0, int oid1) const
{
    // simplified by removing dependency of angles
    const auto& pos0 = models[oid0].get_pose().pos;
    const auto& pos1 = models[oid1].get_pose().pos;

    return (models[oid0].get_bb_radius_from(pos1(0,0), pos1(1,0))
            +models[oid1].get_bb_radius_from(pos0(0,0), pos0(1,0)));
}

double GeomScene::get_bb_dist_diag(int oid0, int oid1) const
{
    auto r0 = models[oid0].get_radius();
    auto r1 = models[oid1].get_radius();
    return (std::sqrt(r0(0,0)*r0(0,0)+r0(1,0)*r0(1,0))
            +std::sqrt(r1(0,0)*r1(0,0)+r1(1,0)*r1(1,0)));
}

double GeomScene::get_max_reco_dist(int oid0, int oid1) const
{
    return classes.get_max_reco_dist(models[oid0].get_class_id(), models[oid1].get_class_id());
}

double GeomScene::get_angle(int oid0, int oid1) const
{
    return (models[oid0].get_pose().rot-models[oid1].get_pose().rot);
}

double GeomScene::get_dist(int oid0, int oid1, int oid2) const
{
    return (models[oid0].get_pose().pos-0.5*(models[oid1].get_pose().pos+models[oid2].get_pose().pos)).norm();
}

double GeomScene::get_bb_dist(int oid0, int oid1, int oid2) const
{
    const auto pos_centroid12 = 0.5*(models[oid1].get_pose().pos+models[oid2].get_pose().pos);
    const auto& pos1 = models[oid1].get_pose().pos;
    const auto& pos2 = models[oid2].get_pose().pos;
    return (models[oid0].get_bb_radius_from(pos_centroid12(0,0), pos_centroid12(1,0))
            +(models[oid1].get_bb_radius_from(pos1(0,0), pos1(1,0))
              +models[oid2].get_bb_radius_from(pos2(0,0), pos2(1,0))
              +get_dist(oid1, oid2)));
}

double GeomScene::get_reco_dist(int oid0, int oid1) const
{
    return classes.get_reco_dist(models[oid0].get_class_id(), models[oid1].get_class_id());
}

const std::vector<double>& GeomScene::get_reco_angles(int oid0, int oid1) const
{
    return classes.get_reco_angles(models[oid0].get_class_id(), models[oid1].get_class_id());
}

double GeomScene::get_cost_bb_intersect(int oid0, int oid1) const
{
    return std::max(0.0, get_bb_dist(oid0, oid1)-get_dist(oid0, oid1));
}

double GeomScene::get_cost_bb_intersect_diag(int oid0, int oid1) const
{
    return std::max(0.0, get_bb_dist_diag(oid0, oid1)-get_dist(oid0, oid1));
}

double GeomScene::get_cost_pairwise_dist(int oid0, int oid1) const
{
    double cost = 0.0;
    const auto bb = get_bb_dist(oid0, oid1);
    const auto dist = get_dist(oid0, oid1);
    if(dist<bb) cost = std::pow(bb/dist, param_alpha);
    const auto mrd = get_max_reco_dist(oid0, oid1);
    if(dist>mrd)
    {
        cost = std::pow(dist/mrd, param_alpha);
    }
    return cost;
}

double GeomScene::get_cost_visibility(int oid0, int oid1, int oid2) const
{
    return std::max(0.0, get_bb_dist(oid0, oid1, oid2)-get_dist(oid0, oid1, oid2));
}

double GeomScene::get_cost_fixed_dist(int oid0, int oid1) const
{
    const auto distdiff = get_dist(oid0, oid1)-get_reco_dist(oid0, oid1);
    return distdiff*distdiff;
}

double GeomScene::get_cost_fixed_angle(int oid0, int oid1) const
{
	double cost = 0.0;
	const auto& rang = get_reco_angles(oid0, oid1);
	double angle = get_angle(oid0, oid1);
	const int32_t valn = rang.size();
	for(int i=0; i<valn; ++i)
	{
		double anglediff = rang[i]-angle;
		anglediff = std::atan2(std::sin(anglediff), std::cos(anglediff));
		if(i==0)
		{
			cost = anglediff*anglediff;
		}
		else
		{
			cost = std::min(cost, anglediff*anglediff);
		}
	}
    return cost;
}

double GeomScene::get_cost_total() const
{
    double tcost = 0.0;
    const int32_t num_models = models.size();
    #pragma omp parallel for reduction(+:tcost)
    for(int i=0; i<num_models; ++i)
    {
        double subtcost = 0.0;
        const bool is_fixed_i = classes.get_class(models[i].get_class_id()).is_fixed;
        for(int j=(i+1); j<num_models; ++j)
        {
            const bool is_fixed_j = classes.get_class(models[j].get_class_id()).is_fixed;

            if(is_fixed_i && is_fixed_j) continue;

            // subtcost += 1000*get_cost_bb_intersect(i, j);
            if(is_fixed_i || is_fixed_j)
            {
                subtcost += 1000*get_cost_bb_intersect(i, j);
            }
            else
            {
                subtcost += 500*get_cost_bb_intersect_diag(i, j);
            }
            subtcost += 0.1*get_cost_pairwise_dist(i, j);
            for(int k=0; k<num_models; ++k)
            {
                if(k==i||k==j)
                {
                    continue;
                }
                subtcost += 0.05*get_cost_visibility(k, i, j);
            }
        }
        // find the nearest wall for non-wall objects (i) only
        if(!classes.get_class(models[i].get_class_id()).is_fixed)
        {
            int nearest_wid = -1;
            double nearest_wall_dist = std::numeric_limits<double>::max();
            for(int j=0; j<num_models; ++j)
            {
                if(!classes.get_class(models[j].get_class_id()).is_fixed ||(i==j)) continue;
                double curr_wall_dist = get_dist(i, j);
                if(curr_wall_dist<nearest_wall_dist)
                {
                    nearest_wall_dist = curr_wall_dist;
                    nearest_wid = j;
                }
            }
            {
                // now once got nearest wall
                subtcost += 3*get_cost_fixed_angle(i, nearest_wid);
                subtcost += 0.05*get_cost_fixed_dist(i, nearest_wid);
            }
        }
        tcost = tcost+subtcost;
    }
    return tcost;
}

const GeomPose GeomScene::generate_random_pose()
{
    // implement better generation by considering equal density along
    // x and y directions without scaling, but using rejection
    GeomPose pose;
    if(boundary.get_points().size()>0)
    {
        pose.pos(0,0) = bbox.pos(0,0)+bbox.rad(0,0)*unidist(rng);
        pose.pos(1,0) = bbox.pos(1,0)+bbox.rad(1,0)*unidist(rng);
    }
    else
    {
        pose.pos(0,0) = unidist(rng);
        pose.pos(1,0) = unidist(rng);
    }
    pose.pos(2,0) = 0.0;
    pose.rot = MESH_PI*unidist(rng);
    return pose;
}

void GeomScene::scatter()
{
    const int n = models.size();
    for(int i=0; i<n; ++i)
    {
        GeomModel& tmodel = models[i];
        if(!classes.get_class(tmodel.get_class_id()).is_fixed)
        {
            GeomPose& pose = tmodel.get_pose();
            pose = generate_random_pose();
        }
    }
}


std::ostream& operator <<(std::ostream& out, const GeomScene& s)
{
    out<<"GeomScene[";
    const int n = s.get_models().size();
    for(int i=0; i<n; ++i)
    {
        out<<s.get_model(i)<<", ";
    }
    out<<"]";
    return out;
}

}

