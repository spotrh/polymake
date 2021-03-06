/* Copyright (c) 1997-2015
   Ewgenij Gawrilow, Michael Joswig (Technische Universitaet Berlin, Germany)
   http://www.polymake.org

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version: http://www.gnu.org/licenses/gpl.txt.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
--------------------------------------------------------------------------------
*/

#ifndef POLYMAKE_TOPAZ_MERGE_VERTICES_H
#define POLYMAKE_TOPAZ_MERGE_VERTICES_H

#include "polymake/Array.h"
#include "polymake/Set.h"

#include "polymake/hash_map"
#include <string>

namespace polymake { namespace topaz {

/** Merges two vertex sets according to their labels.
 *  Afterwards the first set of labels contains all new
 *  labels and the client returns a Map<int,int> mapping 
 *  the vertices of the old complex to their new indices.
 *
 *  The corresponding vertices of L1 and L2 are expected
 *  be numbered according to the topaz standard, that is
 *  consecutively starting with 0.
 */

hash_map<int,int> merge_vertices (Array<std::string>& L1, const Array<std::string>& L2);

void merge_disjoint_vertices (Array<std::string>& L1, const Array<std::string>& L2);

} }

#endif // POLYMAKE_TOPAZ_MERGE_VERTICES_H

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
