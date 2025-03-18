#include "PreCompiledClient.h"
#include "ClientContext.h"

ClientContext* ClientContext::instance = nullptr;

ClientContext* ClientContext::get() {
	return instance;
}

void ClientContext::set(ClientContext* instance) {
	ClientContext::instance = instance;
}