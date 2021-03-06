//
// Copyright (c) 2015 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <string>
#include <base/uri.h>
#include <base/json.h>

#include "response.h"

namespace web
{

////////////////////////////////////////

/// @brief HTTP client
class client
{
public:
	/// @brief Constructor
	client( std::string agent = "HTTPClient/0.0" );

	/// @brief Get an HTTP resource.
	response get( const base::uri &host, double timeout = 0.0 );
	response post( const base::uri &host, std::string &&v, double timeout = 0.0 );
	response post( const base::uri &host, const base::json &v, double timeout = 0.0 )
	{
		std::string tmp;
		tmp <<  v;
		return post( host, std::move( tmp ), timeout );
	}


private:
	std::string _agent;
};

////////////////////////////////////////

}

