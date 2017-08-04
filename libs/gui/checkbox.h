//
// Copyright (c) 2017 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "widget.h"
#include <base/signal.h>
#include <draw/shape.h>

namespace gui
{

////////////////////////////////////////

class checkbox : public widget
{
public:
	checkbox( void );
	~checkbox( void );

	void build( gl::api &ogl ) override;

	void paint( gl::api &ogl ) override;

	void compute_bounds( void ) override;

	bool mouse_press( const base::point &p, int button ) override;
	bool mouse_release( const base::point &p, int button ) override;
	bool mouse_move( const base::point &p ) override;

	bool is_checked( void ) const
	{
		return _state;
	}

	void set_state( bool s );

	base::signal<void(bool)> when_toggled;

public:
	draw::shape _checked;
	draw::shape _unchecked;

	bool _state = false;
	bool _tracking = false;
	bool _current = false;
};

////////////////////////////////////////

}

