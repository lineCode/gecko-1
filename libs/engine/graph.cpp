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

#include "graph.h"

#include <vector>
#include <map>
#include <deque>
#include <base/contract.h>

#include "registry.h"

////////////////////////////////////////

namespace engine
{

////////////////////////////////////////
graph::graph( const registry &r )
	: _ops( r )
{
}

////////////////////////////////////////

graph::~graph( void )
{
}

////////////////////////////////////////

node_id
graph::add_node( const base::cstring &opname, const dimensions &d, std::initializer_list<node_id> inputs )
{
	return add_node( _ops.find( opname ), any(), d, inputs );
}

////////////////////////////////////////

void
graph::tag_rvalue( node_id n )
{
	precondition( n < _nodes.size(), "invalid node id {0}", n );
	_nodes[n].set_rvalue();
}

////////////////////////////////////////

const any &
graph::get_value( node_id n )
{
	precondition( n < _nodes.size(), "invalid node id {0}", n );
	any &v = _nodes[n].value();
	if ( v.empty() )
		v = process( n );

	return v;
}

////////////////////////////////////////

node_id
graph::copy_node( const graph &o, node_id n )
{
	precondition( n < o.size(), "invalid node id {0}", n );
	if ( &o == this )
		return n;

	std::vector<node_id> tocopy;
	std::map<node_id, node_id> nodemap;
	tocopy.push_back( n );
	std::vector<node_id> inputs;

	while ( ! tocopy.empty() )
	{
		const node &cur = o[tocopy.back()];
		bool doAdd = true;
		size_t nInputs = cur.input_size();
		inputs.resize( nInputs );

		for ( size_t i = 0; i != nInputs; ++i )
		{
			node_id curIn = cur.input( i );
			auto inN = nodemap.find( curIn );
			if ( inN == nodemap.end() )
			{
				doAdd = false;
				tocopy.push_back( curIn );
			}
			else
				inputs[i] = inN->second;
		}

		if ( doAdd )
		{
			node_id nnode = add_node( cur.op(), cur.value(), cur.dims(), inputs, cur.hash_value() );
			nodemap[tocopy.back()] = nnode;
			tocopy.pop_back();
		}
	}

	return nodemap[n];
}

////////////////////////////////////////

node_id
graph::move_node( graph &o, node_id n )
{
	precondition( n < o.size(), "invalid node id {0}", n );
	if ( &o == this )
		return n;
	precondition( o[n].output_size() == 0, "Attempt to move a node with outputs in graph" );

	node_id nnode = copy_node( o, n );
	o.remove_node( n );
	return nnode;
}

////////////////////////////////////////

void
graph::remove_node( node_id n )
{
	precondition( n < size(), "invalid node id {0}", n );
	std::deque<node_id> toDel;
	toDel.push_back( n );
	while ( ! toDel.empty() )
	{
		node_id curDel = toDel.front();
		toDel.pop_front();
		node &curDelN = _nodes[curDel];
		const node_id *b = curDelN.begin_inputs();
		const node_id *be = curDelN.end_inputs();
		while ( b != be )
		{
			node &curIn = _nodes[*b];
			if ( curIn.is_rvalue() )
				toDel.push_back( *b );
			else if ( curIn.output_size() == 1 )
			{
				auto rc = _ref_counts.find( *b );
				// no more references, let's schedule it for deletion
				if ( rc == _ref_counts.end() )
					toDel.push_back( *b );
			}

			curIn.remove_output( curDel );
			++b;
		}
		_nodes[curDel] = node();
	}
}

////////////////////////////////////////

any
graph::process( node_id )
{
	return any();
}

////////////////////////////////////////

std::vector<any>
graph::process( const std::vector<node_id> &nodes )
{
	std::vector<any> retval;
	for ( auto n: nodes )
		retval.push_back( process( n ) );
	return retval;
}

////////////////////////////////////////

node_id
graph::find_node( const hash::value &hv )
{
	auto i = _hash_to_node.find( hv );
	if ( i != _hash_to_node.end() )
		return i->second;
	return nullnode;
}

////////////////////////////////////////

node_id
graph::add_node( op_id op, any value, const dimensions &d, std::initializer_list<node_id> inputs )
{
	hash h;
	return add_node( op, std::move( value ), d, inputs, h );
}

////////////////////////////////////////

node_id
graph::add_node( op_id op, any value, const dimensions &d, std::initializer_list<node_id> inputs, hash &h )
{
	if ( _ops[op].input_size() != inputs.size() )
		throw_logic( "Invalid number of inputs passed to operator {0}", _ops[op].name() );

	h << op << d;
	for ( auto n: inputs )
	{
		precondition( n < _nodes.size(), "Invalid input given for new node" );
		h << _nodes[n].hash_value();
	}
	hash::value hv = h.final();

	return add_node( op, std::move( value ), d, inputs, hv );
}

////////////////////////////////////////

node_id
graph::add_node( op_id op, any value, const dimensions &d, std::initializer_list<node_id> inputs, const hash::value &hv )
{
	node_id n = find_node( hv );
	if ( n != nullnode )
	{
		// simple collision check
		const node &colCheck = _nodes[n];
		if ( colCheck.op() != op || colCheck.input_size() != inputs.size() || colCheck.dims() != d )
			throw_logic( "Hash collision with existing node {0} adding opname '{1}'", n, _ops[op].name() );

		return n;
	}
	
	node_id r = static_cast<node_id>( _nodes.size() );
	_nodes.emplace_back( node( op, d, inputs, std::move( value ), hv ) );
	_hash_to_node[hv] = r;

	for ( auto n: inputs )
		_nodes[n].add_output( r );

	return r;
}

////////////////////////////////////////

node_id
graph::add_node( op_id op, any value, const dimensions &d, const std::vector<node_id> &inputs, const hash::value &hv )
{
	node_id n = find_node( hv );
	if ( n != nullnode )
	{
		// simple collision check
		const node &colCheck = _nodes[n];
		if ( colCheck.op() != op || colCheck.input_size() != inputs.size() || colCheck.dims() != d )
			throw_logic( "Hash collision with existing node {0} adding opname '{1}'", n, _ops[op].name() );

		return n;
	}
	
	node_id r = static_cast<node_id>( _nodes.size() );
	_nodes.emplace_back( node( op, d, inputs, std::move( value ), hv ) );
	_hash_to_node[hv] = r;

	for ( auto n: inputs )
		_nodes[n].add_output( r );

	return r;
}

////////////////////////////////////////

} // engine


