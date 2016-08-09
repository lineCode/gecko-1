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

#include "vector_field.h"
#include <base/contract.h>

////////////////////////////////////////

namespace image
{

////////////////////////////////////////

vector_field::vector_field( void )
{
}

////////////////////////////////////////

vector_field::vector_field( int w, int h, bool isAbsolute )
	: _u( w, h ), _v( w, h ), _absolute( isAbsolute )
{
}

////////////////////////////////////////

vector_field::vector_field( const engine::dimensions &d, bool isAbsolute )
	: vector_field( static_cast<int>( d.x ), static_cast<int>( d.y ), isAbsolute )
{
}

////////////////////////////////////////

vector_field::vector_field( const plane &u, const plane &v, bool isAbsolute )
	: _u( u ), _v( v ), _absolute( isAbsolute )
{
	precondition( u.width() == v.width() && u.height() == v.height(), "vector_field must have u and v of same size, received {0}x{1} and {2}x{3}", u.width(), u.height(), v.width(), v.height() );
}

////////////////////////////////////////

vector_field::vector_field( plane &&u, plane &&v, bool isAbsolute )
	: _u( std::move( u ) ), _v( std::move( v ) ), _absolute( isAbsolute )
{
	precondition( _u.width() == _v.width() && _u.height() == _v.height(), "vector_field must have u and v of same size, received {0}x{1} and {2}x{3}", _u.width(), _u.height(), _v.width(), _v.height() );
}

////////////////////////////////////////

vector_field::~vector_field( void )
{
}

////////////////////////////////////////

vector_field::vector_field( const vector_field &o )
	: computed_base( o ), _u( o._u ), _v( o._v ), _absolute( o._absolute )
{
}

////////////////////////////////////////

vector_field::vector_field( vector_field &&o )
	: computed_base( std::move( o ) ),
	  _u( std::move( o._u ) ), _v( std::move( o._v ) ),
	  _absolute( o._absolute )
{
}

////////////////////////////////////////

vector_field &vector_field::operator=( const vector_field &o )
{
	if ( this != &o )
	{
		internal_copy( o );
		_u = o._u;
		_v = o._v;
		_absolute = o._absolute;
	}
	return *this;
}

////////////////////////////////////////

vector_field &vector_field::operator=( vector_field &&o )
{
	adopt( std::move( o ) );
	_u = std::move( o._u );
	_v = std::move( o._v );
	_absolute = o._absolute;
	return *this;
}

////////////////////////////////////////

vector_field
vector_field::create( const plane &a, const plane &b, bool isAbsolute )
{
	return vector_field( a, b, isAbsolute );
}

////////////////////////////////////////

vector_field
vector_field::create( plane &&a, plane &&b, bool isAbsolute )
{
	return vector_field( std::move( a ), std::move( b ), isAbsolute );
}

////////////////////////////////////////

engine::hash &operator<<( engine::hash &h, const vector_field &v )
{
	if ( v.compute_hash( h ) )
		return h;

	h << typeid(v).hash_code() << v.width() << v.height() << v.is_absolute() << v.u() << v.v();
	return h;
}


////////////////////////////////////////


} // namespace image
