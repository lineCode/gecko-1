//
// Copyright (c) 2014-2017 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include <iostream>
#include "checkbox.h"
#include "application.h"
#include <draw/path.h>
#include <draw/paint.h>
#include <draw/icons.h>

namespace gui
{

////////////////////////////////////////

checkbox_w::checkbox_w( void )
{
}

////////////////////////////////////////

checkbox_w::~checkbox_w( void )
{
}

////////////////////////////////////////

void checkbox_w::build( context &ctxt )
{
	const style &s = ctxt.get_style();

	draw::paint c;
	platform::context &hwc = ctxt.hw_context();
	gl::api &ogl = hwc.api();

	_unchecked.rebuild( hwc );
	_checked.rebuild( hwc );

	c.set_fill_color( s.active_icon( s.background_color() ) );
	_unchecked.add( ogl, draw::iconCheckBoxEmpty(), c );

	c.set_fill_color( s.dominant_color() );
	_checked.add( ogl, draw::iconCheckBoxChecked(), c );

	_unchecked.shape_size( 24, 24 );
	_checked.shape_size( 24, 24 );
	_unchecked.set_size( 24, 24 );
	_checked.set_size( 24, 24 );

	auto &l = layout_target();
	l->set_minimum( 24, 24 );
	l->set_maximum( 24, 24 );
}

////////////////////////////////////////

void checkbox_w::paint( context &ctxt )
{
	_checked.set_position( x(), y() );
	_unchecked.set_position( x(), y() );
	bool c = _state;
	if ( _tracking )
		c = _current;
	if ( c )
		_checked.draw( ctxt.hw_context() );
	else
		_unchecked.draw( ctxt.hw_context() );
}

////////////////////////////////////////

bool checkbox_w::mouse_press( const point &p, int button )
{
	unused( button );

	if ( contains( p ) )
	{
		_tracking = true;
		_current = !_state;
		invalidate();
	}
	return _tracking;
}

////////////////////////////////////////

bool checkbox_w::mouse_release( const point &p, int button )
{
	unused( button );

	if ( _tracking )
	{
		_tracking = false;
		if ( contains( p ) )
			set_state( _current );
		invalidate();
		return true;
	}
	return false;
}

////////////////////////////////////////

bool checkbox_w::mouse_move( const point &p )
{
	if ( _tracking )
	{
		if ( contains( p ) )
			_current = !_state;
		else
			_current = _state;
		invalidate();
		return true;
	}
	return false;
}

////////////////////////////////////////

void checkbox_w::set_state( bool s )
{
	if ( _state != s )
	{
		_state = s;
		when_toggled( _state );
	}
}
////////////////////////////////////////

}

