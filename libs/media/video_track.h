
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

	std::shared_ptr<image_frame> at( int64_t f )
	{
		return std::shared_ptr<image_frame>( doRead( f ) );
	}

protected:
	friend class sample;

	virtual image_frame *read( int64_t offset, const sample_rate &r );
	virtual image_frame *doRead( int64_t offset ) = 0;

private:
};

////////////////////////////////////////

}

