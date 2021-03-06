//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "atom.h"
#include <core/pointer.h>
#include <core/contract.h>
#include "xcbpp.h"

namespace platform { namespace xcb
{

////////////////////////////////////////

std::string get_atom_name( xcb_connection_t *c, xcb_atom_t a )
{
	auto reply = xcbpp_get_atom_name( c, a );

	size_t len = xcb_get_atom_name_name_length( reply.get() );
	return std::string( xcb_get_atom_name_name( reply.get() ), len );
}

////////////////////////////////////////

xcb_atom_t get_atom( xcb_connection_t *c, const std::string &name )
{
	xcb_atom_t atom = XCB_NONE;
	auto reply = xcbpp_intern_atom( c, 0, name.size(), name.c_str() );
	if ( reply )
		atom = reply->atom;
	return atom;
}

////////////////////////////////////////

} }

