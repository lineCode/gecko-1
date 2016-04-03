//
// Copyright (c) 2016 Kimball Thurston & Ian Godin
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

#include <vector>
#include <base/uri.h>
#include <functional>
#include "track.h"
#include "video_track.h"
#include "audio_track.h"
#include "data_track.h"

#include "metadata.h"

namespace media
{

////////////////////////////////////////

/// container is a media container. A sample of
/// this would be a quicktime file, or an MXF,
/// or AVC, etc.
class container
{
public:
//	container( void );
//	~container( void );

	size_t size( void )
	{
		return _tracks.size();
	}

	/// There is some amount of container info
	/// such as creator and such
	inline const metadata &info( void ) const;
	inline void add_info( base::cstring n, base::any v );

	std::shared_ptr<track> operator[]( size_t i )
	{
		return _tracks.at( i );
	}

	std::shared_ptr<track> at( size_t i )
	{
		return _tracks.at( i );
	}

	void add_track( const std::shared_ptr<track> &t );

	inline const std::vector<std::shared_ptr<video_track>> &video_tracks( void ) const;
	inline const std::vector<std::shared_ptr<audio_track>> &audio_tracks( void ) const;
	inline const std::vector<std::shared_ptr<data_track>> &data_tracks( void ) const;

private:
	metadata _info;
	std::vector<std::shared_ptr<track>> _tracks;
	std::vector<std::shared_ptr<video_track>> _video_tracks;
	std::vector<std::shared_ptr<audio_track>> _audio_tracks;
	std::vector<std::shared_ptr<data_track>> _data_tracks;
};

////////////////////////////////////////

inline const metadata &container::info( void ) const
{ return _info; }
inline void container::add_info( base::cstring n, base::any v )
{ _info[n] = std::move( v ); }


////////////////////////////////////////

inline const std::vector<std::shared_ptr<video_track>> &container::video_tracks( void ) const
{
	return _video_tracks;
}

inline const std::vector<std::shared_ptr<audio_track>> &container::audio_tracks( void ) const
{
	return _audio_tracks;
}

inline const std::vector<std::shared_ptr<data_track>> &container::data_tracks( void ) const
{
	return _data_tracks;
}



////////////////////////////////////////

}

