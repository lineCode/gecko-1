//
// Copyright (c) 2015-2016 Ian Godin and Kimball Thurston
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <string>

namespace base
{

////////////////////////////////////////

/// if the file name is blah.exr, the extension
/// returned will be 'exr'
std::string file_extension( const std::string &file );

std::string remove_extension( const std::string &file );
std::string remove_extension( std::string &&file );

////////////////////////////////////////

std::string basename( const std::string &file );

////////////////////////////////////////

}

