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
 * @file GeomSceneIO.h
 * @author Sk. Mohammadul Haque
 * @version 0.1.0.0
 * @copyright
 * Copyright (c) 2019, 2023 Sk. Mohammadul Haque.
 * @brief This header file contains declarations of all functions and classes of GeomSceneIO.
 */

#ifndef GEOMSCENEIO_H
#define GEOMSCENEIO_H

#include "GeomScene.h"

namespace simugeom
{

class GeomSceneReader
{
    public:
        GeomSceneReader(GeomScene& _gs);
        virtual ~GeomSceneReader();

        int read(const char* fname);

    private:
        GeomScene& gs;
};

class GeomSceneWriter
{
    public:
        GeomSceneWriter(const GeomScene& _gs);
        virtual ~GeomSceneWriter();

        int write(const char* fname);

    private:
        const GeomScene& gs;
};

}

#endif // GEOMSCENEIO_H
