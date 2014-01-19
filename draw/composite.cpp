
#include "composite.h"

namespace draw
{

////////////////////////////////////////

composite::composite( void )
{
}

////////////////////////////////////////

void composite::draw( gl::context &ctxt )
{
	for ( auto d: _parts )
		d->draw( ctxt );
}

////////////////////////////////////////

}

