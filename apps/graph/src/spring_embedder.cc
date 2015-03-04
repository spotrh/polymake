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
#include "polymake/graph/SpringEmbedder.h"

namespace polymake { namespace graph {

Matrix<double> spring_embedder(const Graph<>& G, perl::OptionSet options)
{
   SpringEmbedder SE(G,options);

   const RandomSeed seed(options["seed"]);
#if POLYMAKE_DEBUG
   if (SE.debug_print_enabled())
      cout << "initial random seed=" << seed.get() << endl;
#endif
   RandomSpherePoints<double> random_points(3, seed);
   Matrix<double> X(G.nodes(),3);
   SE.start_points(X,random_points.begin());

   int max_iter;
   if (!(options["max-iterations"] >> max_iter)) max_iter=10000;
   if (! SE.calculate(X,random_points,max_iter))
      cerr << "WARNING: spring_embedder not converged after " << max_iter << " iterations" << endl;
   return X;
}

UserFunction4perl("# @category Visualization"
                  "# Produce a 3-d embedding for the graph using the spring embedding algorithm"
                  "# along the lines of"
                  "#\t Thomas Fruchtermann and Edward Reingold:"
                  "#\t Graph Drawing by Force-directed Placement."
                  "#\t Software Practice and Experience Vol. 21, 1129-1164 (1992), no. 11."

                  "# @param props::Graph<Undirected> graph to be embedded."

                  "# @options affecting the desired picture"

                  "# @option EdgeMap edge_weights relative edge lengths."
                  "#  By default the embedding algorithm tries to stretch all edges to the same length."
                  "# @option Vector z-ordering an objective function provides an additional force along the z-axis,"
                  "#  trying to rearrange nodes in the order of the function growth."
                  "# @option Float z-factor gain coefficient applied to the //z-ordering// force."
                  "# @option Int seed random seed for initial node placement on a unit sphere."

                  "# @options calculation fine-tuning"

                  "# @option Float scale enlarges the ideal edge length"
                  "# @option Float balance changes the balance between the edge contraction and node repulsion forces"
                  "# @option Float inertion affects how the nodes are moved, can be used to restrain oscillations"
                  "# @option Float viscosity idem"
                  "# @option Float eps a threshold for point movement between iterations, below that it is considered to stand still"
                  "# @option Int max-iterations hard limit for computational efforts."
                  "#  The algorithm terminates at latest after that many iterations regardless of the convergence achieved so far.",

                  &spring_embedder,

                  // the next string is *one* long line
                  "spring_embedder(props::Graph<Undirected>, "
                  "   { scale => 1, balance => 1, viscosity => 1, inertion => 1, eps => undef,"
                  "     'z-ordering' => undef, 'z-factor' => undef, 'edge-weights' => undef,"
                  "      seed => undef, 'max-iterations' => 10000 }) ");
} }

// Local Variables:
// c-basic-offset:3
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
