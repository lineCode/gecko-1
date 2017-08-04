//
// Copyright (c) 2014-2017 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include <iostream>
#include "radio_button.h"
#include "application.h"
#include <draw/path.h>
#include <draw/paint.h>
#include <draw/icons.h>

namespace gui
{

////////////////////////////////////////

radio_button::radio_button( void )
{
}

////////////////////////////////////////

radio_button::~radio_button( void )
{
}

////////////////////////////////////////

void radio_button::build( gl::api &ogl )
{
	draw::paint c;
	c.set_fill_color( { 0.26, 0.26, 0.26 } );
	_unchecked.add( ogl, draw::iconRadioButtonEmpty(), c );
	_checked.add( ogl, draw::iconRadioButtonChecked(), c );

	_unchecked.shape_size( 24, 24 );
	_checked.shape_size( 24, 24 );
	_unchecked.set_size( 24, 24 );
	_checked.set_size( 24, 24 );
}

////////////////////////////////////////

void radio_button::paint( gl::api &ogl )
{
	_checked.set_position( x(), y() );
	_unchecked.set_position( x(), y() );
	bool c = _state;
	if ( _tracking )
		c = _current;
	if ( c )
		_checked.draw( ogl );
	else
		_unchecked.draw( ogl );
}

////////////////////////////////////////

void radio_button::compute_bounds( void )
{
	set_minimum( 24, 24 );
	set_maximum( 24, 24 );
}

////////////////////////////////////////

bool radio_button::mouse_press( const base::point &p, int button )
{
	unused( button );

	if ( contains( p ) )
	{
		_tracking = true;
		_current = true;
		invalidate();
	}
	return _tracking;
}

////////////////////////////////////////

bool radio_button::mouse_release( const base::point &p, int button )
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

bool radio_button::mouse_move( const base::point &p )
{
	if ( _tracking )
	{
		if ( contains( p ) )
			_current = true;
		else
			_current = _state;
		invalidate();
		return true;
	}
	return false;
}

////////////////////////////////////////

void radio_button::set_state( bool s )
{
	if ( _state != s )
	{
		_state = s;
		if ( _state )
			when_selected( _state );
	}
}

////////////////////////////////////////

}

