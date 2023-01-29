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
 * @file GeomAnnealer.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of GeomAnnealer.
 */

#ifndef GEOMANNEALER_H
#define GEOMANNEALER_H

#include "GeomScene.h"
#include "GeomRenderer.h"

namespace simugeom
{

class GeomAnnealer
{
    public:
        GeomAnnealer(GeomScene& _gsn);
        virtual ~GeomAnnealer();

        void set_num_proposals(const int32_t n) { num_proposals = n; }
        void set_renderer(GeomRenderer& _grdr);
        void initialise();
        void iterate(const double beta = 1.0);
        void set_maxiters(const int32_t _maxiters = 500);
        void solve();
        void export_cost_graph(const char* fname);

    protected:
        void download_best_solution();
        void upload_best_solution();

    private:
        GeomScene& gsn;
        double cost_old, cost_new, cost_best;
        double alpha, beta;
        double sigmpos, sigmrot;
        int32_t curr_iter;
        int32_t maxiters;
        int32_t num_proposals;
        std::vector<GeomPose> pose_best;
        std::vector<double> cost_bests, cost_news;
        bool has_renderer;
        GeomRenderer* grdr;
};

}

#endif // GEOMANNEALER_H
