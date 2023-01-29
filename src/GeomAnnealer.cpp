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
 * @file GeomAnnealer.cpp
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This definition file contains definitions of all functions and classes of GeomAnnealer.
 */

#include "../include/GeomAnnealer.h"
#include <omp.h>
#include <limits>
#include <meshlib.h>
#include <numeric>
#include <fstream>

namespace simugeom
{

GeomAnnealer::GeomAnnealer(GeomScene& _gsn) : gsn(_gsn),
    cost_old(std::numeric_limits<double>::max()),
    cost_new(std::numeric_limits<double>::max()),
    cost_best(std::numeric_limits<double>::max()),
    alpha(1.0), beta(std::numeric_limits<double>::max()),
    sigmpos(0.5), sigmrot(0.5), curr_iter(-1), maxiters(500), num_proposals(1),
    pose_best(gsn.get_models().size()), has_renderer(false), grdr(nullptr)
{
}

GeomAnnealer::~GeomAnnealer()
{
}

void GeomAnnealer::download_best_solution()
{
    const int num_models = gsn.get_models().size();
    for(int i=0; i<num_models; ++i)
    {
        GeomModel& tmodel = gsn.get_model(i);
        pose_best[i] = tmodel.pose;
    }
}

void GeomAnnealer::upload_best_solution()
{
    const int num_models = gsn.get_models().size();
    for(int i=0; i<num_models; ++i)
    {
        GeomModel& tmodel = gsn.get_model(i);
        tmodel.pose = pose_best[i];
    }
}

void GeomAnnealer::set_renderer(GeomRenderer& _grdr)
{
    grdr = &_grdr;
    has_renderer = true;
    grdr->initSDL();
}

void GeomAnnealer::initialise()
{
    gsn.scatter();
    curr_iter = 0;
    cost_old = gsn.get_cost_total();//std::numeric_limits<double>::max();
    cost_new = cost_old;
    cost_best = cost_new;
    download_best_solution();
    if(has_renderer)
    {
        grdr->set_thickness(3);
        grdr->set_iteration(curr_iter, maxiters);
        grdr->render(true);
    }
}

void GeomAnnealer::iterate(const double beta2)
{
    ++curr_iter;
    beta = 1.0/(double(curr_iter)*curr_iter);
    // generate random permutation sequence for one epoch
    const int num_models = gsn.get_models().size();
    std::vector<int> seq(num_models);
    std::iota(std::begin(seq), std::end(seq), 0);
    std::shuffle(std::begin(seq), std::end(seq), gsn.rng);
    // select one model in order and perturb
    sigmpos = 0.5*beta2*std::sqrt(gsn.bbox.rad(0,0)*gsn.bbox.rad(0,0)+gsn.bbox.rad(1,0)*gsn.bbox.rad(1,0));
    sigmrot = 0.5*beta2*MESH_TWOPI;
    //std::cout<<"[ITER "<<curr_iter<<"]: old: "<<cost_old<<" best: "<<cost_best<<std::endl;
    for(int i=0; i<num_models; ++i)
    {

        GeomModel& tmodel = gsn.get_model(seq[i]);
        const int32_t cindx = i+(curr_iter-1)*num_models;
        cost_bests[cindx] = cost_best;
        cost_news[cindx] = cost_new;

        if(gsn.classes.get_class(tmodel.get_class_id()).is_fixed) continue;
        tmodel.propose_perturb(num_proposals, sigmpos, sigmrot, gsn);


        for(int k=0; k<num_proposals; ++k)
        {
            // for each ith proposal, try
            std::swap(tmodel.pose,tmodel.proposed_poses[k]);
            if(has_renderer)
            {
                grdr->set_thickness(1);
                grdr->set_iteration(curr_iter, maxiters);
                grdr->render(true);
            }
            cost_new = gsn.get_cost_total();

            if(cost_new<cost_best)
            {
                // accept new pose
                cost_best = cost_new;
                download_best_solution();
                continue;
            }
            alpha = std::exp((cost_old-cost_new)/beta);
            if((cost_new<cost_old) || (alpha>0.5*(gsn.unidist(gsn.rng)+1.0)))
            {
                cost_old = cost_new;
            }
            else
            {
                // retrieve the old solution
                std::swap(tmodel.pose, tmodel.proposed_poses[k]);
            }
        }
    }
    if(has_renderer)
    {
        grdr->set_thickness(3);
        grdr->set_iteration(curr_iter, maxiters);
        grdr->render(true);
    }
}

void GeomAnnealer::export_cost_graph(const char* fname)
{
    std::ofstream ofs(fname, std::ios::binary);
    if(ofs.is_open())
    {
        const int32_t n = cost_bests.size();
        for(int32_t i=0; i<n; ++i)
        {
            ofs<<i<<" "<<cost_news[i]<<" "<<cost_bests[i]<<"\n";
        }
    }
}

void GeomAnnealer::set_maxiters(const int32_t _maxiters)
{
    maxiters = _maxiters;
}
void GeomAnnealer::solve()
{
    initialise();
    cost_bests.resize(maxiters*gsn.get_models().size());
    cost_news.resize(maxiters*gsn.get_models().size());
    for(int32_t i=0; i<maxiters; ++i)
    {
        const auto d = ((double)i)/maxiters;
        const auto d2 = d*d;
        // const auto beta2 = (1.-d);
        // const auto beta2 = (1.-d2);
        const auto beta2 = (1.-d+d2*(1-d2));
        // const auto beta2 = (1.-d2-d2*(d-d2));
        // other functions
        // (1-x+x^2-x^4)
        // (1-x^2-x^3+x^4)
        iterate(beta2);
    }
    upload_best_solution();
}

}


