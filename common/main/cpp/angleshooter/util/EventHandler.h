#pragma once

class EventHandler {
protected:
	~EventHandler() = default;

public:
	virtual void handleEvent(const sf::Event& event) = 0;
};