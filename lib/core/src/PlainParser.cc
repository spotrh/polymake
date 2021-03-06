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

#include "polymake/internal/CharBuffer.h"
#include "polymake/GenericIO.h"
#include "polymake/Rational.h"
#include "polymake/socketstream.h"
#include <cstdlib>
#include <cxxabi.h>

namespace pm {

int PlainParserCommon::count_words()
{
   std::streambuf *mybuf=is->rdbuf();
   if (CharBuffer::skip_ws(mybuf)<0) return 0;
   int cnt=0, offset=0;
   do {
      offset=CharBuffer::next_ws(mybuf,offset+1);
      ++cnt;
   } while (offset>0  &&
            CharBuffer::seek_forward(mybuf,offset) != '\n'  && 
            (offset=CharBuffer::next_non_ws(mybuf,offset+1)) > 0);
   return cnt;
}

int PlainParserCommon::count_lines()
{
   return CharBuffer::count_lines(is->rdbuf());
}

int PlainParserCommon::count_all_lines()
{
   return dynamic_cast<streambuf_with_input_width*>(is->rdbuf())->lines();
}

int PlainParserCommon::count_braced(char opening, char closing)
{
   std::streambuf *mybuf=is->rdbuf();
   if (CharBuffer::skip_ws(mybuf)<0) return 0;
   int cnt=0, offset=0;
   do {
      if (CharBuffer::get_ptr(mybuf)[offset] != opening) {
         is->setstate(is->failbit);
         return 0;
      }
      offset=CharBuffer::matching_brace(mybuf, opening, closing, offset+1);
      if (offset<0) {
         is->setstate(is->failbit);
         return 0;
      }
      ++cnt;
   } while ((offset=CharBuffer::next_non_ws(mybuf,offset+1)) > 0);
   return cnt;
}

int PlainParserCommon::count_leading(char c)
{
   std::streambuf *mybuf=is->rdbuf();
   int cnt=0, offset=-1;
   for (;;) {
      if ((offset=CharBuffer::next_non_ws(mybuf,offset+1)) < 0)
         return -1;
      if (CharBuffer::get_ptr(mybuf)[offset] != c) break;
      ++cnt;
   }
   return cnt;
}

bool PlainParserCommon::at_end()
{
   return CharBuffer::skip_ws(is->rdbuf())<0;
}

void PlainParserCommon::skip_item()
{
   std::streambuf *mybuf=is->rdbuf();
   if (CharBuffer::skip_ws(mybuf)<0) return;

   int offset=0;
   switch (mybuf->sbumpc()) {
   case '<':
      offset=CharBuffer::matching_brace(mybuf, '<', '>');  break;
   case '(':
      offset=CharBuffer::matching_brace(mybuf, '(', ')');  break;
   case '{':
      offset=CharBuffer::matching_brace(mybuf, '{', '}');  break;
   default:
      offset=CharBuffer::next_ws(mybuf,0,false);
   }
   if (offset<0)
      CharBuffer::skip_all(mybuf);
   else
      CharBuffer::get_bump(mybuf,offset+1);
}

void PlainParserCommon::skip_rest()
{
   std::streambuf *mybuf=is->rdbuf();
   CharBuffer::skip_all(mybuf);
}

char* PlainParserCommon::set_temp_range(char opening, char closing)
{
   std::streambuf *mybuf=is->rdbuf();

   if (CharBuffer::skip_ws(mybuf)<0) {
      is->setstate(closing == '\n' ? is->eofbit : is->failbit | is->eofbit);
      return 0;
   }

   int offset;
   if (closing == '\n') {
      offset=CharBuffer::find_char_forward(mybuf,'\n');
      if (offset<0) return 0;
      ++offset;
   } else {
      if (*CharBuffer::get_ptr(mybuf) != opening) {
         is->setstate(is->failbit);
         return 0;
      }
      CharBuffer::get_bump(mybuf,1);
      offset=CharBuffer::matching_brace(mybuf, opening, closing);
      if (offset<0) {
         is->setstate(is->failbit);
         return 0;
      }
   }

   return set_input_range(offset);
}

char* PlainParserCommon::set_input_range(int offset)
{
   streambuf_with_input_width *mybuf=static_cast<streambuf_with_input_width*>(is->rdbuf());
   char *egptr=CharBuffer::end_get_ptr(mybuf);
   if (CharBuffer::get_input_limit(mybuf)) {
      CharBuffer::set_end_get_ptr(mybuf, CharBuffer::get_ptr(mybuf)+offset);
   } else {
      mybuf->set_input_width(offset);
   }
   return egptr;
}

void PlainParserCommon::restore_input_range(char *egptr)
{
   streambuf_with_input_width *mybuf=static_cast<streambuf_with_input_width*>(is->rdbuf());
   if (egptr==CharBuffer::get_input_limit(mybuf)) {
      mybuf->reset_input_width(false);
   } else {
      CharBuffer::set_end_get_ptr(mybuf, egptr);
   }
}

void PlainParserCommon::discard_range(char closing)
{
   std::streambuf *mybuf=is->rdbuf();
   if (is->eof())
      is->clear();
   else if (CharBuffer::skip_ws(mybuf) >= 0 ||
            CharBuffer::get_ptr(mybuf) != CharBuffer::end_get_ptr(mybuf))
      is->setstate(is->failbit);
   if (is->good() && closing != '\n')
      CharBuffer::get_bump(mybuf,1);
}

void PlainParserCommon::skip_temp_range(char *egptr)
{
   streambuf_with_input_width *mybuf=static_cast<streambuf_with_input_width*>(is->rdbuf());
   char *next=CharBuffer::end_get_ptr(mybuf)+1;
   if (egptr==CharBuffer::get_input_limit(mybuf)) {
      mybuf->reset_input_width(false);
      CharBuffer::get_bump(mybuf, next-CharBuffer::get_ptr(mybuf));
   } else {
      CharBuffer::set_get_and_end_ptr(mybuf, next, egptr);
   }
}

char* PlainParserCommon::save_read_pos()
{
   return CharBuffer::get_ptr(is->rdbuf());
}

void PlainParserCommon::restore_read_pos(char *pos)
{
   streambuf_with_input_width *mybuf=static_cast<streambuf_with_input_width*>(is->rdbuf());
   mybuf->rewind(CharBuffer::get_ptr(mybuf)-pos);
}

void PlainParserCommon::get_scalar(Rational& x)
{
   static std::string text;
   if (*is >> text) {
      if (text.find_first_of("eE") != std::string::npos) {
         char *end;
         x=strtod(text.c_str(),&end);
         if (*end) is->setstate(is->failbit);
      } else {
         x.set(text.c_str());
      }
   }
}

void PlainParserCommon::get_scalar(double& x)
{
   static std::string text;
   if (*is >> text) {
      if (text.find('/') != std::string::npos) {
         x=Rational(text.c_str()).to_double();
      } else {
         char *end;
         x=strtod(text.c_str(),&end);
         if (*end) is->setstate(is->failbit);
      }
   }
}

void PlainParserCommon::get_string (std::string& s, char delim)
{
   if (CharBuffer::get_string(is->rdbuf(),s,delim) < 0)
      is->setstate(is->eofbit | is->failbit);
}

PlainPrinter<> cout(std::cout);
PlainPrinter<> cerr(std::cerr);

void complain_no_serialization(const char* text, const std::type_info& ti)
{
   std::string errmsg(text);
   int status;
   char* type_name=abi::__cxa_demangle(ti.name(), NULL, NULL, &status);
   if (status==0) {
      errmsg += type_name;
      free(type_name);
   } else {
      errmsg += ti.name();
   }
   throw std::invalid_argument(errmsg);
}

} // end namespace pm

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
