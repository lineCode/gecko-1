
#pragma once

#include <memory>
#include <map>
#include <platform/dispatcher.h>
#include "window.h"
#include "keyboard.h"
#include "mouse.h"

namespace mswin
{

////////////////////////////////////////

class dispatcher : public platform::dispatcher
{
public:
	dispatcher( const std::shared_ptr<keyboard> &k, const std::shared_ptr<mouse> &m );
	virtual ~dispatcher( void );

	int execute( void );
	void exit( int code );

	void add_window( const std::shared_ptr<window> &win );

private:
	int _exit_code = 0;
	std::shared_ptr<keyboard> _keyboard;
	std::shared_ptr<mouse> _mouse;
};

////////////////////////////////////////

}
