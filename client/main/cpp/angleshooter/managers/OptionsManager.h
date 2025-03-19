#pragma once

class OptionsManager final : public Singleton<OptionsManager> {
	double masterVolume = 1.;
	double musicVolume = 0.;
	double soundVolume = 0.;
	int framesPerSecond = 144;
	double timePerFrame = 1. / framesPerSecond;
	bool onboarded = false;
	bool debug = false;
	OptionsManager() = default;

public:
	[[nodiscard]] double getMasterVolume() const;
	[[nodiscard]] double getMusicVolume() const;
	[[nodiscard]] double getSoundVolume() const;
	[[nodiscard]] bool isOnboarded() const;
	[[nodiscard]] int getFps();
	[[nodiscard]] double getTimePerFrame();
	[[nodiscard]] bool isDebugEnabled() const;
	void setFps(int fps);
	void setMasterVolume(double volume);
	void setMusicVolume(double volume);
	void setSoundVolume(double volume);
	void setOnboarded(bool onboarded);
	void setDebugEnabled(bool enabled);
	void saveToFile();
	void loadFromFile();
};