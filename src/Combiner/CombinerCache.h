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