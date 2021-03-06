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
#include "polymake/Integer.h"
#include "polymake/Rational.h"
#include "polymake/Matrix.h"

namespace polymake { namespace polytope {
namespace {

inline
Integer round_down(const Rational& rat)
{
   const Integer truncated_rat(rat);
   return (truncated_rat!=rat && rat<0) ? truncated_rat-1 : truncated_rat;
}
    
inline
bool feasible(const Vector<Rational>& point, const Matrix<Rational>& Inequalities)
{
   for(Entire< Rows< Matrix<Rational> > >::const_iterator i=entire(rows(Inequalities)); !i.at_end(); ++i)
      if (point*(*i)<0) return false;
   return true;
}

inline 
bool feasible_after_update(Vector<Rational>& diff_vec, const Vector<Rational>& ineqs_col)
{
   diff_vec -= ineqs_col;  // update diff vector (to -b + Az_i)

   for (int i = 0; i < diff_vec.dim(); ++i) {
      if (diff_vec[i] < 0) return false; // infeasible
   }
      
   return true; // feasible
}

}

//Maximization with utility vector c=(1,...,1)
//Assumption: the LP is feasible and bounded in the direction of c
perl::ListReturn find_transitive_lp_sol(const Matrix<Rational>& Inequalities)
{
   int N_cols=Inequalities.cols();
   int dim=N_cols-1;
   Rational lower_bound=std::numeric_limits<Rational>::min();
   Rational upper_bound=std::numeric_limits<Rational>::max();
   bool max_bounded=true;
   bool feasible=true;
   Rational a=std::numeric_limits<Rational>::max();
   for(Entire< Rows< Matrix<Rational> > >::const_iterator i=entire(rows(Inequalities)); !i.at_end(); ++i){
      Rational sum=0;
      for(int j=1;j<N_cols;j++){ //add up all coefficients of one ineq (=project to fixed space spanned by (1,...,1))
         sum+=(*i)[j];
      }
      if(sum!=0){ //Facets with normalvectors orthogonal to (1,...,1) do not yield bounds
                  //therefore, we can neglect them!
         Rational constraint_val=-(*i)[0]/sum;
         Rational bound=dim*constraint_val;
         if(sum>0 && bound>lower_bound){ //new lower bound
            lower_bound=bound;
         } else if(sum<0 &&bound<upper_bound){ //new upper bound
            a=constraint_val;
            upper_bound=bound;
         }  
      }
   }
   if (upper_bound<lower_bound) {
      feasible=false;
   } else if(isinf(upper_bound)) {
      max_bounded=false;
   }
   Vector<Rational> optLPsolution(1|same_element_vector<Rational>(a,dim));
   Rational optLPvalue=upper_bound;
   perl::ListReturn result;
   result << optLPsolution
          << optLPvalue
          << feasible
          << max_bounded;
   return result;
}


perl::ListReturn core_point_algo(perl::Object p, const Rational optLPvalue, perl::OptionSet options)
{
   const Matrix<Rational> Inequalities=p.give("FACETS|INEQUALITIES");
   const int n=p.CallPolymakeMethod("AMBIENT_DIM");
   const Integer d = round_down(optLPvalue/n);
   const Integer r_start = round_down(optLPvalue) % n;

   const bool verbose = options["verbose"];

   if (verbose)
      cout << "dimension=" << n << ", "
           << "LP approximation=" << optLPvalue << ", "
           << "d=" << d << endl;

   int r=r_start.to_int();
   bool ILP_not_feasible(true);
   Vector<Rational> optILPsolution;
   Rational optILPvalue;
   while (r>=0 && ILP_not_feasible) {
      if (verbose)
         cout << "trying r=" << r << endl;
      const Vector<Rational> core_point(1|same_element_vector<Rational>(d+1,r)|same_element_vector<Rational>(d,n-r));
      if (feasible(core_point,Inequalities)) {
         optILPsolution = core_point;
         optILPvalue=(d+1)*r+d*(n-r);
         ILP_not_feasible=false;
      } else
         --r;
   }

   perl::ListReturn result;

   if (ILP_not_feasible) {
      if (verbose)
         cout << "ILP infeasible" << endl;
   } else {
      if (verbose)
         cout << "optimal ILP solution=" << optILPsolution << ", "
              << "value=" << optILPvalue << endl;
      result << optILPsolution
             << optILPvalue;
   }
    
   return result;
}




perl::ListReturn core_point_algo_Rote(perl::Object p, const Rational optLPvalue, perl::OptionSet options)
{
   const Matrix<Rational> Inequalities=p.give("FACETS|INEQUALITIES");
   const int n=p.CallPolymakeMethod("AMBIENT_DIM");
   const Integer d = round_down(optLPvalue/n);
   const Integer r_start = round_down(optLPvalue) % n;

   const bool verbose = options["verbose"];

   if (verbose)
      cout << "dimension=" << n << ", "
           << "LP approximation=" << optLPvalue << ", "
           << "d=" << d << endl;

   int r=r_start.to_int();
   bool ILP_not_feasible(true);
   Vector<Rational> optILPsolution;
   Rational optILPvalue;

   // initializing the difference vector b-A*current_core_point
   Vector<Rational> init_core_point(1|same_element_vector<Rational>(d+1,r)|same_element_vector<Rational>(d,n-r));
   Vector<Rational> diff_vec(Inequalities.rows());
   bool init_is_feasible = true;

   for ( int i = 0; i < Inequalities.rows(); ++i) {
      diff_vec[i] = init_core_point*Inequalities[i];
      if ( diff_vec[i] < 0 ) init_is_feasible = false;
   }
   // finished initialization

   if (init_is_feasible) {
      optILPsolution = init_core_point;
      optILPvalue=(d+1)*r+d*(n-r);
      ILP_not_feasible=false;
   } else { // initial core point was not feasible; test others via update of diff_vec
      --r;

      while (r>=0 && ILP_not_feasible) { 
         if (verbose)
            cout << "trying r=" << r << endl;
         
         if ( feasible_after_update(diff_vec, Inequalities.col(r+1)) ) { // do comparison via update of vector -b+Az, A=ineqs,z=core point
            const Vector<Rational> core_point(1|same_element_vector<Rational>(d+1,r)|same_element_vector<Rational>(d,n-r));
            optILPsolution = core_point;
            optILPvalue=(d+1)*r+d*(n-r);
            ILP_not_feasible=false;
         } else
            --r;
      }
   }

   perl::ListReturn result;

   if (ILP_not_feasible) {
      if (verbose)
         cout << "ILP infeasible" << endl;
   } else {
      if (verbose)
         cout << "optimal ILP solution=" << optILPsolution << ", "
              << "value=" << optILPvalue << endl;
      result << optILPsolution
             << optILPvalue;
   }
    
   return result;
}


UserFunction4perl("# @category Optimization"
                  "# Algorithm to solve highly symmetric integer linear programs (ILP)."
                  "# It is required that the group of the ILP induces the alternating or symmetric group"
                  "# on the set of coordinate directions."
                  "# The linear objective function is the vector (0,1,1,..,1)."
                  "# "
                  "# @param Polytope p"
                  "# @param Rational optLPvalue optimal value of LP approximation"
                  "# @option Bool verbose"
                  "# @return perl::ListReturn (optimal solution, optimal value) or empty",
                  &core_point_algo, "core_point_algo(Polytope, $; {verbose => undef})");

UserFunction4perl("# @category Optimization"
                  "# Version of core_point_algo with improved running time"
                  "# (according to a suggestion by G. Rote)."
                  "# The core_point_algo is an algorithm to solve highly symmetric integer linear programs (ILP)."
                  "# It is required that the group of the ILP induces the alternating or symmetric group"
                  "# on the set of coordinate directions."
                  "# The linear objective function is the vector (0,1,1,..,1)."
                  "# "
                  "# @param Polytope p"
                  "# @param Rational optLPvalue optimal value of LP approximation"
                  "# @option Bool verbose"
                  "# @return perl::ListReturn (optimal solution, optimal value) or empty",
                  &core_point_algo_Rote, "core_point_algo_Rote(Polytope, $; {verbose => undef})");

UserFunction4perl("# @category Optimization"
                  "# Algorithm to solve symmetric linear programs (LP) of the form"
                  "# max c<sup>t</sup>x , c=(0,1,1,..,1)"
                  "# subject to the inequality system given by //Inequalities//."
                  "# It is required that the symmetry group of the LP acts transitively"
                  "# on the coordinate directions."
                  "# "
                  "# @param Matrix Inequalities the inequalities describing the feasible region"
                  "# @return perl::ListReturn (optLPsolution,optLPvalue,feasible,max_bounded)",
                  &find_transitive_lp_sol, "find_transitive_lp_sol(Matrix)");
} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
