#pragma once

class StateManager;

class State {
public:
	typedef std::unique_ptr<State> Pointer;

	State() = default;
	virtual ~State() = default;
	State(const State&) = delete;
	State& operator=(const State&) = delete;
	virtual void init() = 0;
	virtual void destroy();
	virtual void loadAssets() = 0;
	virtual void render(float deltaTime) = 0;
	/**
	 * @return Should continue drawing States
	 */
	[[nodiscard]] virtual bool shouldRenderNextState() const = 0;
	/**
	 * @return Should continue ticking States
	 */
	virtual bool tick(float deltaTime) = 0;
	/**
	 * @return Should continue passing events down to States
	 */
	virtual bool handleEvent(const sf::Event& event) = 0;
	virtual Identifier getStateId() = 0;

protected:
	void requestStackPush(const Identifier& id);
	void requestStackPop();
	void requestStackClear();
};