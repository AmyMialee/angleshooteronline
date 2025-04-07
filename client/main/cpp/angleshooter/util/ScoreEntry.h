#pragma once

class ScoreEntry {
public:
	std::string name;
	sf::Color colour;
	uint16_t score;
	float yLastCurrent;
	float yCurrent;
	float yTarget;
	ScoreEntry(std::string name, sf::Color colour, uint16_t score, float yLastCurrent, float yCurrent) : name(std::move(name)), colour(colour), score(score), yLastCurrent(yLastCurrent), yCurrent(yCurrent), yTarget(yCurrent) {}
};