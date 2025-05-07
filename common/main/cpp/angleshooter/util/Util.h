#pragma once

class Util {
public:
	static sf::Vector2f lerp(float delta, const sf::Vector2f& a, const sf::Vector2f& b);
	static double toRadians(double degrees);
	static double toDegrees(double radians);
	static void centre(sf::Sprite& sprite);
	static void centre(sf::Text& text);
	static int randomInt(int exclusiveMax);
	static float randomFloat();
	static float randomFloat(float min, float max);
	static float randomNormalFloat(float scale);
	static std::string toRoundedString(float value, int decimals = 2);
	static std::string toRoundedString(double value, int decimals = 2);
	static uint16_t swapBytes16(uint16_t inData);
	static uint32_t swapBytes32(uint32_t inData);
	static uint64_t swapBytes64(uint64_t inData);
};