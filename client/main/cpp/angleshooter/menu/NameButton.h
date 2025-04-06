#pragma once

class NameButton : public Button {
	std::function<std::string(std::string)> textFunction;
	std::string name = OptionsManager::get().getName();

public:
	explicit NameButton();
	void setTextFunction(const std::function<std::string(std::string)>& textFunction);
	void handleEvent(const sf::Event& event) override;
};
