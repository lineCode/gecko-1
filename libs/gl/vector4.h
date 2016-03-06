
#pragma once

#include <base/contract.h>
#include <initializer_list>

namespace gl
{

////////////////////////////////////////

/// @brief A vector of 4 floats
class vector4
{
public:
	/// @brief Constructor
	vector4( std::initializer_list<float> l )
	{
		std::copy( l.begin(), l.end(), _data );
	}

	float operator[]( size_t i ) const
	{
		return _data[i];
	}

	float at( size_t i ) const
	{
		precondition( i < 4, "invalid index" );
		return _data[i];
	}

	/// @brief Get data
	const float *data( void ) const
	{
		return _data;
	}

private:
	float _data[4];
};

////////////////////////////////////////

/// @brief Dot product of 2 vectors
inline float operator*( const vector4 &a, const vector4 &b )
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

////////////////////////////////////////

}

