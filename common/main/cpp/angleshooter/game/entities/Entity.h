#pragma once

class Entity : public GameObject {
	sf::Vector2f velocity;
	float drag = 0.01f;

protected:
	virtual void onWallCollision();

public:
	[[nodiscard]] sf::Vector2f getVelocity() const;
	void addVelocity(sf::Vector2f velocity);
	void setVelocity(sf::Vector2f velocity);
	void addPosition(sf::Vector2f change);
	void tick(float deltaTime) override;
	[[nodiscard]] float getDrag() const;
	void setDrag(float drag);
	void renderHitbox(float deltaTime) override;
};