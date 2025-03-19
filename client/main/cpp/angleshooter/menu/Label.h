#pragma once
#include "Widget.h"

class Label final : public Widget {
	sf::Text text;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
public:
	explicit Label(const std::string& text);
	void setText(const std::string& string);
};