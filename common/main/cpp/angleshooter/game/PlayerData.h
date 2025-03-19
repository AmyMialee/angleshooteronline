#pragma once

class PlayerData {
	int score;

public:
	PlayerData();
	[[nodiscard]] int getScore() const;
	void setScore(int score);
	void addScore(int score);
	void resetScore();
};
