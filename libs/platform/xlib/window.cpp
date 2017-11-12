//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "window.h"
#include <base/pointer.h>

#include <string.h>
#include <iostream>

#include <base/contract.h>
#include <base/scope_guard.h>
#include <stdexcept>
#include <gl/check.h>
#include <X11/Xutil.h>

#include "system.h"
#include "cursor.h"

namespace {

const int visual_attribs[] =
{
//	GLX_X_RENDERABLE    , True,
	GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
	GLX_RENDER_TYPE     , GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
	GLX_RED_SIZE        , 8,
	GLX_GREEN_SIZE      , 8,
	GLX_BLUE_SIZE       , 8,
	GLX_ALPHA_SIZE      , 2, // only use 2 in case we're on a 10-bit display
	GLX_DEPTH_SIZE      , 24,
	GLX_STENCIL_SIZE    , 8,
	GLX_DOUBLEBUFFER    , True,
	GLX_SAMPLE_BUFFERS  , 1,
	GLX_SAMPLES         , 4,
	None
};

/*
static bool isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;

  // Extension names should not have spaces.
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  // It takes a bit of care to be fool-proof about parsing the
  // OpenGL extensions string. Don't be fooled by sub-strings, etc.
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}
*/

}

namespace platform { namespace xlib
{

////////////////////////////////////////

window::window( system &s, const std::shared_ptr<Display> &dpy )
	: _display( dpy )
{
	precondition( _display, "null display" );

	Display *disp = _display.get();

	auto query = s.gl_proc_address();

	using glXQueryVersionProc = Bool (*)( Display *, int *, int * );
	auto glxVerFunc = reinterpret_cast<glXQueryVersionProc>( query( "glXQueryVersion" ) );
	if ( ! glxVerFunc  )
		throw_runtime( "Unable to find glXQueryVersion" );
	// Check GLX version.  Version 1.3 is needed for FBConfig
	int glx_major, glx_minor;
	if ( !glxVerFunc( disp, &glx_major, &glx_minor ) )
		throw_runtime( "glx query version failed" );
	if ( ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
		throw_runtime( "glx too old" );

	auto glxChooseFBFunc = reinterpret_cast<PFNGLXCHOOSEFBCONFIGPROC>( query( "glXChooseFBConfig" ) );
	if ( ! glxChooseFBFunc )
		throw_runtime( "Unable to find glXChooseFBConfig" );
	auto glxGetVis = reinterpret_cast<PFNGLXGETVISUALFROMFBCONFIGPROC>( query( "glXGetVisualFromFBConfig" ) );
	if ( ! glxGetVis )
		throw_runtime( "Unable to find glXGetVisualFromFBConfig" );
	PFNGLXGETFBCONFIGATTRIBPROC glxGetFBAttr = (PFNGLXGETFBCONFIGATTRIBPROC) query( "glXGetFBConfigAttrib" );

	// Get the framebuffer configs
	int fbcount;
	GLXFBConfig* fbc = glxChooseFBFunc( disp, DefaultScreen( disp ), visual_attribs, &fbcount );
	if ( fbc == nullptr )
		throw_runtime( "failed to get GL framebuffer configs" );
	on_scope_exit { XFree( fbc ); };

	// Find the best framebuffer
	int best_fbc = -1, best_num_samp = -1;
	for ( int i = 0; i < fbcount; ++i )
	{
		XVisualInfo *vi = glxGetVis( disp, fbc[i] );
		on_scope_exit { XFree( vi ); };

		if ( vi != nullptr )
		{
			int samp_buf, samples;
			glxGetFBAttr( disp, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glxGetFBAttr( disp, fbc[i], GLX_SAMPLES, &samples );
			if ( best_fbc < 0 || ( samp_buf && samples > best_num_samp ) )
				best_fbc = i, best_num_samp = samples;
		}
	}

	GLXFBConfig bestFbc = fbc[ best_fbc ];

	XVisualInfo *vi = glxGetVis( disp, bestFbc );
	if ( vi == nullptr )
		throw std::runtime_error( "no glx visual found" );
	on_scope_exit { XFree( vi ); };

	XSetWindowAttributes swa;
	swa.background_pixmap = None;
	swa.border_pixel = 0;
	swa.event_mask =
		ExposureMask | StructureNotifyMask | VisibilityChangeMask |
		EnterWindowMask | LeaveWindowMask |
		KeyPressMask | KeyReleaseMask | KeymapStateMask |
		ButtonPressMask | ButtonReleaseMask |
		PointerMotionMask | ButtonMotionMask;

	Window root = DefaultRootWindow( disp );
	swa.colormap = XCreateColormap( disp, root, vi->visual, AllocNone );

	_win = XCreateWindow( disp, root, 0, 0, 320, 240, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa );

	// Get the default screen's GLX extension list

	// NOTE: It is not necessary to create or make current to a context before
	// calling glXGetProcAddressARB
	auto glXCreateContextAttribsARB = reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>( query( "glXCreateContextAttribsARB" ) );

	if ( ! glXCreateContextAttribsARB )
		throw_runtime( "unable to retrieve the OpenGL glXCreateContextAttribsARB" );

	// If it does, try to get a GL 4.0 context!
	int atrributes[] =
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB|GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		None
	};

	_glc = glXCreateContextAttribsARB( disp, bestFbc, 0, True, atrributes );
	if ( ! _glc )
		throw std::runtime_error( "Unable to create OpenGL context" );

	_glc_makecurrent = reinterpret_cast<void (*)(Display *, GLXDrawable, GLXContext )>( query( "glXMakeCurrent" ) );
	if ( ! _glc_makecurrent )
		throw_runtime( "unable to retrieve glXMakeCurrent" );
	_glc_swapbuffers = reinterpret_cast<void (*)(Display *, GLXDrawable )>( query( "glXSwapBuffers" ) );
	if ( ! _glc_swapbuffers )
		throw_runtime( "unable to retrieve glXSwapBuffers" );

	acquire();

	if ( !gl3wIsSupported( 3, 3 ) )
		throw std::runtime_error( "opengl 3.3 not supported" );

//	std::cout << "OpenGL:\n\tvendor " << glGetString( GL_VENDOR )
//			  << "\n\trenderer " << glGetString( GL_RENDERER )
//			  << "\n\tversion " << glGetString( GL_VERSION )
//			  << "\n\tshader language " << glGetString( GL_SHADING_LANGUAGE_VERSION )
//			  << "\n\n" << std::endl;

//	int extCount;
//	glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);
//	std::cout << extCount << " extensions:\n";
//	for (int i = 0; i < extCount; ++i)
//		std::cout << "Extension " << i << ": " << glGetStringi( GL_EXTENSIONS, i ) << '\n';
//	std::cout << std::endl;

	// Sync to ensure any errors generated are processed.
	XSync( disp, False );
}

////////////////////////////////////////

window::~window( void )
{
	if ( _win != 0 )
		XDestroyWindow( _display.get(), _win );
}

////////////////////////////////////////

void window::raise( void )
{
	XRaiseWindow( _display.get(), _win );
}

////////////////////////////////////////

void window::lower( void )
{
	XLowerWindow( _display.get(), _win );
}

////////////////////////////////////////

void window::set_popup( void )
{
	XSetWindowAttributes swa;
	swa.override_redirect = true;
	XChangeWindowAttributes( _display.get(), _win, CWOverrideRedirect, &swa );
	_popup = true;
}

////////////////////////////////////////

void window::show( void )
{
	XMapWindow( _display.get(), _win );
	if ( _popup )
		XRaiseWindow( _display.get(), _win );
}

////////////////////////////////////////

void window::hide( void )
{
	XUnmapWindow( _display.get(), _win );
}

////////////////////////////////////////

bool window::is_visible( void )
{
	XWindowAttributes attr;	  
	XGetWindowAttributes( _display.get(), _win, &attr );
	return (attr.map_state == IsViewable);
}

////////////////////////////////////////

void
window::fullscreen( bool fs )
{
	// look at ghost - need to handle netwm method and motif method...
}

////////////////////////////////////////

/*
rect window::geometry( void )
{
}
*/

////////////////////////////////////////

void window::move( double x, double y )
{
	XMoveWindow( _display.get(), _win, int( x + 0.5 ), int( y + 0.5 ) );
}

////////////////////////////////////////

void window::resize( double w, double h )
{
	XResizeWindow( _display.get(), _win, static_cast<unsigned int>( std::max( 0.0, w ) + 0.5 ), static_cast<unsigned int>( std::max( 0.0, h ) + 0.5 ) );
}

////////////////////////////////////////

void window::set_minimum_size( double /*w*/, double /*h*/ )
{
}

////////////////////////////////////////

void window::set_title( const std::string &t )
{
	XStoreName( _display.get(), _win, t.c_str() );
}

////////////////////////////////////////

void window::invalidate( const base::rect & /*r*/ )
{
	if ( !_invalid )
	{
		XClearArea( _display.get(), _win, 0, 0, 0, 0, True );
		_invalid = true;
	}
}

////////////////////////////////////////

void window::acquire( void )
{
	_glc_makecurrent( _display.get(), _win, _glc );
}

////////////////////////////////////////

void window::release( void )
{
	_glc_makecurrent( _display.get(), None, nullptr );
}

////////////////////////////////////////

Window window::id( void ) const
{
	return _win;
}

////////////////////////////////////////

void window::move_event( double x, double y )
{
	int16_t tx = static_cast<int16_t>( x );
	int16_t ty = static_cast<int16_t>( y );
	if ( _last_x != tx || _last_y != ty )
	{
		_last_x = tx;
		_last_y = ty;
		if ( moved )
			moved( x, y );
	}
}

////////////////////////////////////////

void window::resize_event( double w, double h )
{
	uint16_t tw = static_cast<uint16_t>( w );
	uint16_t th = static_cast<uint16_t>( h );
	if ( _last_w != tw || _last_h != th )
	{
		_last_w = tw;
		_last_h = th;
		acquire();
		glViewport( 0, 0, tw, th );
		if ( resized )
			resized( w, h );
		release();
	}
}

////////////////////////////////////////

void window::expose_event( void )
{
	_invalid = false;
	acquire();
	if ( exposed )
		exposed();
	_glc_swapbuffers( _display.get(), _win );
//	glFlush();
//	XFlush( _display.get() );
	release();
}

////////////////////////////////////////

void window::make_current( const std::shared_ptr<::platform::cursor> &c )
{
	if ( c )
	{
		auto xc = static_cast<::platform::xlib::cursor *>( c.get() );
		XDefineCursor( _display.get(), _win, xc->handle() );
	}
	else
	{
		// no cursor - just inherit parent window cursor
		XUndefineCursor( _display.get(), _win );
	}

	XFlush( _display.get() );
}

////////////////////////////////////////

} }

