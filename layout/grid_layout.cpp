
#include "grid_layout.h"

namespace layout
{

////////////////////////////////////////

grid_layout::grid_layout( const std::shared_ptr<draw::area> &c )
	: _container( c ), _rows( c ), _columns( c )
{
	_rows.box.set_direction( direction::DOWN );
	_columns.box.set_direction( direction::RIGHT );
}

////////////////////////////////////////

std::vector<std::shared_ptr<draw::area>> grid_layout::new_row( double w )
{
	std::vector<std::shared_ptr<draw::area>> ret;
	auto r = std::make_shared<draw::area>();
	_rows.box.add_area( r, w );
	_rows.cells.emplace_back( r, orientation::VERTICAL );
	auto &row = _rows.cells.back();
	for ( auto &col: _columns.cells )
	{
		auto a = std::make_shared<draw::area>();
		ret.push_back( a );
		row.add_area( a );
		col.add_area( a );
	}

	return ret;
}

////////////////////////////////////////

std::vector<std::shared_ptr<draw::area>> grid_layout::new_column( double w )
{
	std::vector<std::shared_ptr<draw::area>> ret;
	auto c = std::make_shared<draw::area>();
	_columns.box.add_area( c, w );
	_columns.cells.emplace_back( c, orientation::HORIZONTAL );
	auto &col = _columns.cells.back();
	for ( auto &row: _rows.cells )
	{
		auto a = std::make_shared<draw::area>();
		ret.push_back( a );
		col.add_area( a );
		row.add_area( a );
	}

	return ret;
}

////////////////////////////////////////

void grid_layout::recompute_minimum( void )
{
	for ( auto r: _rows.cells )
		r.recompute_minimum();
	for ( auto c: _columns.cells )
		c.recompute_minimum();

	_rows.box.recompute_minimum();
	_columns.box.recompute_minimum();
}

////////////////////////////////////////

void grid_layout::recompute_layout( void )
{
	_rows.box.recompute_constraint();
	_columns.box.recompute_constraint();

	for ( auto r: _rows.cells )
		r.recompute_constraint();
	for ( auto c: _columns.cells )
		c.recompute_constraint();
}

////////////////////////////////////////

}

