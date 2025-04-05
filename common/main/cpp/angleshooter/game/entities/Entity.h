#pragma once
class World;

class Entity : public sf::Transformable {
	const uint16_t id;
	sf::Vector2f velocity;
	float drag = 0.01f;
	
protected:
	World* world;
	
public:
	explicit Entity(uint16_t id, World* world);
	~Entity() override = default;
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;
	
	[[nodiscard]] float getX() const;
	[[nodiscard]] float getY() const;
	[[nodiscard]] uint16_t getId() const;
	[[nodiscard]] sf::Vector2f getVelocity() const;
	[[nodiscard]] float getDrag() const;
	[[nodiscard]] sf::FloatRect getBoundingBox() const;
	[[nodiscard]] bool isInWall(sf::Vector2f pos) const;
	[[nodiscard]] bool isColliding(const Entity& other) const;
	[[nodiscard]] float getDistanceTo(const Entity& other) const;
	void addVelocity(sf::Vector2f velocity);
	void setVelocity(sf::Vector2f velocity);
	void addPosition(sf::Vector2f change);
	void setDrag(float drag);
	
	virtual void tick(float deltaTime);
	virtual void onWallCollision();
	virtual void onCollision(Entity& other);

	virtual void writeToPacket(sf::Packet& packet) const;
	virtual void readFromPacket(sf::Packet& packet);
	
	[[nodiscard]] virtual bool isMarkedForRemoval() const = 0;
	[[nodiscard]] virtual const Identifier& getEntityType() const = 0;
};