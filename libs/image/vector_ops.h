//
// Copyright (c) 2016 Kimball Thurston
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "vector_field.h"
#include "image.h"

////////////////////////////////////////

namespace image
{

vector_field concatenate( const vector_field &a, const vector_field &b );
vector_field project( const vector_field &a, float scale );

plane divergence( const vector_field &a, const vector_field &b );

image_buf warp_dirac( const image_buf &src, const vector_field &v );
plane warp_dirac( const plane &src, const vector_field &v );

image_buf warp_bilinear( const image_buf &src, const vector_field &v );
plane warp_bilinear( const plane &src, const vector_field &v );

image_buf warp_lanczos( const image_buf &src, const vector_field &v );
plane warp_lanczos( const plane &src, const vector_field &v );

vector_field convert_to_absolute( const vector_field &v );
vector_field convert_to_relative( const vector_field &v );

/// This currently is based on the method used in the KITTI evaluation
/// database. It is normalized based on the vector magnitude, with an
/// optional scale. The direction and magnitude are converted to HSV
/// notation, and then converted to RGB.
/// 
/// The extension above KITTI is to additionally produce the vector
/// magnitude as the 4th (alpha) channel
image_buf colorize( const vector_field &v, float scale = 8.F );

void add_vector_ops( engine::registry &r );

} // namespace image

