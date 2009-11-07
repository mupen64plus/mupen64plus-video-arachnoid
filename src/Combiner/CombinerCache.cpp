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