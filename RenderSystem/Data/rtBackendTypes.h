/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
/*!
 \addtogroup DataApi
 @{
 The DataApi module is a layer of structures that provide
 direct kernel access to the private members of the \ref FrontEnd types.
 \dot
 
digraph
{
    bgcolor  = none;
    compound = true;

    K1[label = "Cpu Kernel" shape = box style = filled fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    K2[label = "Cuda Kernel" shape = box style = filled fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    D[label = "Data Api" shape = box style = filled fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    CL[label = "Central Logic" shape = box style = filled fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    BA[label = ".bAscii" shape = box style = filled fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    BL[label = ".blend" shape = box style = filled fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];

    U[label = "User Access" shape = box
              style               = filled
              fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    L[label = "Loader" shape = box
              style          = filled
              fontcolor = "#AFAFAF" fillcolor = "#5b5b5b" color = "#222222"];
    CL->U[lhead = cluster_0 minlen = 2];
    subgraph cluster_1
    {
        shape             = box
                style     = filled
                fontcolor = "#AFAFAF" color = "#222222";
        L->BA;
        L->BL;
    }
    L->U[ltail = cluster_1 lhead = closter_0];

    subgraph cluster_0
    {
        label = "Front End" shape = box
                style             = filled
                fontcolor = "#AFAFAF" color = "#222222";
        D;
        U;
    }
    U->D[lhead = cluster_0];
    D->K2[lhead = cluster_2 minlen = 2];

    subgraph cluster_2
    {
        label = "Kernels" shape = box
                style           = filled
                fontcolor = "#AFAFAF" color = "#222222";
        K1;
        K2;
    }
}
\enddot
*/


#ifndef _rtBackendTypes_h_
#define _rtBackendTypes_h_

#include "Utils/Config/skConfig.h"

/// <summary>
/// Used to specify the allocation
/// back-end for kernel code.
/// </summary>
enum rtBackendTypes
{
    /// <summary>
    /// Allocate through new and delete.
    /// </summary>
    RT_CPU = 0,

    /// <summary>
    /// Allocate with cuda shared memory.
    /// </summary>
    RT_CUDA,

    /// <summary>
    /// TODO
    /// </summary>
    RT_OPEN_CL,
};

/*! @} */

#endif  //_rtBackendTypes_h_
