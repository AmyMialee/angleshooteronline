#pragma once

class Widget : public sf::Drawable, public sf::Transformable {
	bool selected;
	bool pressed;

public:
	Widget();
	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;
	[[nodiscard]] bool isSelected() const;
	[[nodiscard]] bool isPressed() const;
	virtual void setSelected(bool selected);
	virtual void setPressed(bool pressed);
	[[nodiscard]] virtual bool canBeSelected() const;
	virtual void tick(float deltaTime);
	virtual void handleEvent(const sf::Event& event);
};