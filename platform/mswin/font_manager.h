
#pragma once

#include <platform/font_manager.h>

namespace mswin
{

////////////////////////////////////////

class font_manager : public platform::font_manager
{
public:
	font_manager( void );
	virtual ~font_manager( void );

	virtual std::set<std::string> get_families( void );
	virtual std::set<std::string> get_styles( void );

	virtual std::shared_ptr<draw::font> get_font( const std::string &family, const std::string &style, double pixsize );
};

////////////////////////////////////////

}
