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

//*****************************************************************************
// 
// NOTE THAT THIS FILE IS BASED ON MATERIAL FROM glN64.
// http://gln64.emulation64.com/
//
//*****************************************************************************

#include "CombinerStructs.h"

//-----------------------------------------------------------------------------
//* Merge Stages
//! Try two merge to stages into one
//! From glN64
//-----------------------------------------------------------------------------
void mergeStages(Combiner* c)
{

    // If all we have is a load in the first stage we can just replace
    // each occurance of COMBINED in the second stage with it
    if ((c->stage[0].numOps == 1) && (c->stage[0].op[0].op == LOAD))
    {
        int combined = c->stage[0].op[0].param1;

        for (int i = 0; i < c->stage[1].numOps; i++)
        {
            c->stage[0].op[i].op = c->stage[1].op[i].op;
            c->stage[0].op[i].param1 = (c->stage[1].op[i].param1 == COMBINED) ? combined : c->stage[1].op[i].param1;
            c->stage[0].op[i].param2 = (c->stage[1].op[i].param2 == COMBINED) ? combined : c->stage[1].op[i].param2;
            c->stage[0].op[i].param3 = (c->stage[1].op[i].param3 == COMBINED) ? combined : c->stage[1].op[i].param3;
        }

        c->stage[0].numOps = c->stage[1].numOps;
        c->numStages = 1;
    }
    // We can't do any merging on an interpolation
    else if (c->stage[1].op[0].op != INTERPOLATE)
    {
        int numCombined = 0;

        // See how many times the first stage is used in the second one
        for (int i = 0; i < c->stage[1].numOps; i++)
            if (c->stage[1].op[i].param1 == COMBINED)
                numCombined++;

        // If it's not used, just replace the first stage with the second
        if (numCombined == 0)
        {
            for (int i = 0; i < c->stage[1].numOps; i++)
            {
                c->stage[0].op[i].op = c->stage[1].op[i].op;
                c->stage[0].op[i].param1 = c->stage[1].op[i].param1;
                c->stage[0].op[i].param2 = c->stage[1].op[i].param2;
                c->stage[0].op[i].param3 = c->stage[1].op[i].param3;
            }
            c->stage[0].numOps = c->stage[1].numOps;

            c->numStages = 1;
        }
        // If it's only used once
        else if (numCombined == 1)
        {
            // It's only used in the load, so stack on the ops from stage 2 on stage 1
            if (c->stage[1].op[0].param1 == COMBINED)
            {
                for (int i = 1; i < c->stage[1].numOps; i++)
                {
                    c->stage[0].op[c->stage[0].numOps].op = c->stage[1].op[i].op;
                    c->stage[0].op[c->stage[0].numOps].param1 = c->stage[1].op[i].param1;
                    c->stage[0].numOps++;
                }

                c->numStages = 1;
            }
            // Otherwise, if it's used in the second op, and that op isn't SUB
            // we can switch the parameters so it works out to tack the ops onto stage 1
            else if ((c->stage[1].op[1].param1 == COMBINED) && (c->stage[1].op[1].op != SUB))
            {
                c->stage[0].op[c->stage[0].numOps].op = c->stage[1].op[1].op;
                c->stage[0].op[c->stage[0].numOps].param1 = c->stage[1].op[0].param1;
                c->stage[0].numOps++;

                // If there's another op, tack it onto stage 1 too
                if (c->stage[1].numOps > 2)
                {
                    c->stage[0].op[c->stage[0].numOps].op = c->stage[1].op[2].op;
                    c->stage[0].op[c->stage[0].numOps].param1 = c->stage[1].op[2].param1;
                    c->stage[0].numOps++;
                }

                c->numStages = 1;
            }
        }
    }
}
