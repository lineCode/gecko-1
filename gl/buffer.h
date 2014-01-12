
#pragma once

#include "opengl.h"
#include <vector>

namespace gl
{

////////////////////////////////////////

enum class target
{
	ARRAY_BUFFER = GL_ARRAY_BUFFER,
	ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
};

////////////////////////////////////////

enum class usage
{
	STREAM_DRAW = GL_STREAM_DRAW,
	STATIC_DRAW = GL_STATIC_DRAW,
	DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
};

template<typename D> class buffer;

////////////////////////////////////////

template<typename D>
class bound_buffer
{
public:
	bound_buffer( void ) = delete;
	bound_buffer( const bound_buffer &other ) = delete;
	bound_buffer( bound_buffer &&other )
		: _target( other._target )
	{
		other._target = 0;
	}

	~bound_buffer( void )
	{
		if ( _target != 0 )
			glBindBuffer( _target, 0 );
	}

	void data( const D *data, size_t n, usage u )
	{
		glBufferData( _target, n * sizeof(D), data, u );
	}

	void data( const std::vector<D> &data, usage u )
	{
		glBufferData( _target, data.size() * sizeof(D), data.data(), static_cast<GLenum>( u ) );
	}

	void sub_data( const D *data, size_t offset, size_t n )
	{
		glBufferSubData( _target, offset * sizeof(D), n * sizeof(D), data );
	}

	void sub_data( const std::vector<D> &data, size_t offset )
	{
		glBufferSubData( _target, offset * sizeof(D), data.size() * sizeof(D), data.data() );
	}

private:
	friend class buffer<D>;
	friend class context;

	bound_buffer( GLenum targ, GLuint buf )
		: _target( targ )
	{
		glBindBuffer( _target, buf );
	}

	GLenum _target;
};

////////////////////////////////////////

template<typename D>
class buffer
{
public:
	buffer( const buffer &b ) = delete;

	buffer( void )
	{
		glGenBuffers( 1, &_buffer );
	}

	buffer( gl::target targ, const D *data, size_t n, usage u )
		: buffer()
	{
		auto bb = bind( targ );
		bb.data( data, n, u );
	}

	buffer( gl::target targ, const std::vector<D> &data, usage u )
		: buffer()
	{
		auto bb = bind( targ );
		bb.data( data, u );
	}

	~buffer( void )
	{
		glDeleteBuffers( 1, &_buffer );
	}

	bound_buffer<D> bind( gl::target targ )
	{
		return bound_buffer<D>( static_cast<GLenum>( targ ), _buffer );
	}

private:
	friend class context;

	GLuint _buffer;
};

////////////////////////////////////////

}
