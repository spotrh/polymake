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
#include "polymake/IncidenceMatrix.h"
#include "polymake/Map.h"
#include "polymake/Set.h"
#include "polymake/Array.h"
#include "polymake/group/group_domain.h"
#include "polymake/group/permlib.h"

namespace polymake { namespace polytope {

      namespace{
         std::string determine_group_name (const std::string& g_in_name, const std::string& out_dom_string, const std::string& opt_name)
         {
            if ( opt_name.empty() ) {
               std::ostringstream oss;
               oss << g_in_name << out_dom_string;
               return oss.str();
            } else {
               return opt_name;
            }
         }
      }

perl::Object convert_group_domain(const perl::Object g_in, const IncidenceMatrix<>& VIF, perl::OptionSet options)
{
   perl::Object g_out(g_in.type());
   std::string name = options["name"];
   const int in_domain = g_in.give("DOMAIN");

   g_out.set_description() << "domain conversion of " << g_in.name() << " (" << g_in.description() << ")";
   g_out.set_name( determine_group_name(g_in.name(), 
                                        polymake::group::OnRays == in_domain ? "_f" : "_v", 
                                        name).c_str() );

   IncidenceMatrix<> I;
   switch (in_domain) {
   case polymake::group::OnFacets:
      g_out.take("DOMAIN") << polymake::group::OnRays;
      I = T(VIF);
      break;
   case polymake::group::OnRays:
      g_out.take("DOMAIN") << polymake::group::OnFacets;
      I = VIF;
      break;
   default:
      cerr << "The domain of the input group is " << in_domain << endl;
      throw std::runtime_error("Cannot handle this type of input domain.");
   }


   // initialize auxiliary data structures
   const int n_rows = I.rows();
   Array<Set<int> > row_sets(n_rows);
   Map<Set<int>, int> index_of;
   int ct(0);
   for (Entire<Rows<IncidenceMatrix<> > >::const_iterator rit = entire(rows(I)); !rit.at_end(); ++rit, ++ct) {
      const Set<int> row(*rit);
      index_of[row] = ct;
      row_sets[ct] = row;
   }
   
   const Array<Array<int> > gens_in = g_in.give("GENERATORS");
   Array<Array<int> > gens_out(gens_in.size());
   for (int i=0; i<gens_in.size(); ++i) {
      // for each input generator ...
      Array<int> gen_out(n_rows);
      for (int j=0; j<n_rows; ++j) {
         // for each row of the incidence matrix, regarded as an index set ...
         Set<int> image;
         for (Entire<Set<int> >::const_iterator sit = entire(row_sets[j]); !sit.at_end(); ++sit)
            // find the image of the row under the input generator
            image += gens_in[i][*sit];
         // ... and store the index of that image in a new output generator
         gen_out[j] = index_of[image];
      }
      gens_out[i] = gen_out;
   }

   g_out.take("GENERATORS") << gens_out;
   return g_out;
}


    /* convert action on coords to action on rows of a matrix */
   template <typename MatrixTop, typename Scalar>
   perl::Object convert_coord_action(perl::Object g_in, const GenericMatrix<MatrixTop, Scalar>& mat, const int out_dom, perl::OptionSet options){
      using namespace group;

      std::string name = options["name"];
      const int in_domain = g_in.give("DOMAIN");
      if (in_domain != polymake::group::OnCoords)
        throw std::runtime_error("convert_coord_action: group does not act on coordinates!");
      
      perl::Object g_out(g_in.type());
      g_out.set_description() << "domain conversion of " << g_in.name() << " (" << g_in.description() << ")";
      g_out.set_name( determine_group_name(g_in.name(), 
                                           polymake::group::OnRays == out_dom ? "_v" : "_f", 
                                           name).c_str() );

      if (out_dom == 1) {
         g_out.take("DOMAIN") << group::OnRays;
      } else {
         if (out_dom == 2) {
         g_out.take("DOMAIN") << group::OnFacets;
         }
         else {
            throw std::runtime_error("convert_coord_action: the domain of the output group must be OnRays(1) or OnFacets(2)!"); 
         }
      }


      PermlibGroup group_of_cone = group_from_perlgroup(g_in);      
      int deg = group_of_cone.degree();
      if (mat.cols() <= deg)
         throw std::runtime_error("convert_coord_action: group/matrix dimension mismatch: group degree greater than #(number of matrix columns)-1");

      hash_map<Vector<Scalar>, int> mat_rows; //to look up the index in mat belonging to the image of vec
      typename std::list< Vector<Scalar> > vectors;
      int i=0;
      for (typename Entire< Rows<MatrixTop> >::const_iterator vec=entire(rows(mat.top())); !vec.at_end(); ++vec){
         vectors.push_back(*vec);
         //mat_rows[*vec]=vec->index();
         mat_rows[*vec]=i;
         ++i;
      }


      const Array<Array<int> > gens_in = g_in.give("GENERATORS");
      Array<Array<int> > gens_out(gens_in.size());
      
      for (int k = 0; k < gens_in.size(); ++k) {
         // for each input generator ...
         Array<int> gen_out(mat.rows());
         for (int i = 0; i < mat.rows(); ++i) {
            boost::shared_ptr<permlib::Permutation> gen(new permlib::Permutation((gens_in[k]).begin(),(gens_in[k]).end()));
            CoordinateAction< permlib::Permutation , Scalar > action;
            Vector<Scalar> image = action(*gen,mat.row(i));
            //cerr << "k="<<k<<", i="<<i<<", row="<<mat.row(i)<<", image="<<image << endl; //only for checking
            gen_out[i] = mat_rows[image];
         }
         gens_out[k] = gen_out;
      }

      g_out.take("GENERATORS") << gens_out;
    
      return g_out;
   }


UserFunctionTemplate4perl("# @category Symmetry"
                          "# Converts the generators of the input group from the domain onRays "
                          "# to generators on the domain onFacets, and vice versa. "
                          "# @param group::Group group"
                          "# @param IncidenceMatrix VIF the vertex-facet incidence matrix of the cone or polytope"
                          "# @option String name an optional name for the output group"
                          "# @return group::Group a new group object with the generators induced on the new domain",
                          "convert_group_domain(group::Group, IncidenceMatrix, {name=>''})");

UserFunctionTemplate4perl("# @category Symmetry"
                          "# Converts the generators of a group acting on coordinates to generators "
                          "# of the corresponding group which acts on the rows of the given matrix //mat//. "
                          "# The parameter //dom_out// specifies whether //mat// describes vertices or facets."
                          "# @param group::Group group input group acting on coordinates"
                          "# @param Matrix mat vertices or facets of a polytope"
                          "# @param Int dom_out OnRays(1) or OnFacets(2)"
                          "# @option String name an optional name for the output group"
                          "# @return group::Group a new group object with the generators induced on the new domain",
                          "convert_coord_action(group::Group, Matrix, $, {name=>''})");

} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:

