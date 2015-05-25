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

#include <istream>
#include <ostream>
#include <iostream>
#include <memory>
#include "streambuf.h"


////////////////////////////////////////


namespace base
{

///
/// @brief Provides a basic istream that stores a streambuf.
///
/// This allows to have a streambuf that is managed but provides differing
/// functionality with the same interface
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
class istream_bufstore : virtual public std::basic_istream<CharT, TraitsT>
{
public:
	typedef CharT char_type;
	typedef typename TraitsT::int_type int_type;
	typedef typename TraitsT::pos_type pos_type;
	typedef typename TraitsT::off_type off_type;
	typedef TraitsT traits_type;

	typedef std::basic_istream<CharT, TraitsT> base_type;
	typedef base_streambuf<CharT, TraitsT> streambuf_type;

	istream_bufstore( std::unique_ptr<streambuf_type> &&sb )
			: base_type(), _sbuf( std::move( sb ) )
	{
		this->init( _sbuf.get() );
	}
	istream_bufstore( istream_bufstore &&o )
			: base_type( std::move( o ) ),
			  _sbuf( std::move( o._sbuf ) )
	{
		base_type::set_rdbuf( _sbuf.get() );
	}

	istream_bufstore &operator=( istream_bufstore &&rhs )
	{
		base_type::operator=( std::move( rhs ) );
		_sbuf = std::move( rhs._sbuf );
		base_type::set_rdbuf( _sbuf.get() );
		return *this;
	}
	void
	swap( istream_bufstore &rhs )
	{
		base_type::swap( rhs );
		std::swap( _sbuf, rhs._sbuf );
		base_type::set_rdbuf( _sbuf.get() );
		rhs.base_type::set_rdbuf( rhs._sbuf.get() );
	}

	virtual ~istream_bufstore( void )
	{
	}

	//////////////////
	// members

	// Read the bit in ios_base about rdbuf and the hiding of the
	// base rdbuf function. Given that we're using a pointer,
	// seems like we should do the same thing to discourage fiddling
	streambuf_type *rdbuf( void ) const { return _sbuf.get(); }
	const std::string &uri( void ) const { return _sbuf->get_uri(); }

protected:
	istream_bufstore( const istream_bufstore & ) = delete;
	istream_bufstore &operator=( const istream_bufstore & ) = delete;
	
private:
	std::unique_ptr<streambuf_type> _sbuf;
};


///
/// @brief Provides a basic ostream that stores a streambuf by pointer.
///
/// This allows to have a streambuf that is managed but provides differing
/// functionality with the same interface
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
class ostream_bufstore : virtual public std::basic_ostream<CharT, TraitsT>
{
public:
	typedef CharT char_type;
	typedef typename TraitsT::int_type int_type;
	typedef typename TraitsT::pos_type pos_type;
	typedef typename TraitsT::off_type off_type;
	typedef TraitsT traits_type;

	typedef std::basic_ostream<CharT, TraitsT> base_type;
	typedef base_streambuf<CharT, TraitsT> streambuf_type;

	ostream_bufstore( std::unique_ptr<streambuf_type> &&sb )
			: base_type(), _sbuf( std::move( sb ) )
	{
		this->init( _sbuf.get() );
	}
	ostream_bufstore( ostream_bufstore &&o )
			: base_type( std::move( o ) ),
			  _sbuf( std::move( o._sbuf ) )
	{
		base_type::set_rdbuf( _sbuf.get() );
	}

	ostream_bufstore &operator=( ostream_bufstore &&rhs )
	{
		base_type::operator=( std::move( rhs ) );
		_sbuf = std::move( rhs._sbuf );
		base_type::set_rdbuf( _sbuf.get() );
		return *this;
	}
	void
	swap( ostream_bufstore &rhs )
	{
		base_type::swap( rhs );
		std::swap( _sbuf, rhs._sbuf );
		base_type::set_rdbuf( _sbuf.get() );
		rhs.base_type::set_rdbuf( rhs._sbuf.get() );
	}

	virtual ~ostream_bufstore( void )
	{
	}

	//////////////////
	// members

	// Read the bit in ios_base about rdbuf and the hiding of the
	// base rdbuf function. Given that we're using a pointer,
	// seems like we should do the same thing to discourage fiddling
	streambuf_type *rdbuf( void ) const { return _sbuf.get(); }
	const std::string &uri( void ) const { return _sbuf->get_uri(); }

protected:
	ostream_bufstore( const ostream_bufstore & ) = delete;
	ostream_bufstore &operator=( const ostream_bufstore & ) = delete;
	
private:
	std::unique_ptr<streambuf_type> _sbuf;
};


///
/// @brief Provides a basic ostream that stores a streambuf by pointer.
///
/// This allows to have a streambuf that is managed but provides differing
/// functionality with the same interface
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
class iostream_bufstore : virtual public std::basic_iostream<CharT, TraitsT>
{
public:
	typedef CharT char_type;
	typedef typename TraitsT::int_type int_type;
	typedef typename TraitsT::pos_type pos_type;
	typedef typename TraitsT::off_type off_type;
	typedef TraitsT traits_type;

	typedef std::basic_iostream<CharT, TraitsT> base_type;
	typedef base_streambuf<CharT, TraitsT> streambuf_type;

	iostream_bufstore( std::unique_ptr<streambuf_type> &&sb )
			: base_type(), _sbuf( std::move( sb ) )
	{
		this->init( _sbuf.get() );
	}
	iostream_bufstore( iostream_bufstore &&o )
			: base_type( std::move( o ) ),
			  _sbuf( std::move( o._sbuf ) )
	{
		base_type::set_rdbuf( _sbuf.get() );
	}

	iostream_bufstore &operator=( iostream_bufstore &&rhs )
	{
		base_type::operator=( std::move( rhs ) );
		_sbuf = std::move( rhs._sbuf );
		base_type::set_rdbuf( _sbuf.get() );
		return *this;
	}
	void
	swap( iostream_bufstore &rhs )
	{
		base_type::swap( rhs );
		std::swap( _sbuf, rhs._sbuf );
		base_type::set_rdbuf( _sbuf.get() );
		rhs.base_type::set_rdbuf( rhs._sbuf.get() );
	}

	virtual ~iostream_bufstore( void )
	{
	}

	//////////////////
	// members

	// Read the bit in ios_base about rdbuf and the hiding of the
	// base rdbuf function. Given that we're using a pointer,
	// seems like we should do the same thing to discourage fiddling
	streambuf_type *rdbuf( void ) const { return _sbuf.get(); }
	const std::string &uri( void ) const { return _sbuf->get_uri(); }

protected:
	iostream_bufstore( const iostream_bufstore & ) = delete;
	iostream_bufstore &operator=( const iostream_bufstore & ) = delete;
	
private:
	std::unique_ptr<streambuf_type> _sbuf;
};

typedef istream_bufstore<char> istream;
typedef ostream_bufstore<char> ostream;
typedef iostream_bufstore<char> iostream;

typedef istream_bufstore<wchar_t> wistream;
typedef ostream_bufstore<wchar_t> wostream;
typedef iostream_bufstore<wchar_t> wiostream;

} // namespace base


