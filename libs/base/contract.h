
#pragma once

#include <cassert>
#include <stdexcept>
#include <system_error>
#include "format.h"

namespace base
{

////////////////////////////////////////

/// @brief Exception describing the source file/line
/// @sa throw_location
/// @sa throw_add_location
class location_exception : public std::exception
{
public:
	/// @brief Constructor
	location_exception( const char *file, int line )
	{
		std::ostringstream str;
		str << "file " << file << " line " << line;
		_msg = std::move( str.str() );
	}

	/// @brief Get the source file location.
	const char *file( void ) const
	{
		return _file;
	}

	/// @brief Get the source line location.
	int line( void ) const
	{
		return _line;
	}

	/// @brief Get the exception message
	virtual const char *what( void ) const throw() override
	{
		return _msg.c_str();
	}

private:
	int _line;
	const char *_file;
	std::string _msg;
};

////////////////////////////////////////

/// @brief Print an exception, possible nested
void print_exception( std::ostream &out, const std::exception &e, int level = 0 );

////////////////////////////////////////

/// @brief Precondition exception
class precondition_error : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

////////////////////////////////////////

/// @brief Postcondition exception
class postcondition_error : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

////////////////////////////////////////

/// @defgroup base base
/// @{

/// @brief Throw the exception, adding it's source location
/// @param exc Exception to add source location to
/// @sa base::location_exception
#define throw_location( exc ) \
	do { \
		try { throw base::location_exception( __FILE__, __LINE__ ); } \
		catch ( ... ) { std::throw_with_nested( exc ); } \
	} while ( false )

/// @brief Add a message as a nested exception
///
/// Can only be called from inside a catch block.
/// @param formats Format arguments.
#define throw_add( ... ) \
	std::throw_with_nested( std::runtime_error( base::format( __VA_ARGS__ ) ) );

/// @brief Add the source location as a nested exception.
///
/// Can only be called from inside a catch block.
/// @sa base::location_exception
#define throw_add_location( ... ) \
	do { \
		try { std::throw_with_nested( base::location_exception( __FILE__, __LINE__ ) ); } \
		catch ( ... ) { throw_add( __VA_ARGS__ ); } \
	} while ( false )

/// @brief Throw a runtime error using format
/// @sa base::format
#define throw_runtime( ... ) \
	throw_location( std::runtime_error( base::format( __VA_ARGS__ ) ) )

/// @brief If check is not true, throw a postcondition_error
#define throw_not_yet() \
	throw_location( std::logic_error( "not yet implemented" ) )

/// @brief Throw a errno error using format
/// @sa base::format
#define throw_errno( ... ) \
	throw_location( std::system_error( errno, std::system_category(), base::format( __VA_ARGS__ ) ) )

/// @brief If check is not true, throw a precondition_error
#define precondition( check, ... ) \
	do { if ( !(check) ) throw_location( base::precondition_error( base::format( __VA_ARGS__ ) ) ); } while ( false )

/// @brief If check is not true, throw a postcondition_error
#define postcondition( check, ... ) \
	do { if ( !(check) ) throw_location( base::postcondition_error( base::format( __VA_ARGS__ ) ) ); } while ( false )

/// @brief If check is not true, throw a postcondition_error
#define logic_check( check, ... ) \
	do { if ( !(check) ) throw_location( std::logic_error( base::format( __VA_ARGS__ ) ) ); } while ( false )

/// @}

////////////////////////////////////////

}

