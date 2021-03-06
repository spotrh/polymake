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

#ifndef _POLYMAKE_COMPRESS_INCIDENCE_H
#define _POLYMAKE_COMPRESS_INCIDENCE_H

#include "polymake/Set.h"
#include "polymake/IncidenceMatrix.h"
#include "polymake/FacetList.h"

namespace polymake { namespace polytope {

/// @retval row indices of (non-facets, hidden equations), first includes second
template <typename IM>
std::pair< Set<int>, Set<int> >
compress_incidence(const GenericIncidenceMatrix<IM>& VIF)
{
   Set<int> non_facets, hidden_equations;
   const int nv=VIF.cols();
   FacetList facets(nv);

   for (typename Entire< Rows<IM> >::const_iterator f=entire(rows(VIF)); !f.at_end(); ++f) {
      if (f->size() == nv) {
         facets.skip_facet_id();
         non_facets.push_back(f.index());
         hidden_equations.push_back(f.index());
      } else if (! facets.replaceMax(*f, inserter(non_facets))) {
         non_facets.push_back(f.index());
      }
   }
   return std::make_pair(non_facets, hidden_equations);
}

} }

#endif // _POLYMAKE_COMPRESS_INCIDENCE_H

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
