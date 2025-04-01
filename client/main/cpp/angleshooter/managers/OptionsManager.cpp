#include "PreCompiledClient.h"
#include "OptionsManager.h"

namespace {
	auto fileName("run/options.json");
}

void OptionsManager::saveToFile() {
	nlohmann::json json;
	json["name"] = name;
	json["colour"] = colour;
	json["masterVolume"] = masterVolume;
	json["musicVolume"] = musicVolume;
	json["soundVolume"] = soundVolume;
	json["onboarded"] = onboarded;
	json["fps"] = framesPerSecond;
	for (const auto& keybinding : InputManager::get().getKeybindings()) {
		json["keybindings"][keybinding->getId()->toString()] = keybinding->getKey();
	}
	std::filesystem::create_directories("run");
	if (std::ofstream file(fileName); file.is_open()) {
		file << json.dump(4);
		file.close();
		Logger::debug("Saved options to file");
	} else {
		Logger::errorOnce("Failed to save options to file");
	}
}

void OptionsManager::loadFromFile() {
	if (std::ifstream file(fileName); file.is_open()) {
		Logger::debug("Loading options from file");
		nlohmann::json json;
		file >> json;
		file.close();
		try {
			name = json.value("name", "Player");
			colour = json.value("colour", 0xFFFFFFFF);
			masterVolume = json.value("masterVolume", 100.);
			musicVolume = json.value("musicVolume", 0.);
			soundVolume = json.value("soundVolume", 0.);
			onboarded = json.value("onboarded", false);
			framesPerSecond = json.value("fps", 144);
			timePerFrame = 1. / framesPerSecond;
			for (const auto& keybinding : InputManager::get().getKeybindings()) {
				keybinding->loadBinding(json.value("keybindings", nlohmann::json()).value(keybinding->getId()->toString(), keybinding->getDefaultKey()));
			}
		} catch (const nlohmann::json::exception& e) {
			Logger::error("Failed to load options from file: " + std::string(e.what()));
			saveToFile();
		}
		Logger::debug("Options loaded");
	} else {
		saveToFile();
		Logger::info("Creating new options file");
	}
}

std::string OptionsManager::getName() const {
	return this->name;
}

int OptionsManager::getColour() const {
	return this->colour;
}

double OptionsManager::getMasterVolume() const {
	return this->masterVolume;
}

double OptionsManager::getMusicVolume() const {
	return this->musicVolume;
}

double OptionsManager::getSoundVolume() const {
	return this->soundVolume;
}

bool OptionsManager::isOnboarded() const {
	return this->onboarded;
}

int OptionsManager::getFps() {
	return this->framesPerSecond;
}

double OptionsManager::getTimePerFrame() {
	return this->timePerFrame;
}

bool OptionsManager::isDebugEnabled() const {
	return this->debug;
}

void OptionsManager::setName(const std::string& name) {
	this->name = name;
	saveToFile();
}

void OptionsManager::setColour(int colour) {
	this->colour = colour;
}

void OptionsManager::setFps(int fps) {
	this->framesPerSecond = fps;
	this->timePerFrame = 1. / fps;
	saveToFile();
}

void OptionsManager::setMasterVolume(double volume) {
	this->masterVolume = volume;
	saveToFile();
}

void OptionsManager::setMusicVolume(double volume) {
	this->musicVolume = volume;
	saveToFile();
}

void OptionsManager::setSoundVolume(double volume) {
	this->soundVolume = volume;
	saveToFile();
}

void OptionsManager::setOnboarded(bool onboarded) {
	this->onboarded = onboarded;
	saveToFile();
}

void OptionsManager::setDebugEnabled(bool enabled) {
	this->debug = enabled;
}