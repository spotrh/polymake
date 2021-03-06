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

#include "polymake/Matrix.h"
#include "polymake/Rational.h"
#include "polymake/IncidenceMatrix.h"

namespace polymake { namespace graph { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0>
   FunctionInterface4perl( hungarian_perfect_matching_X, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturn( (hungarian_perfect_matching(arg0.get<T0>())) );
   };

   FunctionInstance4perl(hungarian_perfect_matching_X, perl::Canned< const Matrix< Rational > >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
