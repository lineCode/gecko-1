//
// Copyright (c) 2015 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include <base/contract.h>
#include "transaction.h"

namespace sqlite
{

////////////////////////////////////////

transaction::transaction( database &db )
	: _db( db )
{
	_db.exec( "BEGIN" );
}

////////////////////////////////////////

transaction::~transaction( void )
{
	if ( !_committed )
		_db.exec( "ROLLBACK" );
}

////////////////////////////////////////

void transaction::commit( void )
{
	precondition( !_committed, "transaction already committed" );
	_db.exec( "COMMIT" );
	_committed = true;
}

////////////////////////////////////////

}

