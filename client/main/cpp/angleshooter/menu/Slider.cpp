#include "PreCompiledClient.h"
#include "Slider.h"

Slider::Slider() : forward(false), backward(false) {
	this->Button::setToggle(true);
	setCallback([this]{});
}

void Slider::setTextFunction(const std::function<std::string(double)>& textFunction) {
	this->textFunction = textFunction;
}

void Slider::setConstantCallback(const Consumer<double>& textCallback) {
	this->constantCallback = textCallback;
}

void Slider::setFinalCallback(const Consumer<double>& callback) {
	this->finalCallback = callback;
}

void Slider::setValue(double value) {
	this->value = value;
	if (this->textFunction) this->setText(this->textFunction(value));
}

void Slider::tick(float deltaTime) {
	if (this->isPressed() && (forward || backward)) {
		this->value += static_cast<float>(forward - backward) * (1 + deltaTime) * 0.01;
		this->value = std::clamp(value, 0., 1.);
		if (this->constantCallback) this->constantCallback(value);
		if (this->textFunction) this->setText(this->textFunction(value));
	}
}

void Slider::setPressed(bool pressed) {
	if (!pressed) {
		this->backward = false;
		this->forward = false;
		if (this->finalCallback) this->finalCallback(value);
	}
	Button::setPressed(pressed);
}

void Slider::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::A || keyPressed->scancode == sf::Keyboard::Scan::Left) {
			backward = true;
		} else if (keyPressed->scancode == sf::Keyboard::Scan::D || keyPressed->scancode == sf::Keyboard::Scan::Right) {
			forward = true;
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Space || keyPressed->scancode == sf::Keyboard::Scan::Enter) {
			setPressed(false);
		}
	} else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
		if (keyReleased->scancode == sf::Keyboard::Scan::A || keyReleased->scancode == sf::Keyboard::Scan::Left) {
			backward = false;
		} else if (keyReleased->scancode == sf::Keyboard::Scan::D || keyReleased->scancode == sf::Keyboard::Scan::Right) {
			forward = false;
		}
		if (!backward && !forward) {
			if (this->finalCallback) this->finalCallback(value);
		}
	}
	Button::handleEvent(event);
}