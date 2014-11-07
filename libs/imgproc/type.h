
#pragma once

#include <vector>
#include <map>
#include <typeindex>
#include <base/variant.h>
#include "data_type.h"

namespace imgproc
{

class type;

////////////////////////////////////////

class type_variable
{
public:
	type_variable( void )
		: _id()
	{
	}

	type_variable( size_t i )
		: _id( i )
	{
	}

	size_t id( void ) const
	{
		return _id;
	}	

	bool operator==( const type_variable &v ) const
	{
		return id() == v.id();
	}

	bool operator!=( const type_variable &v ) const
	{
		return id() != v.id();
	}

	bool operator<( const type_variable &v ) const
	{
		return id() < v.id();
	}
	
private:
	size_t _id;
};

////////////////////////////////////////

class type_operator
{
public:
	type_operator( void )
		: _type( pod_type::UNKNOWN, 0 )
	{
	}

	type_operator( const var_type &t )
		: _type( t )
	{
	}

	type_operator( pod_type t, size_t d = 0 )
		: _type( t, d )
	{
	}

	template<typename Iterator>
	type_operator( const var_type &t, Iterator first, Iterator last )
		: _type( t ), _types( first, last )
	{
	}

	template<typename Range>
	type_operator( const var_type &t, Range range )
		: _type( t ), _types( range.begin(), range.end() )
	{
	}

	bool compare_type( const type_operator &o ) const
	{
		bool ret = ( _type == o._type );
		return ret;
	}

	std::vector<type>::iterator begin( void )
	{
		return _types.begin();
	}

	std::vector<type>::iterator end( void )
	{
		return _types.end();
	}

	std::vector<type>::const_iterator begin( void ) const
	{
		return _types.begin();
	}

	std::vector<type>::const_iterator end( void ) const
	{
		return _types.end();
	}

	bool empty( void ) const
	{
		return _types.empty();
	}

	size_t size( void ) const
	{
		return _types.size();
	}

	const type &at( size_t i ) const
	{
		return _types.at( i );
	}

	pod_type base_type( void ) const
	{
		return _type.base_type();
	}

	size_t dimensions( void ) const
	{
		return _type.dimensions();
	}

	std::string name( void ) const;

	void add( type t );

	void set_type( pod_type t )
	{
		_type.set_type( t );
	}

	void set_dimensions( size_t d )
	{
		_type.set_dimensions( d );
	}

private:
	var_type _type;
	std::vector<type> _types;
};

////////////////////////////////////////

class type : public base::variant<type_variable,type_operator>
{
public:
	using variant::variant;
};

////////////////////////////////////////

std::ostream &operator<<( std::ostream &out, const type &t );

////////////////////////////////////////

}

