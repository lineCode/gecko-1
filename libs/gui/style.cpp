//
// Copyright (c) 2017 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "style.h"

namespace gui
{

////////////////////////////////////////

void style::set_font_manager( const std::shared_ptr<script::font_manager> &fmgr )
{
	_fmgr = fmgr;

	// sizes should be in points (1/72 of an inch)
	_display = _fmgr->get_font( "Arial", "Bold", 24 );
	_title = _fmgr->get_font( "Arial", "Bold", 16 );
	_body = _fmgr->get_font( "Arial", "Bold", 12 );
	_caption = _fmgr->get_font( "Arial", "Bold", 12 );
}

////////////////////////////////////////

gl::color style::primary_text( gl::color bg ) const
{
	float w = bg.distance( gl::white );
	float b = bg.distance( gl::black );
	if ( w > b )
		return gl::color( 1.F, 1.F, 1.F, 1.F );
	else
		return gl::color( 0.F, 0.F, 0.F, 0.87F );
}

////////////////////////////////////////

gl::color style::secondary_text( gl::color bg ) const
{
	float w = bg.distance( gl::white );
	float b = bg.distance( gl::black );
	if ( w > b )
		return gl::color( 1.F, 1.F, 1.F, 0.70F );
	else
		return gl::color( 0.F, 0.F, 0.F, 0.54F );
}

////////////////////////////////////////

gl::color style::disabled_text( gl::color bg ) const
{
	float w = bg.distance( gl::white );
	float b = bg.distance( gl::black );
	if ( w > b )
		return gl::color( 1, 1, 1, 0.50F );
	else
		return gl::color( 0, 0, 0, 0.38F );
}

////////////////////////////////////////

gl::color style::divider( gl::color bg ) const
{
	float w = bg.distance( gl::white );
	float b = bg.distance( gl::black );
	if ( w > b )
		return gl::color( 1, 1, 1, 0.12F );
	else
		return gl::color( 0, 0, 0, 0.12F );
}

////////////////////////////////////////

gl::color style::active_icon( gl::color bg ) const
{
	float w = bg.distance( gl::white );
	float b = bg.distance( gl::black );
	if ( w > b )
		return gl::color( 1, 1, 1, 1.00F );
	else
		return gl::color( 0, 0, 0, 0.54F );
}

////////////////////////////////////////

gl::color style::inactive_icon( gl::color bg ) const
{
	float w = bg.distance( gl::white );
	float b = bg.distance( gl::black );
	if ( w > b )
		return gl::color( 1, 1, 1, 0.50F );
	else
		return gl::color( 0, 0, 0, 0.38F );
}

////////////////////////////////////////

}
