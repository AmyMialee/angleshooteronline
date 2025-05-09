#pragma once

class StateManager final {
	struct PendingChange {
		explicit PendingChange(StackMove action, Identifier id = Identifier::empty);
		StackMove action;
		Identifier id;
	};

	std::map<Identifier, std::function<State::Pointer()>> stateMap;
	std::vector<std::pair<State::Pointer, bool>> stack;
	std::vector<PendingChange> pending;
	State::Pointer create(const Identifier& id);
	void applyChanges();

protected:
	StateManager();
	~StateManager() = default;

public:
	template<typename T> void registerState(const Identifier& id);
	void loadAssets();
	void tick(float deltaTime);
	void render(float deltaTime);
	void handleEvent(const sf::Event& event);
	[[nodiscard]] Identifier getStateId() const;

	void push(const Identifier& id);
	void pop();
	void clear();
	[[nodiscard]] bool isEmpty() const;

	static StateManager& get() {
		static StateManager instance;
		return instance;
	}
};

template<typename T>
void StateManager::registerState(const Identifier& id) {
	stateMap[id] = [this] {
		return State::Pointer(new T());
	};
}