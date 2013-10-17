
#include "window.h"
#include "painter.h"
#include <core/contract.h>
#include <stdexcept>
#include <xcb/xcb.h>

namespace xcb
{

////////////////////////////////////////

window::window( xcb_connection_t *c, xcb_screen_t *screen )
	: _connection( c )
{
	precondition( _connection, "null connection" );
	_win = xcb_generate_id( _connection );
	xcb_create_window( _connection,    // Connection
		XCB_COPY_FROM_PARENT,          // depth (same as root)
		_win,                          // window Id
		screen->root,                  // parent window
		0, 0,                          // x, y
		100, 100,                      // width, height
		10,                            // border_width
		XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
		screen->root_visual,           // visual
		0, NULL);                      // masks, not used yet
}

////////////////////////////////////////

window::~window( void )
{
}

////////////////////////////////////////

void window::raise( void )
{
	const static uint32_t values[] = { XCB_STACK_MODE_ABOVE };
	xcb_configure_window( _connection, _win, XCB_CONFIG_WINDOW_STACK_MODE, values);
}

////////////////////////////////////////

/*
void window::lower( void )
{
	const static uint32_t values[] = { XCB_STACK_MODE_BELOW };
	xcb_configure_window( _connection, _win, XCB_CONFIG_WINDOW_STACK_MODE, values);
}
*/

////////////////////////////////////////

void window::show( void )
{
	xcb_map_window( _connection, _win );
}

////////////////////////////////////////

void window::hide( void )
{
	xcb_unmap_window( _connection, _win );
}

////////////////////////////////////////

bool window::is_visible( void )
{
	// TODO fix this
	return true;
}

////////////////////////////////////////

/*
rect window::geometry( void )
{
}
*/

////////////////////////////////////////

void window::resize( double w, double h )
{
	const static uint32_t values[] = { uint32_t(w+0.5), uint32_t(h+0.5) };
	xcb_configure_window( _connection, _win, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values );
}

////////////////////////////////////////

void window::set_minimum_size( double w, double h )
{
}

////////////////////////////////////////

void window::set_title( const std::string &t )
{
}

////////////////////////////////////////

std::shared_ptr<platform::painter> window::paint( void )
{
	return std::make_shared<painter>( _connection );
}

////////////////////////////////////////

xcb_window_t window::id( void ) const
{
	return _win;
}

////////////////////////////////////////

}

