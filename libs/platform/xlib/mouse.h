//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <platform/mouse.h>

namespace platform { namespace xlib
{

////////////////////////////////////////

/// @brief Xlib implementation of platform::mouse.
class mouse : public ::platform::mouse
{
public:
	/// @brief Constructor.
	mouse( ::platform::system *s );

	/// @brief Destructor.
	~mouse( void );

	void start( void ) override;
	void cancel( void ) override;
	void shutdown( void ) override;
	wait poll_object( void ) override;
	bool poll_timeout( duration &when, const time_point &curtime ) override;
	void emit( const time_point &curtime ) override;
};

////////////////////////////////////////

} }

