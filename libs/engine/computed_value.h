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

#include <memory>

#include <base/const_string.h>
#include <base/contract.h>

#include "graph.h"

////////////////////////////////////////

namespace engine
{

///
/// @brief computed_value provides a pattern for values computed using
/// an engine graph.
///
/// The idea behind this is to enable transparent construction of C++
/// code, but when a particular value is actually needed, the graph
/// can be processed at that point.
///
/// An example of this might be if there's a simple sum operator that
/// loops over a plane and sums all the pixel components. if that
/// operator is used in the graph, and passed as an argument to
/// another operator, this happens all internal to the graph. if
/// however, there's some c++ code that needs that value for a
/// conditional for some reason, the graph which acts as the value is
/// processed and the result improved. So you want to be able to say:
///
/// auto x = sum( p );
/// if ( static_cast<float>( x ) < 0.1F )
///   std::cout << "do something" << std::endl;
///
/// compared to
/// p = solarize( p, sum( p ) / static_cast<double>( p.width() * p.height() ) );
///
/// where the value can just stay a value and be processed later when
/// the item referencing p is evaluated.
///
template <typename T>
class computed_value
{
public:
	computed_value( void ) = default;
	~computed_value( void )
	{
		clear_graph();
	}

	template <typename... Args>
	explicit computed_value( const registry &r, const base::cstring &opname, const dimensions &d, Args &&... args )
	{
		_graph = find_or_create_graph( r, std::forward<Args>( args )... );
		_id = _graph->add_node( opname, d, { check_or_add( *_graph, std::forward<Args>( args ) )... } );
		_graph->reference( _id );
	}

	inline const std::shared_ptr<graph> &graph_ptr( void ) const { return _graph; }
	inline node_id id( void ) const { return _id; }
	inline dimensions node_dims( void ) const
	{
		dimensions r = nulldim;
		if ( _graph )
			r = (*_graph)[_id].dims();
		return r;
	}
	inline bool pending( void ) const
	{
		if ( _graph )
		{
			const node &n = (*_graph)[_id];
			if ( n.value().empty() )
				return true;
		}
			
		return false;
	}

	inline T compute( void ) const
	{
		if ( ! _graph )
			throw_runtime( "No graph to compute with" );
		const any &v = _graph->get_value( _id );
		return std::experimental::any_cast<T>( v );
	}

	inline void clear_graph( void ) noexcept
	{
		if ( _graph )
		{
			_graph->unreference( _id );
			_graph.reset();
		}
		_id = nullnode;
	}

protected:
	template <typename X, bool>
	struct check_delegate;

	template <typename X>
	struct check_delegate<X, false>
	{
		static node_id process( graph &g, X &&v )
		{
			return g.add_constant( std::forward<X>( v ) );
		}
	};
	template <typename X>
	struct check_delegate<X, true>
	{
		static node_id process( graph &g, X &&v )
		{
			node_id r = g.move_node( *(v.graph_ptr()), v.id() );
			g.tag_rvalue( r );
			return r;
		}
		static node_id process( graph &g, const X &v )
		{
			return g.copy_node( *(v.graph_ptr()), v.id() );
		}
	};

	template <typename X>
	inline node_id check_or_add( graph &g, X &&v )
	{
		return check_delegate<X, std::is_base_of<computed_value<X>, X>::value>::process( g, std::forward<X>( v ) );
	}

	template <typename X, typename... Args>
	inline std::shared_ptr<graph>
	find_or_create_graph( const registry &reg, X &&x, Args &&... args )
	{
		std::shared_ptr<graph> r = check( reg, std::forward<X>( x ) );
		if ( r )
			return r;
		return find_or_create_graph( reg, std::forward<Args>( args )... );
	}

	inline std::shared_ptr<graph>
	find_or_create_graph( const registry &reg )
	{
		return std::make_shared<graph>( reg );
	}

	template <typename V>
	inline std::shared_ptr<graph>
	check( const registry &reg, const computed_value<V> &x )
	{
		precondition( &(x.graph().op_registry()) == &reg, "computed_values with mixed operation registries intermingled" );
		return x.graph();
	}
	template <typename V>
	inline std::shared_ptr<graph>
	check( const registry &reg, const V & )
	{
		return std::shared_ptr<graph>();
	}

	std::shared_ptr<graph> _graph;
	node_id _id = nullnode;
};

} // namespace engine


