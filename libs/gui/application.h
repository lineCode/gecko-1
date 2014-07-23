
#pragma once

#include <memory>
#include <set>
#include "window.h"
#include "popup.h"
#include "menu.h"
#include <script/font_manager.h>

namespace gui
{

////////////////////////////////////////

class application : public std::enable_shared_from_this<application>
{
public:
	application( const std::string &platform = std::string(), const std::string &render = std::string() );
	~application( void );

	const std::string &active_platform( void ) { return _platform; }

	std::shared_ptr<window> new_window( void );
	std::shared_ptr<popup> new_popup( void );
	std::shared_ptr<menu> new_menu( void );

	int run( void );
	void exit( int code );

	void push( void );
	void pop( void );

	std::set<std::string> get_font_families( void );
	std::set<std::string> get_font_styles( const std::string &family );
	std::shared_ptr<script::font> get_font( const std::string &family, const std::string &style, double pixsize );

	std::shared_ptr<script::font> get_default_font( void );

	static std::shared_ptr<application> current( void );

private:
	struct impl;
	std::unique_ptr<impl> _impl;

	std::string _platform;
	std::shared_ptr<script::font_manager> _fmgr;
};

////////////////////////////////////////

}
