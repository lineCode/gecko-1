
#pragma once

#include "image_frame.h"
#include "track.h"

namespace media
{

////////////////////////////////////////

class video_track : public track
{
public:
	virtual ~video_track( void );

	using track::track;

	std::shared_ptr<image_frame> operator[]( int64_t f )
	{
		return at( f );
	}

	virtual std::shared_ptr<image_frame> at( int64_t f ) = 0;

private:
};

////////////////////////////////////////

}

