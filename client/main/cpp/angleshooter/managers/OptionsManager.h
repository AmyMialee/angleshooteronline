#pragma once

class OptionsManager final {
	std::string name = "Player";
	std::string ip = "127.0.0.1";
	std::uint8_t r = 0xFF;
	std::uint8_t g = 0xAA;
	std::uint8_t b = 0xAA;
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
	[[nodiscard]] std::string getIp() const;
	[[nodiscard]] sf::Color getColour() const;
	[[nodiscard]] double getMasterVolume() const;
	[[nodiscard]] double getMusicVolume() const;
	[[nodiscard]] double getSoundVolume() const;
	[[nodiscard]] bool isOnboarded() const;
	[[nodiscard]] int getFps();
	[[nodiscard]] double getTimePerFrame();
	[[nodiscard]] bool isDebugEnabled() const;
	void setName(const std::string& name);
	void setIp(const std::string& ip);
	void setColour(sf::Color colour);
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