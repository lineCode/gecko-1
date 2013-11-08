
#pragma once

#include <memory>
#include <functional>
#include <draw/point.h>

namespace platform
{

////////////////////////////////////////

/// @brief Mouse device.
///
/// Mouse device which can send button press/release and mouse motion events.
class mouse
{
public:

	/// @brief Constructor.
	mouse( void );

	/// @brief Destructor.
	virtual ~mouse( void );

private:
};

////////////////////////////////////////

}

