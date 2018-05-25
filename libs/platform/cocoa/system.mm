//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "system.h"
#include "screen.h"
#include "window.h"
#include "dispatcher.h"
#include <gl/gl3w.h>

#include <mutex>
#include <stdexcept>
#include <dlfcn.h>

#include <base/contract.h>

#include <Cocoa/Cocoa.h>

namespace platform { namespace cocoa
{

////////////////////////////////////////

system::system( const std::string & )
	: platform::system( "cocoa", "Cocoa" )
{
    [NSAutoreleasePool new];
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    NSMenu *menubar = [[NSMenu new] autorelease];
    NSMenuItem *appMenuItem = [[NSMenuItem new] autorelease];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    NSMenu *appMenu = [[NSMenu new] autorelease];
    NSString *appName = [[NSProcessInfo processInfo] processName];
    NSString *quitTitle = [@"Quit " stringByAppendingString:appName];
    NSMenuItem *quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(terminate:) keyEquivalent:@"q"] autorelease];

    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];

	_keyboard = std::make_shared<keyboard>();
	_mouse = std::make_shared<mouse>();
	_dispatcher = std::make_shared<dispatcher>( _keyboard, _mouse );

    [NSApp activateIgnoringOtherApps:YES];

	NSArray *s = [NSScreen screens];
	for ( size_t i = 0; i < [s count]; ++i )
	{
		auto scr = std::make_shared<screen>( [s objectAtIndex:i] );
		_screens.push_back( scr );
	}
}

////////////////////////////////////////

system::~system( void )
{
}

////////////////////////////////////////

bool
system::is_working( void ) const
{
	return true;
}

////////////////////////////////////////

std::vector<std::shared_ptr<::platform::screen>> system::screens( void )
{
	return _screens;
}

////////////////////////////////////////

const std::shared_ptr<::platform::screen> &
system::default_screen( void )
{
	precondition( ! _screens.empty(), "expect at least 1 screen" );
	return _screens.front();
}

////////////////////////////////////////

std::shared_ptr<::platform::cursor>
system::new_cursor( void )
{
	std::cout << "TODO" << std::endl;
	return std::shared_ptr<::platform::cursor>();
}

////////////////////////////////////////

std::shared_ptr<::platform::cursor>
system::builtin_cursor( standard_cursor sc )
{
	std::cout << "TODO" << std::endl;
	return std::shared_ptr<::platform::cursor>();
}

////////////////////////////////////////

void
system::set_selection( selection sel )
{
	std::cout << "TODO" << std::endl;
}

////////////////////////////////////////

std::pair<std::vector<uint8_t>, std::string>
system::query_selection( selection_type sel,
						 const std::vector<std::string> &allowedMimeTypes,
						 const std::string &clipboardName )
{
	std::cout << "TODO" << std::endl;
	return std::make_pair( std::vector<uint8_t>(), std::string() );
}

////////////////////////////////////////

std::pair<std::vector<uint8_t>, std::string>
system::query_selection( selection_type sel,
						 const selection_type_function &mimeSelector,
						 const std::string &clipboardName )
{
	std::cout << "TODO" << std::endl;
	return std::make_pair( std::vector<uint8_t>(), std::string() );
}

////////////////////////////////////////

const std::vector<std::string> &
	system::default_string_types( void )
{
	std::cout << "TODO" << std::endl;
	static std::vector<std::string> mtypes
	{
		"text/plain;charset=utf-8", "text/plain"
	};
	return mtypes;
}

////////////////////////////////////////

selection_type_function
system::default_string_selector( void )
{
	std::cout << "TODO" << std::endl;
	auto selFun = [](const std::vector<std::string> &l) -> std::string 
		{
			std::string ret{ "text/plain;charset=utf-8" };
			if ( std::find( l.begin(), l.end(), ret ) != l.end() )
				return ret;
			ret = "text/plain";
			if ( std::find( l.begin(), l.end(), ret ) != l.end() )
				return ret;
			return std::string();
		};
	return selection_type_function{ selFun };
}

////////////////////////////////////////

system::mime_converter
system::default_string_converter( void )
{
	std::cout << "TODO" << std::endl;
	auto convFun = [](const std::vector<uint8_t> &d, const std::string &cur, const std::string &to) -> std::vector<uint8_t> 
		{
			if ( base::begins_with( cur, "text/plain" ) )
			{
				if ( base::begins_with( to, "text/plain" ) )
					return d;
			}
			return std::vector<uint8_t>();
		};
	return mime_converter{ convFun };
}

////////////////////////////////////////

void
system::begin_drag( selection sel,
					const std::shared_ptr<::platform::cursor> &c )
{
	std::cout << "TODO" << std::endl;
}

////////////////////////////////////////

std::pair<std::vector<uint8_t>, std::string>
system::query_drop( const selection_type_function &chooseMimeType )
{
	std::cout << "TODO" << std::endl;
	return std::make_pair( std::vector<uint8_t>(), std::string() );
}


////////////////////////////////////////

std::shared_ptr<menu> system::new_system_menu( void )
{
	return std::shared_ptr<menu>();
}

////////////////////////////////////////

std::shared_ptr<tray> system::new_system_tray_item( void )
{
	return std::shared_ptr<tray>();
}

////////////////////////////////////////

std::shared_ptr<platform::window> system::new_window( const std::shared_ptr<::platform::screen> &s )
{
	rect p{ 0, 0, 320, 240 };
	auto ret = std::make_shared<::platform::cocoa::window>( s ? s : default_screen(), p );
	_dispatcher->add_window( ret );
	return ret;
}

////////////////////////////////////////

void
system::destroy_window( const std::shared_ptr<::platform::window> &w )
{
	auto x = std::static_pointer_cast<window>( w );
	_dispatcher->remove_window( x );
}

////////////////////////////////////////

std::shared_ptr<platform::dispatcher> system::get_dispatcher( void )
{
	return _dispatcher;
}

////////////////////////////////////////

std::shared_ptr<platform::keyboard> system::get_keyboard( void )
{
	return _keyboard;
}

////////////////////////////////////////

std::shared_ptr<platform::mouse> system::get_mouse( void )
{
	return _mouse;
}

////////////////////////////////////////

uint8_t
system::modifier_state( void )
{
	std::cout << "TODO" << std::endl;
	return 0;
}

////////////////////////////////////////

bool
system::query_mouse( uint8_t &buttonMask, uint8_t &modifiers, coord_type &x, coord_type &y, int &screen )
{
	std::cout << "TODO" << std::endl;
	buttonMask = 0;
	modifiers = 0;
	x = 0;
	y = 0;
	screen = -1;
	return false;
}



////////////////////////////////////////

} // namespace cocoa
} // namespace platform

