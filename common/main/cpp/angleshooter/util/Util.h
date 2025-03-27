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
	static std::string toRoundedString(float value, int decimals = 0);
	static std::string toRoundedString(double value, int decimals = 0);
	static std::string toString(sf::TcpSocket::Status);
};