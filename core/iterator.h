
#pragma once

////////////////////////////////////////

template<typename It>
class iterator_range
{
public:
	iterator_range( It b, It e )
		: _begin( b ), _end( e )
	{
	}

	It begin( void ) const
	{
		return _begin;
	}

	It end( void ) const
	{
		return _end;
	}

private:
	It _begin;
	It _end;
};

////////////////////////////////////////

template<typename It>
iterator_range<It> range( It b, It e )
{
	return iterator_range<It>( b, e );
}

////////////////////////////////////////

template<typename I, typename T>
class column_iterator : public std::iterator<std::random_access_iterator_tag,T>
{
public:
	column_iterator( I b, int n )
		: _iterator( b ), _n( n )
	{
	}

	column_iterator<I,T> &operator+=( int i )
	{
		_iterator += i;
		return *this;
	}

	column_iterator<I,T> &operator-=( int i )
	{
		_iterator -= i;
		return *this;
	}

	column_iterator<I,T> operator+( int i )
	{
		return column_iterator<I,T>( _iterator + i, _n );
	}

	column_iterator<I,T> operator-( int i )
	{
		return column_iterator<I,T>( _iterator - i, _n );
	}

	int operator-( const column_iterator<I,T> &i )
	{
		return _iterator - i._iterator;
	}

	column_iterator<I,T> &operator++( void )
	{
		_iterator++;
		return *this;
	}

	bool operator!=( const column_iterator<I,T> &e )
	{
		return _iterator != e._iterator;
	}

	/*
	T *operator->( void )
	{
		return &( (*_iterator)[_n] );
	}
	*/

	T &operator*( void )
	{
		return (*_iterator)[_n];
	}

private:
	I _iterator;
	int _n;
};

////////////////////////////////////////

template<typename C>
iterator_range<column_iterator<typename C::iterator, typename C::value_type::value_type>> column( C &c, int n )
{
	auto beg = column_iterator<typename C::iterator, typename C::value_type::value_type>( c.begin(), n ); 
	auto end = column_iterator<typename C::iterator, typename C::value_type::value_type>( c.end(), n ); 

	return iterator_range<column_iterator<typename C::iterator, typename C::value_type::value_type>>( beg, end );
}

////////////////////////////////////////

template<typename C>
iterator_range<typename C::iterator> rows( C &c )
{
	return range( c.begin(), c.end() );
}

////////////////////////////////////////

template<typename C>
std::vector<iterator_range<column_iterator<typename C::iterator, typename C::value_type::value_type>>> columns( C &c )
{
	std::vector<iterator_range<column_iterator<typename C::iterator, typename C::value_type::value_type>>> ranges;
	if ( !c.empty() )
	{
		auto &row = c[0];
		for ( size_t i = 0; i < row.size(); ++i )
		{
			ranges.emplace_back(
				column_iterator<typename C::iterator, typename C::value_type::value_type>( c.begin(), i ),
				column_iterator<typename C::iterator, typename C::value_type::value_type>( c.end(), i ) );
		}
	}

	return ranges;
}

////////////////////////////////////////
