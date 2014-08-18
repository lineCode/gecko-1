
#include <base/contract.h>
#include <fstream>
#include <imgproc/token.h>
#include <imgproc/parser.h>
#include <map>
#include <memory>

namespace
{

int safemain( int argc, char *argv[] )
{
	precondition( argc == 2, "expected single argument" );

	std::ifstream src( argv[1] );

	/*
	imgproc::iterator token( src );
	while ( token.next() )
		std::cout << token << '\n';
	*/

	std::vector<std::shared_ptr<imgproc::func>> funcs;
	imgproc::parser parser( funcs, src );

	parser();
	if ( parser.has_errors() )
		std::cout << "ERROR: parsing " << argv[1] << std::endl;
	for ( auto msg: parser.messages() )
		std::cout << msg << std::endl;

	for ( auto f: funcs )
	{
		f->write( std::cout );
		std::cout << std::endl;
	}

	return 0;
}

}

int main( int argc, char *argv[] )
{
	try
	{
		return safemain( argc, argv );
	}
	catch ( const std::exception &e )
	{
		base::print_exception( std::cerr, e );
	}
}
