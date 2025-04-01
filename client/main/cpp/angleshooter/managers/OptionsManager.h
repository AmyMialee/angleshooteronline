#pragma once

class OptionsManager final {
	std::string name = "Player";
	int colour = 0xFFFFFFFF;
	double masterVolume = 1.;
	double musicVolume = 0.;
	double soundVolume = 0.;
	int framesPerSecond = 144;
	double timePerFrame = 1. / framesPerSecond;
	bool onboarded = false;
	bool debug = false;

protected:
	OptionsManager() = default;
	~OptionsManager() = default;

public:
	OptionsManager(const OptionsManager&) = delete;
	void operator=(const OptionsManager&) = delete;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] int getColour() const;
	[[nodiscard]] double getMasterVolume() const;
	[[nodiscard]] double getMusicVolume() const;
	[[nodiscard]] double getSoundVolume() const;
	[[nodiscard]] bool isOnboarded() const;
	[[nodiscard]] int getFps();
	[[nodiscard]] double getTimePerFrame();
	[[nodiscard]] bool isDebugEnabled() const;
	void setName(const std::string& name);
	void setColour(int colour);
	void setMasterVolume(double volume);
	void setMusicVolume(double volume);
	void setSoundVolume(double volume);
	void setOnboarded(bool onboarded);
	void setFps(int fps);
	void setDebugEnabled(bool enabled);
	void saveToFile();
	void loadFromFile();

	static OptionsManager& get() {
		static OptionsManager instance;
		return instance;
	}
};