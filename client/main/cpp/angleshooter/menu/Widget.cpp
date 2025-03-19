#include "PreCompiledClient.h"
#include "Widget.h"

Widget::Widget() : selected(false), pressed(false) {}

bool Widget::isPressed() const {
	return pressed;
}

bool Widget::isSelected() const {
	return selected;
}

void Widget::setPressed(bool pressed) {
	this->pressed = pressed;
}

bool Widget::canBeSelected() const {
	return false;
}

void Widget::tick(float deltaTime) {}

void Widget::handleEvent(const sf::Event& event) {}

void Widget::setSelected(bool selected) {
	this->selected = selected;
}