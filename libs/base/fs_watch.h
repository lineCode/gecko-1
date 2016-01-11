//
// Copyright (c) 2015 Kimball Thurston
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "uri.h"
#include "const_string.h"
#include <memory>


////////////////////////////////////////


namespace base
{

/// what other events do we need? Need to keep this list to a minimum
/// for maximal cross-platform compatibility
enum class fs_event : uint8_t
{
	CREATED = 0x01,
	DELETED = 0x02,
	MODIFIED = 0x04,
	RENAMED_FROM = 0x08,
	RENAMED_TO = 0x10
};

constexpr inline fs_event operator|( fs_event lhs, fs_event rhs)
{
	using T = std::underlying_type<fs_event>::type;
	return static_cast<fs_event>( static_cast<T>(lhs) | static_cast<T>(rhs) );
}

constexpr inline uint8_t operator&( fs_event lhs, fs_event rhs )
{
	using T = std::underlying_type<fs_event>::type;
	return ( static_cast<T>(lhs) & static_cast<T>(rhs) );
}

inline fs_event &operator|=( fs_event &lhs, fs_event rhs )
{
	using T = std::underlying_type<fs_event>::type;
	lhs = static_cast<fs_event>( static_cast<T>(lhs) | static_cast<T>(rhs) );
	return lhs;
}

class fs_watch;

class fs_watcher
{
public:
	virtual ~fs_watcher( void );

	virtual void registerWatch( const base::uri &path, fs_watch &w,
								fs_event evt_mask, bool recursive ) = 0;
	virtual void move( fs_watch &w, fs_watch &neww ) = 0;
	virtual void unregisterWatch( fs_watch &w ) = 0;

protected:
	fs_watcher( const fs_watcher & ) = delete;
	fs_watcher &operator=( const fs_watcher & ) = delete;

	fs_watcher( void );
};

class fs_watch
{
public:
	typedef std::function<void(fs_event, const base::cstring &)> event_handler;

	fs_watch( const std::shared_ptr<fs_watcher> &fsw,
			  const event_handler &evt );
	fs_watch( fs_watch &&o );
	~fs_watch( void );
	fs_watch &operator=( fs_watch &&o );

	inline void emit( fs_event e, const base::cstring &p ) { _evt_cb( e, p ); }

private:
	fs_watch( const fs_watch & ) = delete;
	fs_watch &operator=( const fs_watch & ) = delete;

	std::shared_ptr<fs_watcher> _fsw;
	event_handler _evt_cb;
};

} // namespace base



