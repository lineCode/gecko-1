
#pragma once

#include "widget.h"

namespace gui
{

////////////////////////////////////////

enum class scroll_behavior
{
	FREE,
	BOUND,
	NONE
};

////////////////////////////////////////

class scroll_area : public widget
{
public:
	scroll_area( scroll_behavior hscroll = scroll_behavior::BOUND, scroll_behavior vscroll = scroll_behavior::BOUND );
	~scroll_area( void );

	void set_widget( const std::shared_ptr<widget> &v )
	{
		_widget = v;
	}

	void paint( const std::shared_ptr<draw::canvas> &canvas ) override;

	bool mouse_press( const draw::point &p, int button ) override;
	bool mouse_release( const draw::point &p, int button ) override;
	bool mouse_move( const draw::point &p ) override;

	void compute_minimum( void ) override;
	void compute_layout( void ) override;

private:
	bool _tracking = false;
	draw::point _track;

	scroll_behavior _hscroll = scroll_behavior::BOUND, _vscroll = scroll_behavior::BOUND;
	std::shared_ptr<widget> _widget;
};

////////////////////////////////////////

}

