//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <memory>
#include <map>
#include <atomic>
#include <platform/dispatcher.h>
#include "window.h"

namespace platform { namespace wayland
{

class keyboard;
class mouse;

////////////////////////////////////////

/// @brief Wayland implementation of platform::dispatcher.
///
/// Dispatcher implemented using Wayland.
class dispatcher : public ::platform::dispatcher
{
public:
	/// @brief Constructor.
	dispatcher( ::platform::system *s, const std::shared_ptr<struct wl_display> &dpy );
	~dispatcher( void ) override;

	int execute( void ) override;
	void exit( int code ) override;
	void add_waitable( const std::shared_ptr<waitable> &w ) override;
	void remove_waitable( const std::shared_ptr<waitable> &w ) override;

	/// @brief Add a window.
	///
	/// Add a window for the dispatcher to handle events.
	void add_window( const std::shared_ptr<window> &w );

	void remove_window( const std::shared_ptr<window> &w );

private:
	void wake_up_executor( void );
	bool drain_wayland_events( void );

	int _exit_code = 0;
	int _wait_pipe[2];
	std::atomic<bool> _exit_requested{false};
	std::shared_ptr<struct wl_display> _display;
	std::shared_ptr<keyboard> _keyboard;
	std::shared_ptr<mouse> _mouse;
};

////////////////////////////////////////

} }

