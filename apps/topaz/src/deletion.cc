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

#include "polymake/client.h"
#include "polymake/topaz/complex_tools.h"
#include "polymake/list"
#include "polymake/Set.h"
#include "polymake/PowerSet.h"
#include "polymake/FacetList.h"

namespace polymake { namespace topaz {
        
// Delete face from the list of facets C
// Returns 0 if face was not a subset of any of the facets of C (C remains unchanged),
// returns 1 otherwise
bool delete_face(FacetList &C, const Set<int>& face)
{
   std::list< Set<int> > deletedFaces;
   if (C.eraseMin(face, back_inserter(deletedFaces))) {
      do {
         // insert the parts of the boundary not containing the face
         const Set<int> deleted = deletedFaces.front();  deletedFaces.pop_front();
         const bool exact_match = deleted.size() == face.size();
         for (Subsets_less_1<const Set<int>&>::const_iterator boundaryIt=all_subsets_less_1(deleted).begin();  !boundaryIt.at_end(); ++boundaryIt) {
            if (!exact_match && (face - *boundaryIt).empty()) {
               // face is a proper subset of the boundary face too
               deletedFaces.push_back(*boundaryIt);
            } else {
               C.insertMax(*boundaryIt);
            }
         }
      } while (! deletedFaces.empty());
      return true;
   }
   return false;
}

perl::Object deletion_complex(perl::Object p_in, const Set<int>& face, perl::OptionSet options)      
{       
   FacetList facets = p_in.give("FACETS");      
   const int n_vert = p_in.give("N_VERTICES");
   if (face.empty())
      throw std::runtime_error("deletion: empty face specified");
   if (face.front()<0 || face.back()>n_vert-1)
      throw std::runtime_error("deletion: specified vertex indices out of range");
   
   if ( !delete_face(facets,face) ) {
      std::ostringstream e;
      wrap(e) << "deletion: " << face << " does not specify a face.";
      throw std::runtime_error(e.str());
   }

   perl::Object p_out("SimplicialComplex");
   p_out.set_description() << "Deletion of " << face << " in " << p_in.name() << ".\n";

   Set<int> V;
   if (facets.empty()) {
      p_out.take("FACETS") << Array< Set<int> >(1);
   } else {
      Array< Set<int> > array_facets = as_array(lex_ordered(facets));
      V = accumulate(array_facets, operations::add());
      adj_numbering(array_facets,V);
      p_out.take("FACETS") << array_facets;
   }

   if (!options["no_labels"]) {
      const Array<std::string> L = p_in.give("VERTEX_LABELS");
      const Array<std::string> new_L(V.size(), select(L,V).begin());
      p_out.take("VERTEX_LABELS") << new_L;
   }

   return p_out;
}

UserFunction4perl("# @category Producing a new simplicial complex from others"
                  "# Remove the given //face// and all the faces containing it."
                  "# @param SimplicialComplex complex"
                  "# @param Set<Int> face specified by vertex indices."
                  "#  Please use [[labeled_vertices]] if you want to specify the face by vertex labels."
                  "# @option Bool no_labels do not write vertex labels."
                  "# @return SimplicialComplex",
                  &deletion_complex, "deletion(SimplicialComplex $ { no_labels => 0 } )");
} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
