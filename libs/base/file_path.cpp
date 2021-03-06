//
// Copyright (c) 2015-2016 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#include "file_path.h"

namespace base
{

////////////////////////////////////////

std::string file_extension( const std::string &file )
{
	size_t off = file.find_last_of( '.' );
	if ( off != std::string::npos )
		return file.substr( off + 1 );
	return std::string();
}

////////////////////////////////////////

std::string remove_extension( const std::string &file )
{
	size_t off = file.find_last_of( '.' );
	if ( off != std::string::npos )
		return file.substr( 0, off );
	return file;
}

////////////////////////////////////////

std::string remove_extension( std::string &&file )
{
	size_t off = file.find_last_of( '.' );
	if ( off != std::string::npos )
		file.erase( off );
	return std::move( file );
}

////////////////////////////////////////

std::string basename( const std::string &file )
{
	size_t start = file.find_last_of( '/' );
	if ( start == std::string::npos )
		start = 0;
	else
		start = start + 1;

	size_t end = file.find_last_of( '.' );
	if ( end == std::string::npos )
		end = file.size();

	return file.substr( start, end - start );
}

////////////////////////////////////////

}

