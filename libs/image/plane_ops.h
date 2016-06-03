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

#pragma once

#include <base/contract.h>
#include "plane.h"
#include "plane_stats.h"
#include "plane_convolve.h"
#include <engine/float_ops.h>

////////////////////////////////////////

namespace image
{

inline plane create_plane( int w, int h, float v )
{
	engine::dimensions d;
	d.x = static_cast<uint16_t>( w );
	d.y = static_cast<uint16_t>( h );
	return plane( "p.assign", d, v );
}
inline plane create_plane( int w, int h, const engine::computed_value<float> &v )
{
	engine::dimensions d;
	d.x = static_cast<uint16_t>( w );
	d.y = static_cast<uint16_t>( h );
	return plane( "p.assign", d, v );
}

inline plane abs( const plane &p ) { return plane( "p.abs", p.dims(), p ); }
inline plane abs( plane &&p ) { return plane( "p.abs", p.dims(), std::move( p ) ); }

inline plane copysign( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to copysign for planes of different sizes" );
	return plane( "p.copysign_pp", a.dims(), a, b );
}

inline plane square( const plane &p ) { return plane( "p.square", p.dims(), p ); }
inline plane square( plane &&p ) { return plane( "p.square", p.dims(), std::move( p ) ); }

inline plane fma( const plane &p, float b, float c ) { return plane( "p.fma_pnn", p.dims(), p, b, c ); }

inline plane sqrt( const plane &p ) { return plane( "p.sqrt", p.dims(), p ); }
inline plane sqrt( plane &&p ) { return plane( "p.sqrt", p.dims(), std::move( p ) ); }

inline plane magnitude( const plane &p ) { return plane( "p.abs", p.dims(), p ); }
inline plane magnitude( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to find magnitude for planes of different sizes" );
	return plane( "p.mag2", a.dims(), a, b );
}

inline plane magnitude( const plane &a, const plane &b, const plane &c )
{
	precondition( a.width() == b.width() && a.height() == b.height() && a.width() == c.width() && a.height() == c.height(), "unable to find magnitude for planes of different sizes" );
	return plane( "p.mag3", a.dims(), a, b, c );
}

// exponential functions
inline plane exp( const plane &p ) { return plane( "p.exp", p.dims(), p ); }
inline plane log( const plane &p ) { return plane( "p.log", p.dims(), p ); }
inline plane exp2( const plane &p ) { return plane( "p.exp2", p.dims(), p ); }
inline plane log2( const plane &p ) { return plane( "p.log2", p.dims(), p ); }
inline plane pow( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to compute power for planes of different sizes" );
	return plane( "p.pow_pp", a.dims(), a, b );
}
inline plane pow( const plane &p, int i ) { return plane( "p.pow_pi", p.dims(), p, i ); }
inline plane pow( const plane &p, float v ) { return plane( "p.pow_pn", p.dims(), p, v ); }

// trig functions
inline plane atan2( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to compute atan2 for planes of different sizes" );
	return plane( "p.atan2", a.dims(), a, b );
}

// comparison operators

/// out = (a < b) ? c : d
inline plane if_less( const plane &a, float b, const plane &c, const plane &d )
{
	precondition( a.width() == c.width() && a.height() == c.height(), "unable to compute if_less for planes of different sizes" );
	precondition( a.width() == d.width() && a.height() == d.height(), "unable to compute if_less for planes of different sizes" );
	return plane( "p.if_less_fpp", a.dims(), a, b, c, d );
}

/// out = (a > b) ? c : d
inline plane if_greater( const plane &a, float b, const plane &c, const plane &d )
{
	precondition( a.width() == c.width() && a.height() == c.height(), "unable to compute if_less for planes of different sizes" );
	precondition( a.width() == d.width() && a.height() == d.height(), "unable to compute if_less for planes of different sizes" );
	return plane( "p.if_greater_fpp", a.dims(), a, b, c, d );
}

////////////////////////////////////////
// normal operators
////////////////////////////////////////

/// unary plane operators
inline plane operator-( const plane &p )
{
	return plane( "p.mul_pn", p.dims(), p, -1.F );
}
inline plane operator-( plane &&p )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), -1.F );
}

/// @defgroup Add two planes together, or add a value to a plane. We
/// enumerate all the variants explicitly so we can tag r-value
/// objects for re-use, non-cache, etc.
///
/// @{
inline plane operator+( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to add planes of different sizes" );
	return plane( "p.add_pp", a.dims(), a, b );
}

inline plane operator+( plane &&a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to add planes of different sizes" );
	return plane( "p.add_pp", a.dims(), std::move( a ), b );
}

inline plane operator+( const plane &a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to add planes of different sizes" );
	return plane( "p.add_pp", a.dims(), a, std::move( b ) );
}

inline plane operator+( plane &&a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to add planes of different sizes" );
	return plane( "p.add_pp", a.dims(), std::move( a ), std::move( b ) );
}

inline plane operator+( const plane &p, float v )
{
	return plane( "p.add_pn", p.dims(), p, v );
}
inline plane operator+( const plane &p, const engine::computed_value<float> &v )
{
	return plane( "p.add_pn", p.dims(), p, v );
}
inline plane operator+( plane &&p, float v )
{
	return plane( "p.add_pn", p.dims(), std::move( p ), v );
}
inline plane operator+( plane &&p, const engine::computed_value<float> &v )
{
	return plane( "p.add_pn", p.dims(), std::move( p ), v );
}
inline plane operator+( float v, const plane &p )
{
	return plane( "p.add_pn", p.dims(), p, v );
}
inline plane operator+( const engine::computed_value<float> &v, const plane &p )
{
	return plane( "p.add_pn", p.dims(), p, v );
}
inline plane operator+( const engine::computed_value<float> &v, plane &&p )
{
	return plane( "p.add_pn", p.dims(), std::move( p ), v );
}
inline plane operator+( float v, plane &&p )
{
	return plane( "p.add_pn", p.dims(), std::move( p ), v );
}

inline plane &operator+=( plane &a, const plane &b )
{
	a = a + b;
	return a;
}
inline plane &operator+=( plane &a, plane &&b )
{
	a = a + std::move( b );
	return a;
}
inline plane &operator+=( plane &a, float b )
{
	a = a + b;
	return a;
}
inline plane &operator+=( plane &a, const engine::computed_value<float> &b )
{
	a = a + b;
	return a;
}
/// @}

/// @defgroup subtract planes / values from planes.
/// Enumerate all the variants explicitly so we can tag r-value
/// objects for re-use, non-cache, etc.
///
/// @{
inline plane operator-( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to subtract planes of different sizes" );
	return plane( "p.sub_pp", a.dims(), a, b );
}

inline plane operator-( plane &&a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to subtract planes of different sizes" );
	return plane( "p.sub_pp", a.dims(), std::move( a ), b );
}

inline plane operator-( const plane &a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to subtract planes of different sizes" );
	return plane( "p.sub_pp", a.dims(), a, std::move( b ) );
}

inline plane operator-( plane &&a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to subtract planes of different sizes" );
	return plane( "p.sub_pp", a.dims(), std::move( a ), std::move( b ) );
}

inline plane operator-( const plane &p, float v )
{
	return plane( "p.add_pn", p.dims(), p, - v );
}
inline plane operator-( const plane &p, const engine::computed_value<float> &v )
{
	return plane( "p.add_pn", p.dims(), p, - v );
}
inline plane operator-( plane &&p, float v )
{
	return plane( "p.add_pn", p.dims(), std::move( p ), - v );
}
inline plane operator-( plane &&p, const engine::computed_value<float> &v )
{
	return plane( "p.add_pn", p.dims(), std::move( p ), - v );
}
inline plane operator-( float v, const plane &p )
{
	return plane( "p.fma_pnn", p.dims(), p, -1.F, v );
}
inline plane operator-( const engine::computed_value<float> &v, const plane &p )
{
	return plane( "p.fma_pnn", p.dims(), p, -1.F, v );
}
inline plane operator-( float v, plane &&p )
{
	return plane( "p.fma_pnn", p.dims(), std::move( p ), -1.F, v );
}
inline plane operator-( const engine::computed_value<float> &v, plane &&p )
{
	return plane( "p.fma_pnn", p.dims(), std::move( p ), -1.F, v );
}

inline plane &operator-=( plane &a, const plane &b )
{
	a = a - b;
	return a;
}
inline plane &operator-=( plane &a, plane &&b )
{
	a = a - std::move( b );
	return a;
}
inline plane &operator-=( plane &a, float b )
{
	a = a - b;
	return a;
}
inline plane &operator-=( plane &a, const engine::computed_value<float> &b )
{
	a = a - b;
	return a;
}
/// @}

/// @defgroup Multiplies planes and planes/values.  Enumerate all the
/// variants explicitly so we can tag r-value objects for re-use,
/// non-cache, etc.
///
/// @{
inline plane operator*( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to multiply planes of different sizes" );
	return plane( "p.mul_pp", a.dims(), a, b );
}

inline plane operator*( plane &&a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to multiply planes of different sizes" );
	return plane( "p.mul_pp", a.dims(), std::move( a ), b );
}

inline plane operator*( const plane &a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to multiply planes of different sizes" );
	return plane( "p.mul_pp", a.dims(), a, std::move( b ) );
}

inline plane operator*( plane &&a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to multiply planes of different sizes" );
	return plane( "p.mul_pp", a.dims(), std::move( a ), std::move( b ) );
}

inline plane operator*( const plane &p, float v )
{
	return plane( "p.mul_pn", p.dims(), p, v );
}
inline plane operator*( const plane &p, const engine::computed_value<float> &v )
{
	return plane( "p.mul_pn", p.dims(), p, v );
}
inline plane operator*( plane &&p, float v )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), v );
}
inline plane operator*( plane &&p, const engine::computed_value<float> &v )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), v );
}
inline plane operator*( float v, const plane &p )
{
	return plane( "p.mul_pn", p.dims(), p, v );
}
inline plane operator*( const engine::computed_value<float> &v, const plane &p )
{
	return plane( "p.mul_pn", p.dims(), p, v );
}
inline plane operator*( float v, plane &&p )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), v );
}
inline plane operator*( const engine::computed_value<float> &v, plane &&p )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), v );
}

inline plane &operator*=( plane &a, const plane &b )
{
	a = a * b;
	return a;
}
inline plane &operator*=( plane &a, plane &&b )
{
	a = a * std::move( b );
	return a;
}
inline plane &operator*=( plane &a, float b )
{
	a = a * b;
	return a;
}
inline plane &operator*=( plane &a, const engine::computed_value<float> &b )
{
	a = a * b;
	return a;
}
/// @}


/// @defgroup Divides planes and planes/values.  Enumerate all the
/// variants explicitly so we can tag r-value objects for re-use,
/// non-cache, etc.
///
/// @{
inline plane operator/( const plane &a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to divide planes of different sizes" );
	return plane( "p.div_pp", a.dims(), a, b );
}

inline plane operator/( plane &&a, const plane &b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to divide planes of different sizes" );
	return plane( "p.div_pp", a.dims(), std::move( a ), b );
}

inline plane operator/( const plane &a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to divide planes of different sizes" );
	return plane( "p.div_pp", a.dims(), a, std::move( b ) );
}

inline plane operator/( plane &&a, plane &&b )
{
	precondition( a.width() == b.width() && a.height() == b.height(), "unable to divide planes of different sizes" );
	return plane( "p.div_pp", a.dims(), std::move( a ), std::move( b ) );
}

inline plane operator/( const plane &p, float v )
{
	return plane( "p.mul_pn", p.dims(), p, 1.F / v );
}
inline plane operator/( const plane &p, const engine::computed_value<float> &v )
{
	return plane( "p.mul_pn", p.dims(), p, 1.F / v );
}
inline plane operator/( plane &&p, float v )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), 1.F / v );
}
inline plane operator/( plane &&p, const engine::computed_value<float> &v )
{
	return plane( "p.mul_pn", p.dims(), std::move( p ), 1.F / v );
}
inline plane operator/( float v, const plane &p )
{
	return plane( "p.div_np", p.dims(), v, p );
}
inline plane operator/( const engine::computed_value<float> &v, const plane &p )
{
	return plane( "p.div_np", p.dims(), v, p );
}
inline plane operator/( float v, plane &&p )
{
	return plane( "p.div_np", p.dims(), v, std::move( p ) );
}
inline plane operator/( const engine::computed_value<float> &v, plane &&p )
{
	return plane( "p.div_np", p.dims(), v, std::move( p ) );
}

inline plane &operator/=( plane &a, const plane &b )
{
	a = a / b;
	return a;
}
inline plane &operator/=( plane &a, plane &&b )
{
	a = a / std::move( b );
	return a;
}
inline plane &operator/=( plane &a, float b )
{
	a = a / b;
	return a;
}
inline plane &operator/=( plane &a, const engine::computed_value<float> &b )
{
	a = a / b;
	return a;
}
/// @}

} // namespace image



