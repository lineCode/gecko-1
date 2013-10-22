
#pragma once

#include <string>
#include <memory>
#include <functional>

#include "painter.h"

namespace platform
{

////////////////////////////////////////

/// @brief A rectangular area of the screen
class window
{
public:
	/// @brief Constructor
	window( void );

	/// @brief Destructor
	virtual ~window( void );

	/// @brief Raise the window
	/// Raise the window above all other windows.
	virtual void raise( void ) = 0;

//	virtual void lower( void ) = 0;

	/// @brief Show the window
	/// Make the window visible.
	virtual void show( void ) = 0;

	/// @brief Hide the window
	/// Make the window invisible.
	virtual void hide( void ) = 0;

	/// @brief Query if the window is visible
	/// @return Whether the window is visible or not
	virtual bool is_visible( void ) = 0;

//	virtual rect geometry( void ) = 0;

//	virtual void set_geometry( const rect &r ) = 0;

	/// @brief Resize the window
	/// Resize the window to the given size.
	/// @param w New width of the window
	/// @param h New height of the window
	virtual void resize( double w, double h ) = 0;

	/// @brief Set minimum window size
	/// The window will not be allowed to resize smaller than the minimum given.
	/// @param w Minimum width for the window
	/// @param h Minimum height for the window
	virtual void set_minimum_size( double w, double h ) = 0;

	/// @brief Set the window title
	/// Set the window title shown in the title bar.
	/// @param t The window title
	virtual void set_title( const std::string &t ) = 0;

//	virtual void set_icon( const icon &i );

	virtual std::shared_ptr<painter> paint( void ) = 0;

	void when_closed( std::function<void(void)> f ) { _closed = f; }
	void when_shown( std::function<void(void)> f ) { _shown = f; }
	void when_hidden( std::function<void(void)> f ) { _hidden = f; }
	void when_minimized( std::function<void(void)> f ) { _minimized = f; }
	void when_maximized( std::function<void(void)> f ) { _maximized = f; }
	void when_restored( std::function<void(void)> f ) { _restored = f; }
	void when_exposed( std::function<void()> f ) { _exposed = f; }
	void when_moved( std::function<void(double,double)> f ) { _moved = f; }
	void when_resized( std::function<void(double,double)> f ) { _resized = f; }
	void when_entered( std::function<void(void)> f ) { _entered = f; }
	void when_exited( std::function<void(void)> f ) { _exited = f; }

	void closed( void ) { if ( _closed ) _closed(); }
	void shown( void ) { if ( _shown ) _shown(); }
	void hidden( void ) { if ( _hidden ) _hidden(); }
	void minimized( void ) { if ( _minimized ) _minimized(); }
	void maximized( void ) { if ( _maximized ) _maximized(); }
	void restored( void ) { if ( _restored ) _restored(); }
	void exposed( void ) { if ( _exposed ) _exposed(); }
	void moved( double x, double y ) { if ( _moved ) _moved( x, y ); }
	void resized( double w, double h ) { if ( _resized ) _resized( w, h ); }
	void entered( void ) { if ( _entered ) _entered(); }
	void exited( void ) { if ( _exited ) _exited(); }

private:
	std::function<void(void)> _closed;
	std::function<void(void)> _shown;
	std::function<void(void)> _hidden;
	std::function<void(void)> _minimized;
	std::function<void(void)> _maximized;
	std::function<void(void)> _restored;
	std::function<void(void)> _exposed;
	std::function<void(double,double)> _moved;
	std::function<void(double,double)> _resized;
	std::function<void(void)> _entered;
	std::function<void(void)> _exited;
};

////////////////////////////////////////

}

