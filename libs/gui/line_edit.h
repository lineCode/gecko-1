//
// Copyright (c) 2014 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include "widget.h"
#include "application.h"
#include <draw/text.h>
#include <draw/rectangle.h>

namespace gui
{

////////////////////////////////////////

class line_edit : public widget
{
public:
	line_edit( void );
	line_edit( std::string l, const gl::color &c = gl::white, const std::shared_ptr<script::font> &f = application::current()->get_default_font() );
	~line_edit( void );

	const std::string &text( void ) const { return _text.get_text(); }
	void set_text( const std::string &t ) { _text.set_text( t ); }

	void set_font( std::shared_ptr<script::font> &f ) { _text.set_font( f ); }

	void paint( gl::api &ogl ) override;

	void compute_bounds( void ) override;

	bool key_press( platform::scancode c ) override;

	bool text_input( char32_t c ) override;

private:
	draw::rectangle _line;
	draw::rectangle _marker;
	draw::text _text;
	size_t _cursor = 0;
};

////////////////////////////////////////

}

