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
#include "polymake/Matrix.h"
#include "polymake/IncidenceMatrix.h"

namespace polymake { namespace tropical { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0, typename T1, typename T2>
   FunctionInterface4perl( types_T_X_X, T0,T1,T2 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]);
      WrapperReturn( (types<T0>(arg0.get<T1>(), arg1.get<T2>())) );
   };

   template <typename T0, typename T1, typename T2>
   FunctionInterface4perl( coarse_types_T_X_X, T0,T1,T2 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]);
      WrapperReturn( (coarse_types<T0>(arg0.get<T1>(), arg1.get<T2>())) );
   };

   FunctionWrapper4perl( bool (pm::Array<pm::Set<int, pm::operations::cmp>, void>, pm::Set<int, pm::operations::cmp>, int, pm::Set<int, pm::operations::cmp>&) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]), arg3(stack[3]);
      IndirectWrapperReturn( arg0.get< perl::TryCanned< const Array< Set< int > > > >(), arg1.get< perl::TryCanned< const Set< int > > >(), arg2, arg3.get< perl::TryCanned< Set< int > > >() );
   }
   FunctionWrapperInstance4perl( bool (pm::Array<pm::Set<int, pm::operations::cmp>, void>, pm::Set<int, pm::operations::cmp>, int, pm::Set<int, pm::operations::cmp>&) );

   FunctionWrapper4perl( bool (pm::Array<pm::Set<int, pm::operations::cmp>, void>, pm::Set<int, pm::operations::cmp>, int, pm::Set<int, pm::operations::cmp>&, pm::Set<int, pm::operations::cmp>&) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]), arg3(stack[3]), arg4(stack[4]);
      IndirectWrapperReturn( arg0.get< perl::TryCanned< const Array< Set< int > > > >(), arg1.get< perl::TryCanned< const Set< int > > >(), arg2, arg3.get< perl::TryCanned< Set< int > > >(), arg4.get< perl::TryCanned< Set< int > > >() );
   }
   FunctionWrapperInstance4perl( bool (pm::Array<pm::Set<int, pm::operations::cmp>, void>, pm::Set<int, pm::operations::cmp>, int, pm::Set<int, pm::operations::cmp>&, pm::Set<int, pm::operations::cmp>&) );

   FunctionWrapper4perl( pm::Set<int, pm::operations::cmp> (pm::Array<pm::Set<int, pm::operations::cmp>, void>, pm::Set<int, pm::operations::cmp>, int) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      IndirectWrapperReturn( arg0.get< perl::TryCanned< const Array< Set< int > > > >(), arg1.get< perl::TryCanned< const Set< int > > >(), arg2 );
   }
   FunctionWrapperInstance4perl( pm::Set<int, pm::operations::cmp> (pm::Array<pm::Set<int, pm::operations::cmp>, void>, pm::Set<int, pm::operations::cmp>, int) );

   FunctionInstance4perl(types_T_X_X, Rational, perl::Canned< const Matrix< Rational > >, perl::Canned< const Matrix< Rational > >);
   FunctionInstance4perl(coarse_types_T_X_X, Rational, perl::Canned< const Matrix< Rational > >, perl::Canned< const Matrix< Rational > >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
