//
// Copyright (c) 2016 Kimball Thurston
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

#include "media_io.h"
#include <base/contract.h>

////////////////////////////////////////

namespace image
{

////////////////////////////////////////

image_buf
extract_frame( const media::image_frame &f )
{
	std::vector<std::string> chans;
	chans.reserve( f.size() );
	for ( size_t i = 0, N = f.size(); i != N; ++i )
		chans.emplace_back( f.name( i ) );

	return extract_frame( f, chans );
}

////////////////////////////////////////

image_buf extract_frame( const media::image_frame &f, const std::vector<std::string> &chans )
{
	size_t nC = chans.size();
	if ( f.size() < nC )
		throw_runtime( "Request for {0} channels/planes, but media image only has {1}", nC, f.size() );

	image_buf r;

	int64_t px = 0, py = 0;
	for ( size_t i = 0, N = nC; i != N; ++i )
	{
		if ( ! f.has_channel( chans[i] ) )
			throw_runtime( "media image does not have channel '{0}'", chans[i] );
		const media::image_buffer &ib = f[chans[i]];
		if ( i == 0 )
		{
			px = ib.width();
			py = ib.height();
		}
		else if ( px != ib.width() )
			throw_runtime( "media image channel {0} width {1} does not match channel 0 width {2}", i, ib.width(), px );
		else if ( py != ib.height() )
			throw_runtime( "media image channel {0} height {1} does not match channel 0 width {2}", i, ib.height(), py );

		// TODO: do we want this?
		//r.add_plane( plane( "m.conv_to_plane", d, f, c[i] ) );
		// need to add hash functions, etc.
		plane p( px, py );
		for ( int64_t y = 0; y < py; ++y )
			ib.get_scanline( y, p.line( static_cast<int>( y ) ), 1 );
		r.add_plane( std::move( p ) );
	}
	return r;
}

////////////////////////////////////////

std::shared_ptr<media::image_frame>
to_frame( const image_buf &i, const std::vector<std::string> &chans, const std::string &type )
{
	if ( i.size() < chans.size() )
		throw_runtime( "image does not have enough channels ({0}) for requested channel list size ({1})", i.size(), chans.size() );

	engine::dimensions d = i.dims();
	std::shared_ptr<media::image_frame> r = std::make_shared<media::image_frame>( d.x, d.y );
	for ( size_t c = 0, nC = chans.size(); c != nC; ++c )
	{
		const plane &p = i[c];
		media::image_buffer ib;
		if ( type == "f16" )
		{
			ib = media::image_buffer::simple_buffer<base::half>( p.width(), p.height() );
		}
		else if ( type == "u16" )
		{
			ib = media::image_buffer::simple_buffer<uint16_t>( p.width(), p.height() );
		}
		else
			throw_runtime( "Unknown/unhandled data type tag" );

		for ( int64_t y = 0, h = p.height(); y < h; ++y )
		{
			const float *pL = p.line( static_cast<int>( y ) );
			ib.set_scanline( y, pL, 1 );
		}
		r->add_channel( chans[c], ib );
	}
	return r;
}

////////////////////////////////////////

} // image


