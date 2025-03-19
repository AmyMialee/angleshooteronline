#pragma once

class Slider final : public Button {
	std::function<std::string(double)> textFunction;
	Consumer<double> constantCallback;
	Consumer<double> finalCallback;
	double value;
	bool forward;
	bool backward;
	
public:
	explicit Slider();
	void setTextFunction(const std::function<std::string(double)>& textFunction);
	void setConstantCallback(const Consumer<double>& textCallback);
	void setFinalCallback(const Consumer<double>& callback);
	void setValue(double value);
	void tick(float deltaTime) override;
	void setPressed(bool pressed) override;
	void handleEvent(const sf::Event& event) override;
};