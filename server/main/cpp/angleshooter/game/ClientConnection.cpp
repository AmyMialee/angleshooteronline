#include "PreCompiledServer.h"
#include "ClientConnection.h"

ClientConnection::ClientConnection() { socket.setBlocking(false); }