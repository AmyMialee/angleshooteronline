#pragma once
#include "Button.h"

class Slider final : public Button {
public:
	typedef std::shared_ptr<Slider> Pointer;
	typedef std::function<void(double)> Callback;

	explicit Slider();
	void setTextFunction(const std::function<std::string(double)>& textFunction);
	void setConstantCallback(const Callback& textCallback);
	void setFinalCallback(const Callback& callback);
	void setValue(double value);
	void tick(float deltaTime) override;
	void setPressed(bool pressed) override;
	void handleEvent(const sf::Event& event) override;

private:
	std::function<std::string(double)> textFunction;
	Callback constantCallback;
	Callback finalCallback;
	double value;
	bool forward;
	bool backward;
};