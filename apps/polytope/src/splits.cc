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
#include "polymake/Matrix.h"
#include "polymake/PowerSet.h"
#include "polymake/Graph.h"
#include "polymake/linalg.h"

namespace polymake { namespace polytope {

template<typename Scalar>
Matrix<Scalar> splits(const Matrix<Scalar>& vert, const Graph<>& graph, const Matrix<Scalar>& facets, const int d, perl::OptionSet options)
{
   Set<int> coords;
   options["Coordinates"]>>coords; 
   const int n=vert.rows();  
   const int a_dim=vert.cols()-1;

   ListMatrix< Vector<Scalar> > splits;
   //test for each d vertices if they lie in a splitting hyperplane
   for (Entire< Subsets_of_k<const sequence&> >::const_iterator i=entire(all_subsets_of_k(sequence(0,n),d)); !i.at_end(); ++i) {
      const Matrix<Scalar> h=vert.minor(*i,All);
    
      //they must have full rank
      if (rank(h)==d) {
         bool neu=true;

         //exclude facets and sets that are already in a split
         for (typename Entire< std::list<Vector <Scalar> > >::const_iterator j=entire(rows(splits)); neu&&!j.at_end(); ++j) 
            if (is_zero(h*(*j))) neu=false;
         for (typename Entire< Rows <Matrix <Scalar> > >::const_iterator j=entire(rows(facets)); neu&&!j.at_end(); ++j) 
            if (is_zero(h*(*j))) neu=false;

         if (neu) {
            int pos=0;
            Matrix<Scalar> o(a_dim-d,a_dim+1);
            for (Entire< Set <int> >::const_iterator k=entire(coords); !k.at_end(); ++k) o(pos++,*k)=1; 
            for (; pos<a_dim-d; ++pos) o(pos,pos)=1;
            Vector<Scalar> a=null_space(h/o).row(0);

            bool is_split=true;

            //test for each edge of it is cut by the hyperplane, which means that it does not define a split.
            for(Entire<Edges< Graph <> > >::const_iterator e=entire(edges(graph)); is_split&&!e.at_end();++e)
               if ((a*vert.row(e.from_node()))*(a*vert.row(e.to_node()))<0) is_split=false;
        
            if (is_split) {
               //normalizes the split, i.e. divides by the first non-zero entry
               Scalar fac;
               for (int k=0;(fac=a[k])==0;++k);
               a/=fac;
               splits/=a;
            }
         }
      }
   }

   return splits;
}

UserFunctionTemplate4perl("# @category Triangulations, subdivisions and volume"
                          "# Computes the [[Polytope::SPLITS|SPLITS]] of a polytope."
                          "# The splits are normalized by dividing by the first non-zero entry."
                          "# If the polytope is not fulldimensional the first entries are set to zero unless //coords//"
                          "# are specified."
                          "# @param Matrix V vertices of the polytope"
                          "# @param Graph G graph of the polytope"
                          "# @param Matrix F facets of the polytope"
                          "# @param Int dimension of the polytope"
                          "# @option Set<Int> coords entries that should be set to zero"
                          "# @return Matrix",
                          "splits<Scalar>(Matrix<type_upgrade<Scalar>> $ Matrix<type_upgrade<Scalar>> $;{ coords => undef })");
} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
