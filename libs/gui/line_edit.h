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
	line_edit( std::string l );
	~line_edit( void );

	const std::string &text( void ) const { return _text.get_text(); }
	void set_text( const std::string &t ) { _text.set_text( t ); }
	void set_prompt( const std::string &p ) { _prompt.set_text( p ); }

	void set_font( std::shared_ptr<script::font> &f ) { _text.set_font( f ); _prompt.set_font( f ); }

	void build( gl::api &ogl ) override;
	void paint( gl::api &ogl ) override;

	void compute_bounds( void ) override;

	bool key_press( platform::scancode c ) override;

	bool text_input( char32_t c ) override;

private:
	draw::rectangle _line;
	draw::rectangle _marker;
	draw::text _text;
	draw::text _prompt;
	size_t _cursor = 0;
};

////////////////////////////////////////

}

