
#pragma once

#include "drawable.h"
#include "mesh.h"
#include <gl/check.h>

namespace draw
{

////////////////////////////////////////

class composite : public drawable
{
public:
	composite( void );

	void add( const std::shared_ptr<drawable> &d )
	{
		_parts.push_back( d );
	}

	void draw( gl::context &ctxt ) override;

private:
	std::vector<std::shared_ptr<drawable>> _parts;
};

////////////////////////////////////////

inline void operator+=( const std::shared_ptr<composite> &c, const std::shared_ptr<drawable> &d )
{
	c->add( d );
}

////////////////////////////////////////

}
