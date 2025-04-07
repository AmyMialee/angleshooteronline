#pragma once

class ScoreManager {

protected:
	ScoreManager();
	~ScoreManager() = default;
public:

	static ScoreManager& get() {
		static ScoreManager instance;
		return instance;
	}
};
