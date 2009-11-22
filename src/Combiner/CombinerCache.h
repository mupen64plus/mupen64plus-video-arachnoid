/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#ifndef COMBINER_CACHE_H_
#define COMBINER_CACHE_H_

#include "CombinerStructs.h"
#include <list>

//*****************************************************************************
//* Cached Combiner
//! Struct used to store decoded mux values and the result of them
//*****************************************************************************
struct CachedCombiner
{
    unsigned long long mux;               //Decoded value defining how to combine colors
    TexEnvCombiner* compiled;
};

//*****************************************************************************
//* Combiner Cache
//! Class used to store and retrive decoded mux values and the result of them.
//*****************************************************************************
class CombinerCache
{
public:

    //Add/Store decoded mux value and the result
    void newCompiledCombiner(unsigned long long mux, TexEnvCombiner* compiled);

    //Try to find decoded mux value, (return 0 if not found)
    CachedCombiner* findCachedCombiner(unsigned long long mux);

    //Destroy
    void dispose();

private:

    typedef std::list<CachedCombiner*> CombinerList;  //!< Type used to store combiled texture combiners
    CombinerList m_cachedCombiners;                   //!< List of cached combiners

};


#endif
