#include "PreCompiledClient.h"
#include "State.h"

void State::destroy() {}

void State::requestStackPush(const Identifier& id) {
	ClientContext::get()->getStateManager()->push(id);
}

void State::requestStackPop() {
	ClientContext::get()->getStateManager()->pop();
}

void State::requestStackClear() {
	ClientContext::get()->getStateManager()->clear();
}