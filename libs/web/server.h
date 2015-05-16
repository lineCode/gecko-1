
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>

#include <base/thread_pool.h>
#include "response.h"
#include "request.h"

namespace web
{

////////////////////////////////////////

class server
{
public:
	server( uint16_t port, size_t threads = 1 );

	typedef std::function<void( response&, request& )> handler;

	handler &resource( const std::string &method, const std::string &re );

	handler &default_resource( const std::string &method );

	void run( void );

private:
	void handle_client( net::tcp_socket &client );
	std::map<std::string,std::map<std::string,handler>> _resources;
	std::map<std::string,handler> _defaults;

	bool _done = false;
	uint16_t _port;
	size_t _threads = 0;
};

////////////////////////////////////////

}

