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

#include "polymake/Rational.h"

namespace polymake { namespace polytope { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0>
   FunctionInterface4perl( compress_incidence_primal_T_x_f16, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturnVoid( (compress_incidence_primal<T0>(arg0)) );
   };

   template <typename T0>
   FunctionInterface4perl( compress_incidence_dual_T_x_f16, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturnVoid( (compress_incidence_dual<T0>(arg0)) );
   };

   FunctionInstance4perl(compress_incidence_primal_T_x_f16, Rational);
   FunctionInstance4perl(compress_incidence_primal_T_x_f16, double);
   FunctionInstance4perl(compress_incidence_dual_T_x_f16, Rational);
   FunctionInstance4perl(compress_incidence_dual_T_x_f16, double);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
