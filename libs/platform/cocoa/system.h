
#pragma once

#include <platform/system.h>
#include "dispatcher.h"

namespace platform { namespace cocoa
{

////////////////////////////////////////

/// @brief Cocoa implementation of system.
class system : public ::platform::system
{
public:
	/// @brief Constructor.
	system( void );

	~system( void );

	std::vector<std::shared_ptr<::platform::screen>> screens( void ) override;
	std::shared_ptr<::platform::window> new_window( void ) override;
	std::shared_ptr<::platform::timer> new_timer( void ) override;

	std::shared_ptr<::platform::dispatcher> get_dispatcher( void ) override;
	std::shared_ptr<::platform::keyboard> get_keyboard( void ) override;
	std::shared_ptr<::platform::mouse> get_mouse( void ) override;

private:
	std::shared_ptr<dispatcher> _dispatcher;
	std::shared_ptr<keyboard> _keyboard;
	std::shared_ptr<mouse> _mouse;
};

////////////////////////////////////////

} }
