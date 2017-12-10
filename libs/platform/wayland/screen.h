//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <platform/screen.h>
#include <memory>

namespace platform { namespace wayland
{

////////////////////////////////////////

/// @brief X11 implementation of platform::screen
class screen : public platform::screen
{
public:
	/// @brief Constructor.
	screen( void );
	~screen( void );

	bool is_default( void ) const override;

	double refresh_rate( void ) const override;

	rect bounds( bool active ) const override;

	base::dsize dpi( void ) const override;

private:
};

////////////////////////////////////////

} }

