//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <platform/font_manager.h>

namespace platform { namespace dummy
{

////////////////////////////////////////

class font_manager : public platform::font_manager
{
public:
	font_manager( void );
	~font_manager( void );

	std::set<std::string> get_families( void ) override;
	std::set<std::string> get_styles( const std::string &family ) override;

	std::shared_ptr<gldraw::font> get_font( const std::string &family, const std::string &style, double pixsize ) override;
};

////////////////////////////////////////

} }

