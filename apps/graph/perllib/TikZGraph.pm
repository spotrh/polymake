#  Copyright (c) 1997-2015
#  Ewgenij Gawrilow, Michael Joswig (Technische Universitaet Berlin, Germany)
#  http://www.polymake.org
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the
#  Free Software Foundation; either version 2, or (at your option) any
#  later version: http://www.gnu.org/licenses/gpl.txt.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#-------------------------------------------------------------------------------

package TikZ::Lattice;
use Polymake::Struct (
   [ '@ISA' => 'Wire' ],
);

sub new {
   my $self=&_new;
   my $Graph=$self->source;
   my @label_width=map {
      (length($Graph->NodeLabels->($_))+1)*0.8;
   } 0..$Graph->n_nodes-1;

   my $embedding=$Graph->Coord;
   if (is_object($embedding)) {
      # expecting Visual::GraphEmbedding here
      $embedding->label_width=\@label_width;
      $embedding->options->{dual}= $Graph->Mode eq "dual";
   }

   $self;
}

sub drawPoint {
    my ($self,$i) = @_;
    my $id=$self->id;

#points stuff
    my $point_style=$self->source->VertexStyle;
    my $point_flag= is_code($point_style) || $point_style !~ $Visual::hidden_re ? "show" : "hide";
    my $point_color=$self->source->VertexColor;
    my $point_color_flag= is_code($point_color) ? "show" : "hide";
    my $point_labels=$self->source->VertexLabels;
#---
    my $point_thickness=$self->source->VertexThickness;
    my $point_thickness_flag= is_code($point_thickness) ? "show" : "hide";

    my $text;
    my $option_string = ($point_color_flag eq "show") ? "pointcolor\_$id"."\_$i" : "pointcolor\_$id";
    my $pthick = ($point_thickness_flag eq "show") ? $point_thickness->($i) : $point_thickness;
    $text .= "  \\node[text=black, inner sep=3pt,draw=black,fill=$option_string,rectangle,rounded corners=3pt] at (v$i\_$id) {".$point_labels->($i).'};'."\n";

    return $text;
}

1


