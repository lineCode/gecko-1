//
// Copyright (c) 2018 Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <platform/context.h>
#include <windows.h>

////////////////////////////////////////

namespace platform
{
namespace mswin
{

///
/// @brief Class context provides...
///
class context : public ::platform::context
{
public:
	using base = ::platform::context;
	using render_func_ptr = base::render_func_ptr;
	using render_query = base::render_query;

	context( void );
	~context( void ) override;

	render_query render_query_func( void ) override;

	void init( HWND hwnd );
	void share( base &o ) override;

	void set_viewport( coord_type x, coord_type y, coord_type w, coord_type h ) override;
	void swap_buffers( void ) override;

protected:
	void acquire( void ) override;
	void release( void ) override;

	void reset_clip( const rect &r ) override;

private:
	HGLRC _hrc = nullptr;
	HDC _hdc = nullptr;
};

} // namespace mswin
} // namespace platform


