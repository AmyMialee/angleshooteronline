#include "PreCompiledServer.h"
#include "PlayerHandler.h"

PlayerHandler::PlayerHandler() : ready(false), timedOut(false) {
	socket.setBlocking(false);
}