#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PlayerData.h"

PlayerData::PlayerData() : score(0) {}

int PlayerData::getScore() const {
	return this->score;
}

void PlayerData::setScore(int score) {
	this->score = score;
}

void PlayerData::addScore(int score) {
	this->score += score;
}

void PlayerData::resetScore() {
	this->score = 0;
}