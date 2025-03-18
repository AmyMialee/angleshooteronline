#pragma once

class Widget : public sf::Drawable, public sf::Transformable, public EventHandler {
	bool selected;
	bool pressed;

public:
	typedef std::shared_ptr<Widget> Pointer;

	Widget();
	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;
	[[nodiscard]] bool isSelected() const;
	[[nodiscard]] bool isPressed() const;
	virtual void setSelected(bool selected);
	virtual void setPressed(bool pressed);
	[[nodiscard]] virtual bool canBeSelected() const;
	virtual void tick(float deltaTime);
	void handleEvent(const sf::Event& event) override;
};