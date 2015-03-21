/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * https://github.com/mupen64plus/mupen64plus-video-arachnoid/
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

#include "CombinerCache.h"

//-----------------------------------------------------------------------------
//* New Compiled Combiner
//! Function used to add decoded mux values and the result of them to the list.
//-----------------------------------------------------------------------------
void CombinerCache::newCompiledCombiner(unsigned long long mux, TexEnvCombiner* compiled)
{
    //Create new Combiner
    CachedCombiner* newCombiner = new CachedCombiner();
    newCombiner->mux      = mux;
    newCombiner->compiled = compiled;

    //Add Combiner to list
    m_cachedCombiners.push_back(newCombiner);
}

//-----------------------------------------------------------------------------
//* New Compiled Combiner
//! Function used to retrive decoded mux values and the result of them to the list.
//-----------------------------------------------------------------------------
CachedCombiner* CombinerCache::findCachedCombiner(unsigned long long mux)
{
    for (CombinerList::iterator it = m_cachedCombiners.begin(); it!=m_cachedCombiners.end(); ++it)
    {
        if ( (*it)->mux == mux )
        {
            return (*it); //Found old combiner!!
        }
    }

    return 0; //Could not find it
}

//-----------------------------------------------------------------------------
//* Dispose
//! Destroys all values in list.
//-----------------------------------------------------------------------------
void CombinerCache::dispose()
{
    for (CombinerList::iterator it = m_cachedCombiners.begin(); it!=m_cachedCombiners.end(); ++it)
    {
        delete (*it)->compiled;
        delete (*it);
    }

    m_cachedCombiners.clear();
}
