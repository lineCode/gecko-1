//
// Copyright (c) 2017 Kimball Thurston
// All rights reserved.
// Copyrights licenced under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include "spectral_sensitivity.h"

////////////////////////////////////////

namespace color
{

using matching_function = spectral_sensitivity<double>;
using luminous_efficiency_function = spectral_density<double>;

namespace two_deg
{

const matching_function &cie_1931_cmf( void );
const matching_function &cie_1931_judd_1951( void );
const matching_function &cie_1931_judd_vos( void );

/// LMS cone fundamental intensity functions (not yet ratified?)
/// x == l, y == m, z == s
const matching_function &cie_2012_xyz_cmf( void );
const luminous_efficiency_function &stockman_2005_Vlambda( void );
const luminous_efficiency_function &stockman_2011_Vlambda( void );

}

const luminous_efficiency_function &cie_1924_photopic_Vlambda( void );
const luminous_efficiency_function &cie_1924_photopic_Vlambda_judd_1951( void );
const luminous_efficiency_function &cie_1924_photopic_Vlambda_judd_vos( void );

namespace ten_deg
{

const matching_function &cie_1964_cmf( void );

/// LMS cone fundamental intensity function (not yet ratified)
/// as for two deg
const matching_function &cie_2012_xyz_cmf( void );

const luminous_efficiency_function &stockman_2005_Vlambda( void );
const luminous_efficiency_function &stockman_2011_Vlambda( void );
}

} // namespace color



