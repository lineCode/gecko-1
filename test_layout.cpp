
#include <iostream>
#include <map>
#include <functional>

#include <platform/xcb/system.h>

#include <layout/form_layout.h>
#include <layout/box_layout.h>
#include <layout/grid_layout.h>
#include <layout/tree_layout.h>
#include <draw/cairo/font.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace {

////////////////////////////////////////

std::shared_ptr<layout> test_box( std::shared_ptr<container> c )
{
	auto l =  std::make_shared<box_layout>( c, direction::DOWN );
	for ( size_t i = 0; i < 5; ++i )
	{
		auto a = l->new_area( i == 2 ? 1.0 : 0.0 );
		a->set_minimum( 100, 24 );
	}
	return l;
}

////////////////////////////////////////

std::shared_ptr<layout> test_tree( std::shared_ptr<container> c )
{
	auto l = std::make_shared<tree_layout>( c, 24.0 );
	for ( size_t i = 0; i < 2; ++i )
	{
		auto a = l->new_area( 0.0 );
		a->set_minimum( 100, 24 );
	}
	auto b1 = l->new_branch();
	for ( size_t i = 0; i < 2; ++i )
	{
		auto a = b1->new_area();
		a->set_minimum( 100, 24 );
	}
	for ( size_t i = 0; i < 2; ++i )
	{
		auto a = l->new_area( 0.0 );
		a->set_minimum( 100, 24 );
	}
	l->new_area( 1.0 );

	return l;
}

////////////////////////////////////////

std::shared_ptr<layout> test_form( std::shared_ptr<container> c )
{
	std::shared_ptr<area> a, b;

	auto l = std::make_shared<form_layout>( c );

	for ( size_t i = 0; i < 5; ++i )
	{
		std::tie( a, b ) = l->new_row();
		a->set_minimum( 100, 24 );
		b->set_minimum( 100, 24 );
	}

	return l;
}

////////////////////////////////////////

std::shared_ptr<layout> test_grid( std::shared_ptr<container> c )
{
	auto l = std::make_shared<grid_layout>( c );

	for ( size_t i = 0; i < 5; ++i )
		auto tmp = l->new_column( i == 2 ? 1.0 : 0.0 );

	for ( size_t i = 0; i < 5; ++i )
	{
		auto tmp = l->new_row( i == 2 ? 1.0 : 0.0 );
		for ( auto a: tmp )
			a->set_minimum( 50, 24 );
	}

	return l;
}

////////////////////////////////////////

std::map<std::string,std::function<std::shared_ptr<layout>(std::shared_ptr<container>)>> tests =
{
	{ "grid", test_grid },
	{ "box", test_box },
	{ "form", test_form },
	{ "tree", test_tree },
};

////////////////////////////////////////

int safemain( int argc, char **argv )
{
	precondition( argc > 1, "expected argument" );

	auto sys = std::make_shared<xcb::system>();
	auto win = sys->new_window();

	std::shared_ptr<container> c = std::make_shared<container>();

	auto layout = tests[argv[1]]( c );

	FT_Library ftlib;
	if ( FT_Init_FreeType( &ftlib ) != 0 )
		throw std::runtime_error( "Freetype initialization failed" );

	auto font = std::make_shared<cairo::font>( ftlib, "/usr/share/fonts/TTF/Ubuntu-R.ttf", 0 );

	draw::gradient grad;
	grad.add_stop( 0.0, draw::color( 0.6, 0.6, 0.6 ) );
	grad.add_stop( 1.0, draw::color( 0.3, 0.3, 0.3 ) );

	auto redraw_window = [&] ( void )
	{
		auto canvas = win->canvas();
		canvas->fill( draw::color( 0.5, 0.5, 0.5 ) );

		bool skip = true;
		for ( auto a: *c )
		{
			if ( ( a->width() * a->height() > 0 ) && !skip )
			{
				draw::path p;
				p.rounded_rect( { a->x1() + 0.5, a->y1() + 0.5 }, a->width(), a->height(), 5 );
				draw::paint paint( draw::color( 1, 1, 1 ) );
				paint.set_fill_linear( { a->x1(), a->y1() }, 0, a->height(), grad );
				canvas->draw_path( p, paint );
			}
			else
				skip = false;
		}
		draw::paint paint2( draw::color( 1, 1, 1 ) );
		paint2.set_fill_color( { 0, 0, 0 } );
		canvas->draw_text( font, { 50, 150 }, "Hello World!", paint2 );
		canvas->present();
	};

	auto recompute_layout = [&] ( double w, double h )
	{
		layout->recompute_minimum();
		std::shared_ptr<area> b = c->bounds();

		if ( w < b->minimum_width() )
			w = b->minimum_width();
		if ( h < b->minimum_height() )
			h = b->minimum_height();
		b->set_horizontal( 0, w );
		b->set_vertical( 0, h );

		layout->recompute_layout();
	};

	recompute_layout( 640, 480 );

	win->when_resized( recompute_layout );
	win->when_exposed( redraw_window );

	win->set_title( "Hello World" );
	win->resize( 640, 480 );
	win->show();

	std::shared_ptr<platform::timer> t = sys->new_timer();
	t->when_elapsed( [&]
	{
		t->schedule( 10.0 );
	} );

//	win->when_entered( [&] { std::cout << "Entered!" << std::endl; } );
//	win->when_exited( [&] { std::cout << "Exited!" << std::endl; } );

	t->schedule( 1.0 );

	return sys->get_dispatcher()->execute();
}

////////////////////////////////////////

}

////////////////////////////////////////

int main( int argc, char *argv[] )
{
	int ret = -1;
	try
	{
		ret = safemain( argc, argv );
	}
	catch ( std::exception &e )
	{
		print_exception( std::cerr, e );
	}
	return ret;
}

////////////////////////////////////////

