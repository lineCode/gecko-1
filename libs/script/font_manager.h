
#pragma once

#include <string>
#include <set>
#include <memory>
#include "font.h"

namespace script
{

////////////////////////////////////////

/// @brief Font manager
///
/// Allows creation and listing of fonts.
class font_manager
{
public:
	/// @brief Constructor.
	font_manager( void );

	/// @brief Destructor.
	virtual ~font_manager( void );

	/// @brief List of families.
	///
	/// Get a list of families of all managed fonts.
	/// @return The list of families
	virtual std::set<std::string> get_families( void ) = 0;

	/// @brief List of style.
	///
	/// Get a list of styles available.
	/// @return The list of styles.
	virtual std::set<std::string> get_styles( const std::string &family ) = 0;

	/// @brief Get a particular font.
	///
	/// Create and return a font with the given attributes.
	/// @param family Family of the font to create
	/// @param style Style of the font to create
	/// @param pixsize Size of the font to create
	///
	/// @return The best matching font found
	virtual std::shared_ptr<font> get_font( const std::string &family, const std::string &style, double pixsize ) = 0;

	static const std::vector<std::shared_ptr<font_manager>> &list( void );

	static std::shared_ptr<font_manager> common( void );

private:
	static void init( void );
	static void enroll( const std::shared_ptr<font_manager> &mgr ); 
};

////////////////////////////////////////

}
