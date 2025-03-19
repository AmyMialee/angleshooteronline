#include "PreCompiledClient.h"
#include "State.h"

void State::destroy() {}

void State::requestStackPush(const Identifier& id) {
	StateManager::get().push(id);
}

void State::requestStackPop() {
	StateManager::get().pop();
}

void State::requestStackClear() {
	StateManager::get().clear();
}