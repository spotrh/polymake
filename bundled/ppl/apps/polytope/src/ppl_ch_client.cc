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
#include "polymake/polytope/ppl_interface.h"

namespace polymake { namespace polytope {
    
template <typename Scalar>
void ppl_ch_primal(perl::Object p)
{
   ppl_interface::solver<Scalar> solver;
   ch_primal(p, solver);
}
    
    
template <typename Scalar>
void ppl_ch_dual(perl::Object p)
{
   ppl_interface::solver<Scalar> solver;
   ch_dual(p, solver);
}

FunctionTemplate4perl("ppl_ch_primal<Scalar> (Cone<Scalar>) : void");
FunctionTemplate4perl("ppl_ch_dual<Scalar> (Cone<Scalar>) : void");

} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
