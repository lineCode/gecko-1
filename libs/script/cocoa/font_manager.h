//
// Copyright (c) 2013-2017 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <script/font_manager.h>

namespace script { namespace cocoa
{

////////////////////////////////////////

/// @brief Cocoa implementation of font_manager.
class font_manager : public script::font_manager
{
public:
	/// @brief Constructor.
	font_manager( void );

	/// @brief Destructor.
	~font_manager( void ) override;

	std::set<std::string> get_families( void ) override;
	std::set<std::string> get_styles( const std::string &family ) override;

	std::shared_ptr<script::font> get_font( const std::string &family, const std::string &style, points pts, int dpiw, int dpih, int maxGlyphW, int maxGlyphH ) override;

private:
};

////////////////////////////////////////

} }
