//
// Copyright (c) 2018 Kimball Thurston
// All rights reserved.
// Copyrights licenced under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include <string>
#include <base/const_string.h>
#include <base/small_vector.h>
#include "metadata.h"
#include "view.h"

////////////////////////////////////////

namespace media
{

///
/// @brief Class layer provides an abstraction to store a layer within a track.
///
/// Some media, such as openexr, supports having many logical layers. This is
/// a simple layer to encapsulate this.
///
class layer
{
public:
    layer( base::cstring nm ) : _name( nm ) {}

    const std::string &name( void ) const { return _name; }

    bool has_multiple_views( void ) const { return view_count() > 1; }
    size_t view_count( void ) const { return _views.size(); }

    const view &operator[]( size_t i ) const { return _views[i]; }
    const view &operator[]( base::cstring v ) const;

    const std::string &default_view_name( void ) const;
    const view &default_view( void ) const;

    bool has_view( size_t viewIdx ) const;
    bool has_view( base::cstring v ) const;

    std::shared_ptr<image> find_image( base::cstring v ) const;
    std::shared_ptr<data> find_data( base::cstring v ) const;

    inline void set_meta( base::cstring name, metadata_value v ) { _metadata[name] = std::move( v ); }
	inline const metadata &meta( void ) const { return _metadata; }

    view &add_view( base::cstring nm );

private:
    std::string _name;
    metadata _metadata;
    base::small_vector<view, 2> _views;
};

} // namespace media



