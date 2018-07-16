//
// Copyright (c) 2017 Ian Godin
// All rights reserved.
// Copyrights licensed under the MIT License.
// See the accompanying LICENSE.txt file for terms
//

#pragma once

#include "composite.h"
#include <layout/grid.h>
#include <base/signal.h>

namespace gui
{

class label_w;
class radio_button_w;

////////////////////////////////////////

class choices_w : public composite
{
public:
	choices_w( void );
	~choices_w( void ) override;

	void add_choice( const std::string &l );

	signal<void(size_t)> when_activated;

	void build( context &ctxt ) override;

protected:
	void for_subwidgets( const std::function<void(const std::shared_ptr<widget>&)> &f ) override;

	void choose( size_t c );

private:
	std::shared_ptr<layout::grid> _layout;
	std::vector<std::shared_ptr<label_w>> _labels;
	std::vector<std::shared_ptr<radio_button_w>> _radios;
};

////////////////////////////////////////

using choices = widget_ptr<choices_w>;

}
