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

///==== this line controls the automatic file splitting: max.instances=60

#include "polymake/client.h"
#include "polymake/Rational.h"
#include "polymake/linalg.h"
#include "polymake/Vector.h"
#include "polymake/Integer.h"
#include "polymake/SparseMatrix.h"
#include "polymake/IncidenceMatrix.h"
#include "polymake/Polynomial.h"
#include "polymake/QuadraticExtension.h"
#include "polymake/RationalFunction.h"

namespace polymake { namespace common { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0, typename T1, typename T2>
   FunctionInterface4perl( new_X_X, T0,T1,T2 ) {
      perl::Value arg0(stack[1]), arg1(stack[2]);
      WrapperReturnNew(T0, (arg0.get<T1>(), arg1.get<T2>()) );
   };

   FunctionInstance4perl(new_X_X, Rational, perl::Canned< const Integer >, perl::Canned< const Integer >);
   FunctionInstance4perl(new_X_X, Rational, int, perl::Canned< const Integer >);
   FunctionInstance4perl(new_X_X, Rational, perl::Canned< const Integer >, int);
   OperatorInstance4perl(Binary__ne, perl::Canned< const Rational >, perl::Canned< const Rational >);
   OperatorInstance4perl(Binary_add, perl::Canned< const Rational >, perl::Canned< const UniPolynomial< Rational, Rational > >);
   OperatorInstance4perl(Unary_not, perl::Canned< const Rational >);
   OperatorInstance4perl(Binary_add, perl::Canned< const Rational >, perl::Canned< const QuadraticExtension< Rational > >);
   OperatorInstance4perl(Binary_sub, perl::Canned< const Rational >, perl::Canned< const QuadraticExtension< Rational > >);
   OperatorInstance4perl(Binary_mul, perl::Canned< const Rational >, perl::Canned< const QuadraticExtension< Rational > >);
   OperatorInstance4perl(Binary_div, perl::Canned< const Rational >, perl::Canned< const QuadraticExtension< Rational > >);
   OperatorInstance4perl(Binary__le, perl::Canned< const Rational >, perl::Canned< const Rational >);
   OperatorInstance4perl(Binary_mul, perl::Canned< const Rational >, perl::Canned< const UniMonomial< Rational, int > >);
   OperatorInstance4perl(Binary_add, perl::Canned< const Rational >, perl::Canned< const UniMonomial< Rational, int > >);
   OperatorInstance4perl(Binary_sub, perl::Canned< const Rational >, perl::Canned< const UniMonomial< Rational, int > >);
   OperatorInstance4perl(Binary_mul, perl::Canned< const Rational >, perl::Canned< const UniTerm< Rational, int > >);
   OperatorInstance4perl(Binary_add, perl::Canned< const Rational >, perl::Canned< const UniTerm< Rational, int > >);
   OperatorInstance4perl(Binary_sub, perl::Canned< const Rational >, perl::Canned< const UniTerm< Rational, int > >);
   OperatorInstance4perl(Binary_mul, perl::Canned< const Rational >, perl::Canned< const UniPolynomial< Rational, int > >);
   OperatorInstance4perl(Binary_add, perl::Canned< const Rational >, perl::Canned< const UniPolynomial< Rational, int > >);
   OperatorInstance4perl(Binary_sub, perl::Canned< const Rational >, perl::Canned< const UniPolynomial< Rational, int > >);
   OperatorInstance4perl(Binary_div, perl::Canned< const Rational >, perl::Canned< const UniMonomial< Rational, int > >);
   OperatorInstance4perl(Binary_div, perl::Canned< const Rational >, perl::Canned< const UniTerm< Rational, int > >);
   OperatorInstance4perl(Binary_div, perl::Canned< const Rational >, perl::Canned< const UniPolynomial< Rational, int > >);
   OperatorInstance4perl(assign, Rational, perl::Canned< const pm::sparse_elem_proxy<pm::sparse_proxy_base<pm::sparse2d::line<pm::AVL::tree<pm::sparse2d::traits<pm::sparse2d::traits_base<pm::Rational, true, false, (pm::sparse2d::restriction_kind)0>, false, (pm::sparse2d::restriction_kind)0> > >, pm::unary_transform_iterator<pm::AVL::tree_iterator<pm::sparse2d::it_traits<pm::Rational, true, false>, (pm::AVL::link_index)1>, std::pair<pm::BuildUnary<pm::sparse2d::cell_accessor>, pm::BuildUnaryIt<pm::sparse2d::cell_index_accessor> > > >, pm::Rational, pm::NonSymmetric> >);
   OperatorInstance4perl(Binary__ne, perl::Canned< const Rational >, perl::Canned< const Integer >);
   OperatorInstance4perl(Binary_mul, perl::Canned< const Rational >, perl::Canned< const pm::SameElementVector<pm::Rational const&> >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
