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
#include "polymake/Array.h"
#include "polymake/Rational.h"
#include <fstream>
#include "polymake/common/print_constraints.h"

namespace polymake { namespace polytope {


template <typename Scalar>
void print_constraints(perl::Object P, perl::OptionSet options){
   const bool ispoly = P.isa("Polytope");
   Matrix<Scalar> Ineqs = P.give("FACETS|INEQUALITIES");
   Matrix<Scalar> Eqs = P.give("LINEAR_SPAN|EQUATIONS");
   Array<std::string> coord_labels;
   if ( P.exists("COORDINATE_LABELS") ) {
      coord_labels=P.give("COORDINATE_LABELS");
   }
   if (Ineqs.rows() > 0){
      cout << (P.exists("FACETS") ? "Facets:" : "Inequalities:") << endl;
      common::print_constraints_sub(Ineqs, coord_labels, options["ineq_labels"], 0, !ispoly);
   }
   if (Eqs.rows() > 0){
      cout << (P.exists("LINEAR_SPAN") ? (ispoly ? "Affine hull:" : "Linear span:") : "Equations:") << endl;
      common::print_constraints_sub(Eqs, coord_labels, options["eq_labels"], 1, !ispoly);
   }
}



UserFunctionTemplate4perl("# @category Optimization"
           "# Write the [[FACETS]] / [[INEQUALITIES]] and the [[LINEAR_SPAN]] / [[EQUATIONS]]"
           "# of a polytope //P// or cone //C// in a readable way."
           "# [[COORDINATE_LABELS]] are adopted if present."
           "# @param Cone<Scalar> C the given polytope or cone"
			  "# @option Array<String> ineq_labels changes the labels of the inequality rows"
			  "# @option Array<String> eq_labels changes the labels of the equation rows"
           "# @return Bool",
           "print_constraints<Scalar>(Cone<Scalar>; { ineq_labels => undef, eq_labels => undef }) : void");



}}
