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

#include "polymake/client.h"
#include "polymake/Graph.h"

namespace polymake { namespace common { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0>
   FunctionInterface4perl( adjacency_matrix_X12_f4, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturnLvalueAnch( 1, (arg0), T0, (adjacency_matrix(arg0.get<T0>())) );
   };

   FunctionInstance4perl(adjacency_matrix_X12_f4, perl::Canned< Graph< Undirected > >);
   FunctionInstance4perl(adjacency_matrix_X12_f4, perl::Canned< const Graph< Directed > >);
   FunctionInstance4perl(adjacency_matrix_X12_f4, perl::Canned< const Graph< Undirected > >);
   FunctionInstance4perl(adjacency_matrix_X12_f4, perl::Canned< Graph< Directed > >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
