//
// Copyright (c) 2014 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include "widget.h"
#include "application.h"
#include <base/alignment.h>
#include <gl/color.h>
#include <draw/text.h>
#include <draw/rectangle.h>

namespace gui
{

////////////////////////////////////////

class label : public widget
{
public:
	label( void );
	label( std::string l, base::alignment a = base::alignment::LEFT );
	~label( void );

	void set_text( const std::string &utf8 ) { _text.set_text( utf8 ); }
	void set_color( const gl::color &c ) { _text.set_color( c ); }
	void set_font( const std::shared_ptr<script::font> &f ) { _text.set_font( f ); }
	void set_align( base::alignment a ) { _align = a; }
	void set_bg( const gl::color &c )
	{
		_bg_color = c;
		_bg.set_color( c );
	}

	void build( gl::api &ogl ) override;

	void paint( gl::api &ogl ) override;

	void compute_bounds( void ) override;

private:
	gl::color _bg_color;
	draw::rectangle _bg;
	draw::text _text;
	base::alignment _align = base::alignment::LEFT;
};

////////////////////////////////////////

}

