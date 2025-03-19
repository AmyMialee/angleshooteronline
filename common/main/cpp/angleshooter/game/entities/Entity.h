#pragma once

class Entity : public sf::Transformable {
public:
	typedef std::shared_ptr<Entity> Pointer;
	typedef std::pair<Entity*, Entity*> Pair;

	Entity();
	~Entity() override = default;
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	void baseTick(float deltaTime);
	[[nodiscard]] float getX() const;
	[[nodiscard]] float getY() const;
	[[nodiscard]] uint32_t getId() const;
	void setId(uint32_t id);
	[[nodiscard]] virtual sf::FloatRect getBoundingBox() const;
	[[nodiscard]] bool isColliding(const Entity& other) const;
	[[nodiscard]] float getDistanceTo(const Entity& other) const;
	[[nodiscard]] bool isInWall(sf::Vector2f pos) const;
	[[nodiscard]] virtual bool isMarkedForRemoval() const;
	virtual void onCollision(Entity& other);
	virtual const Identifier& getEntityType() const = 0;
	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;
	[[nodiscard]] sf::Vector2f getVelocity() const;
	void addVelocity(sf::Vector2f velocity);
	void setVelocity(sf::Vector2f velocity);
	void addPosition(sf::Vector2f change);
	[[nodiscard]] float getDrag() const;
	void setDrag(float drag);

protected:
	virtual void tick(float deltaTime);
	virtual void render(float deltaTime);
	virtual void onWallCollision();

private:
	sf::RectangleShape hitbox;
	uint32_t id;
	sf::Vector2f velocity;
	float drag = 0.01f;
};