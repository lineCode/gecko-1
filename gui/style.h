
#pragma once

#include <memory>
#include <string>
#include <view/view.h>

namespace gui
{

////////////////////////////////////////

class style
{
public:
	style( void );
	virtual ~style( void );

	virtual std::shared_ptr<view::view> button_frame( const std::shared_ptr<draw::area> &area ) = 0;
	virtual std::shared_ptr<view::view> button_text( const std::shared_ptr<draw::area> &area, const std::string &txt ) = 0;

//	virtual std::shared_ptr<view::view> label_frame( const std::shared_ptr<draw::area> &area );
	virtual std::shared_ptr<view::view> label_text( const std::shared_ptr<draw::area> &area, const std::string &txt ) = 0;
};

////////////////////////////////////////

}
