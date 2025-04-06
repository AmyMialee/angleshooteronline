#pragma once

class IpButton : public Button {
	std::function<std::string(std::string)> textFunction;
	std::string ipAddress = OptionsManager::get().getIp();

public:
	explicit IpButton();
	void setTextFunction(const std::function<std::string(std::string)>& textFunction);
	void handleEvent(const sf::Event& event) override;
};
