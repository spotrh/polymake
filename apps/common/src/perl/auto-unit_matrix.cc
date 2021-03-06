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

///==== this line controls the automatic file splitting: max.instances=40

#include "polymake/Integer.h"
#include "polymake/QuadraticExtension.h"
#include "polymake/Rational.h"
#include "polymake/RationalFunction.h"
#include "polymake/TropicalNumber.h"
#include "polymake/client.h"
#include "polymake/linalg.h"

namespace polymake { namespace common { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0>
   FunctionInterface4perl( unit_matrix_T_x, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturn( (unit_matrix<T0>(arg0)) );
   };

   FunctionInstance4perl(unit_matrix_T_x, Rational);
   FunctionInstance4perl(unit_matrix_T_x, RationalFunction< Rational, int >);
   FunctionInstance4perl(unit_matrix_T_x, double);
   FunctionInstance4perl(unit_matrix_T_x, int);
   FunctionInstance4perl(unit_matrix_T_x, Integer);
   FunctionInstance4perl(unit_matrix_T_x, TropicalNumber< Min, Rational >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
