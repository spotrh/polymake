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

#include "polymake/graph/connected.h"
#include "polymake/Graph.h"

namespace polymake { namespace topaz {
namespace {

// return values: 1=true, 0=false, -1=undef (does not occur here)
template <typename Complex>
int fill_graph(Graph<>& G, const Complex& C, int *bad_link_p)
{
   // check whether each vertex is contained in 1 or 2 edges
   for (typename Entire<Complex>::const_iterator c_it=entire(C); !c_it.at_end(); ++c_it) {
      typename Complex::value_type::const_iterator f_it=c_it->begin();
      const int n1=*f_it, n2=*++f_it;
      G.edge(n1,n2);
      if (G.degree(n1) > 2) {
         if (bad_link_p) *bad_link_p=n1;
         return 0;
      }
      if (G.degree(n2) > 2) {
         if (bad_link_p) *bad_link_p=n2;
         return 0;
      }
   }

   return 1;
}

}

// return values: 1=true, 0=false, -1=undef (does not occur here)
template <typename Complex, typename VertexSet>
int is_ball_or_sphere(const Complex& C, const GenericSet<VertexSet>& V, int2type<1>)
{
   Graph<> G(V);
   // check graph for three properties
   // (1) connected
   // (2) degree(v)<3 all v in V
   // (3) #{v|degree(v)=1} =: n_leafs equals 0 or 2
   if (fill_graph(G,C,0)==0 || !graph::is_connected(G)) return 0;

   int n_leafs=0;
   for (typename Entire<VertexSet>::const_iterator v=entire(V.top()); !v.at_end(); ++v)
      if (G.degree(*v)==1) {
         if (++n_leafs > 2) return 0;
      }

   return (n_leafs!=1) ? 1 : 0;
}

// return values: 1=true, 0=false, -1=undef (does not occur here)
template <typename Complex, typename VertexSet>
int is_manifold(const Complex& C, const GenericSet<VertexSet>& V, int2type<1>, int *bad_link_p)
{
   Graph<> G(V);
   return fill_graph(G,C,bad_link_p);
}

} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
