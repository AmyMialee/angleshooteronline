#pragma once

class GameObject : public sf::Transformable {
public:
	typedef std::shared_ptr<GameObject> Pointer;
	typedef std::pair<GameObject*, GameObject*> Pair;

	GameObject();
	~GameObject() override = default;
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	void baseTick(float deltaTime);
	void baseRender(float deltaTime);
	virtual void renderHitbox(float deltaTime);
	[[nodiscard]] float getX() const;
	[[nodiscard]] float getY() const;
	[[nodiscard]] uint32_t getId() const;
	void setId(uint32_t id);
	[[nodiscard]] virtual sf::FloatRect getBoundingBox() const;
	[[nodiscard]] bool isColliding(const GameObject& other) const;
	[[nodiscard]] float getDistanceTo(const GameObject& other) const;
	[[nodiscard]] bool isInWall(sf::Vector2f pos) const;
	[[nodiscard]] virtual bool isMarkedForRemoval() const;
	virtual void onCollision(GameObject& other);
	bool operator==(const GameObject& other) const;
	bool operator!=(const GameObject& other) const;

protected:
	virtual void tick(float deltaTime);
	virtual void render(float deltaTime);

private:
	sf::RectangleShape hitbox;
	uint32_t id;
};