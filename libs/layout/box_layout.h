
#pragma once

#include <memory>
#include "layout.h"
#include "constraint.h"

namespace layout
{

////////////////////////////////////////

/// @brief Box layout.
///
/// Organizes areas in single row (or column).
/// Each area can have a weight.  Extra space will be distributed among the areas by weight.
template<typename container, typename area>
class box_layout : public layout<container,area>
{
public:
	box_layout( direction dir = direction::DOWN )
		: _dir( dir )
	{
	}

	void set_direction( direction d )
	{
		_dir = d;
	}

	void set_pad( double left, double right, double top, double bottom ) override
	{
		_pad[0] = left;
		_pad[1] = right;
		_pad[2] = top;
		_pad[3] = bottom;
	}

	void set_spacing( double horiz, double vert ) override
	{
		_hspacing = horiz;
		_vspacing = vert;
	}

	void add( const std::shared_ptr<area> &a, double weight = 0.0 )
	{
		_areas.push_back( a );
		_weights.push_back( weight );
		this->added( a );
	}

	void set_weight( size_t i, double weight )
	{
		_weights.at( i ) = weight;
	}

	void recompute_minimum( container &master ) override
	{
		switch ( _dir )
		{
			case direction::LEFT:
				flow_minimum( _areas, master, _dir, _hspacing, _pad[0], _pad[1] );
				overlap_minimum( _areas, master, orientation::VERTICAL, _pad[2], _pad[3] );
				break;

			case direction::RIGHT:
				flow_minimum( _areas, master, _dir, _hspacing, _pad[1], _pad[0] );
				overlap_minimum( _areas, master, orientation::VERTICAL, _pad[2], _pad[3] );
				break;

			case direction::UP:
				flow_minimum( _areas, master, _dir, _vspacing, _pad[2], _pad[3] );
				overlap_minimum( _areas, master, orientation::HORIZONTAL, _pad[0], _pad[1] );
				break;

			case direction::DOWN:
				flow_minimum( _areas, master, _dir, _vspacing, _pad[2], _pad[3] );
				overlap_minimum( _areas, master, orientation::HORIZONTAL, _pad[0], _pad[1] );
				break;
		}
	}

	void recompute_layout( container &master ) override
	{
		switch ( _dir )
		{
			case direction::LEFT:
				flow_constraint( _areas, _weights, master, _dir, _hspacing, _pad[0], _pad[1] );
				overlap_constraint( _areas, master, orientation::VERTICAL, _pad[2], _pad[3] );
				break;

			case direction::RIGHT:
				flow_constraint( _areas, _weights, master, _dir, _hspacing, _pad[1], _pad[0] );
				overlap_constraint( _areas, master, orientation::VERTICAL, _pad[2], _pad[3] );
				break;

			case direction::UP:
				flow_constraint( _areas, _weights, master, _dir, _vspacing, _pad[2], _pad[3] );
				overlap_constraint( _areas, master, orientation::HORIZONTAL, _pad[0], _pad[1] );
				break;

			case direction::DOWN:
				flow_constraint( _areas, _weights, master, _dir, _vspacing, _pad[2], _pad[3] );
				overlap_constraint( _areas, master, orientation::HORIZONTAL, _pad[0], _pad[1] );
				break;
		}
	}

protected:
	direction _dir = direction::DOWN;
	double _pad[4] = { 0.0, 0.0, 0.0, 0.0 };
	double _hspacing = 0.0, _vspacing = 0.0;
	std::vector<std::shared_ptr<area>> _areas;
	std::vector<double> _weights;
};

////////////////////////////////////////

}
