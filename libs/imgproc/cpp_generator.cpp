
#include "cpp_generator.h"
#include "variable.h"
#include "function.h"
#include <utf/utf.h>

namespace imgproc
{

////////////////////////////////////////

cpp_generator::cpp_generator( std::ostream &cpp )
	: _cpp( cpp )
{
	_globals = std::make_shared<scope>();
//	_globals->set_compile( [&]( const std::shared_ptr<func> &f, const std::vector<data_type> &args ) { return this->compile( f, args ); } );

	// Add default stuff
	_cpp << "// Generated by imgc\n\n";
//	_cpp << "#include <imgproc/buffer.h>\n";
//	_cpp << "#include <cmath>\n";
//	_cpp << "using namespace imgproc;\n";
//	_cpp << "float clamp( float x, float mn, float mx ) { return std::max( std::min( x, mx ), mn ); }\n";

	// Add built-in functions
//	_globals->add_function( U"floor", std::make_shared<func>( U"std::floor", U"x" ) );
//	_globals->add_function( U"ceil", std::make_shared<func>( U"std::ceil", U"x" ) );
//	_globals->add_function( U"abs", std::make_shared<func>( U"std::fabs", U"x" ) );
//	_globals->add_function( U"erf", std::make_shared<func>( U"std::erf", U"x" ) );
//	_globals->add_function( U"clamp", std::make_shared<func>( U"clamp", U"x", U"mn", U"mx" ) );
//	_globals->add_function( U"array", std::make_shared<func>( U"array" ) );
}

////////////////////////////////////////

std::string cpp_generator::operator()( const integer_expr &e )
{
	uint64_t v = e.integer();
	std::string result = "_tmp" + base::to_string( _tmp++ );
	_cpp << "\tuint64_t " << result << " = " << v << ";\n";
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const floating_expr &e )
{
	// TODO when should we use FLOAT32?
	double v = e.floating();
	std::string result = "_tmp" + base::to_string( _tmp++ );
	_cpp << "\tdouble " << result << " = " << v << ";\n";
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const identifier_expr &e )
{
	return std::string( base::format( "{0}", e.value() ) );
}

////////////////////////////////////////

std::string cpp_generator::operator()( const prefix_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );
	auto line = base::format( "\tauto {0} = {1}({2});\n", result, e.operation(), visit( e.expression() ) );
	_cpp << line;
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const postfix_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );
	auto line = base::format( "\tauto {0} = ({1}){2};\n", result, visit( e.expression() ), e.operation() );
	_cpp << line;
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const infix_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );
	std::string e1 = visit( e.expression1() );
	std::string e2 = visit( e.expression2() );
	auto line = base::format( "\tauto {0} = ({1}){2}({3});\n", result, e1, e.operation(), e2 );
	_cpp << line;
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const circumfix_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );
	auto line = base::format( "\tauto {0} = {1}{2}{3};\n", result, e.open(), visit( e.expression() ), e.close() );
	_cpp << line;
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const postcircumfix_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );
	auto line = base::format( "\tauto {0} = {1}{2}{3}{4};\n", result, visit( e.expression1() ), e.open(), visit( e.expression2() ), e.close() );
	_cpp << line;
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const call_expr &e )
{
	std::stringstream args;
	bool first = true;
	for ( auto &a: e.arguments() )
	{
		if ( !first )
			args << ", ";
		args << visit( a );
		first = false;
	}

	std::string result = "_tmp" + base::to_string( _tmp++ );
	_cpp << base::format( "\tauto {0} = {1}( {2} );\n", result, e.function(), args.str() );

	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const if_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );

	auto cond = visit( e.condition() );
	auto wtrue = visit( e.when_true() );
	auto wfalse = visit( e.when_false() );

	_cpp << base::format( "\tauto {0};\n", result );
	_cpp << base::format( "\tif ( {0} )\n", cond );
	_cpp << base::format( "\t\t{0} = {1};\n", result, wtrue );
	_cpp << "\telse\n";
	_cpp << base::format( "\t\t{0} = {1};\n", result, wfalse );

	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const range_expr &e )
{
	precondition( e.start(), "null starting value" );

	std::string result = base::format( "{0}", e.variable() );
	if ( e.end() )
	{
		if ( e.by() )
			_cpp << base::format( "\tfor ( auto {0} = {1}; {0} < {2}; {0} += {3} )\n", result, visit( e.start() ), visit( e.end() ), visit( e.by() ) );
		else
			_cpp << base::format( "\tfor ( auto {0} = {1}; {0} < {2}; {0} += 1 )\n", result, visit( e.start() ), visit( e.end() ) );
	}
	else
	{
		if ( e.by() )
			_cpp << base::format( "\tfor ( auto {0} = imgproc::get_start( {1}, {3} ); {0} < imgproc::get_end( {1}, {3} ); {0} += {2} )\n", result, visit( e.start() ), visit( e.by() ), e.index() );
		else
			_cpp << base::format( "\tfor ( auto {0} = imgproc::get_start( {1}, {2} ); {0} < imgproc::get_end( {1}, {2} ); {0} += 1 )\n", result, visit( e.start() ), e.index() );
	}
	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const for_expr &e )
{
	std::string result = "_tmp" + base::to_string( _tmp++ );

	_cpp << base::format( "\tauto {0};\n", result );
	for ( auto &range: e.ranges() )
	{
		(*this)( range );
		_cpp << "\t{\n";
	}

	_cpp << base::format( "\t{0} = {1};\n", result, visit( e.result() ) );

	for ( auto &range: e.ranges() )
		_cpp << "\t}\n";

	return result;
}

////////////////////////////////////////

std::string cpp_generator::operator()( const assign_expr &e )
{
	return std::string();
}

////////////////////////////////////////

std::string cpp_generator::operator()( const lambda_expr &e )
{
	return std::string();
}



////////////////////////////////////////

/*
void cpp_generator::add_functions( const std::vector<std::shared_ptr<func>> &funcs )
{
	for ( auto f: funcs )
		_globals->add_function( f->name(), f );
}
*/

////////////////////////////////////////

type cpp_generator::compile( const function &f, const std::vector<type> &types )
{
	_cpp << base::format( "{0}\n{1}\n", f.name(), '{' );
	visit( f.result() );
	_cpp << "}\n\n";
	return types.at( 0 );
}

////////////////////////////////////////

/*
std::shared_ptr<func> cpp_generator::get_function( const std::u32string &name )
{
	auto f = _globals->get_function( name );
	if ( !f )
		throw_runtime( "function \"{0}\" not found", name );
	return f;
}
*/

////////////////////////////////////////

std::string cpp_generator::visit( const std::shared_ptr<expr> &e )
{
	precondition( e, "null expression" );
	_type = e->get_type();
	return base::visit<std::string>( *this, *e );
}

////////////////////////////////////////

}

