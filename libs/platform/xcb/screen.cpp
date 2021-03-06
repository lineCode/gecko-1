//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "screen.h"

#include <core/contract.h>
#include <string>
#include <stdexcept>
#include <xcb/xcb.h>

namespace platform { namespace xcb
{

////////////////////////////////////////

screen::screen( xcb_screen_t *scr )
	: _screen( scr )
{
	precondition( _screen, "null screen" );
}

////////////////////////////////////////

screen::~screen( void )
{
}

////////////////////////////////////////

draw::size screen::bounds( void )
{
	precondition( _screen, "null screen" );
	return { double(_screen->width_in_pixels), double(_screen->height_in_pixels) };
}

////////////////////////////////////////

} }

