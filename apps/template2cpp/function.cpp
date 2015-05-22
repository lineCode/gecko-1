
#include "function.h"

////////////////////////////////////////

function::function( std::string n, size_t indent )
	: _name( std::move( n ) ), _indent( indent, '\t' )
{
	_code += _indent;
	_code += "std::string result;\n";
}

////////////////////////////////////////

void function::push_text( bool trim )
{
	if ( trim )
		_builder = base::rtrim( _builder );
	if ( !_builder.empty() )
	{
		_code += _indent;
		_code += "result.append( \"";
		int count = _builder.size();
		for ( size_t i = 0; i < _builder.size(); ++i )
		{
			char c = _builder[i];
			switch ( c )
			{
				case '"': _code.append( "\\\"" ); break;
				case '\t': _code.append( "\\t" ); break;
				case '\n': _code.append( "\\n" ); break;
				case '\r': _code.append( "\\r" ); break;

				default:
					_code.push_back( c );
					break;
			}
		}
		_code += "\", ";
		_code += base::to_string( count );
		_code += " );\n";
	}
	_builder.clear();
}

////////////////////////////////////////

void function::push_code( void )
{
	_builder = base::trim( _builder );
	if ( !_builder.empty() )
	{
		_code += _indent;
		for ( size_t i = 0; i < _builder.size(); ++i )
		{
			char c = _builder[i];
			if ( c == '\n' )
			{
				_code = base::rtrim( _code );
				_code.push_back( c );
				_code.append( _indent );
			}
			else
				_code.push_back( c );
		}
		_code = base::rtrim( _code );
		_code.push_back( '\n' );
	}
	_builder.clear();
}

////////////////////////////////////////

void function::push_expr( void )
{
	if ( !_builder.empty() )
	{
		_code += _indent;
		_code += "result.append( ";
		_code += _builder;
		_code += " );\n";
	}
	_builder.clear();
}

////////////////////////////////////////

void function::save( std::ostream &out )
{
	out << "\nstd::string " << _name << "( ";
	if ( _args.empty() )
		out << "void";
	else
		std::copy( _args.begin(), _args.end(), base::infix_ostream_iterator<std::string>( out, ", " ) );
	out << " )\n";
	out << "{\n" << _code << "\n\treturn result;\n}\n";
}

////////////////////////////////////////
