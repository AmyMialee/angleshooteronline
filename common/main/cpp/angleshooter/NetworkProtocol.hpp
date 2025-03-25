#pragma once

class NetworkProtocol {
public:
	inline const static auto S2C_BROADCAST_MESSAGE = Identifier("S2C_BROADCAST_MESSAGE");
	inline const static auto S2C_INITIAL_STATE = Identifier("S2C_INITIAL_STATE");
	inline const static auto S2C_PLAYER_EVENT = Identifier("S2C_PLAYER_EVENT");
	inline const static auto S2C_PLAYER_REALTIME_CHANGE = Identifier("S2C_PLAYER_REALTIME_CHANGE");
	inline const static auto S2C_PLAYER_CONNECT = Identifier("S2C_PLAYER_CONNECT");
	inline const static auto S2C_PLAYER_DISCONNECT = Identifier("S2C_PLAYER_DISCONNECT");
	inline const static auto S2C_SPAWN_ENEMY = Identifier("S2C_SPAWN_ENEMY");
	inline const static auto S2C_SPAWN_PICKUP = Identifier("S2C_SPAWN_PICKUP");
	inline const static auto S2C_SPAWN_SELF = Identifier("S2C_SPAWN_SELF");
	inline const static auto S2C_UPDATE_CLIENT_STATE = Identifier("S2C_UPDATE_CLIENT_STATE");
	inline const static auto S2C_MISSION_SUCCESS = Identifier("S2C_MISSION_SUCCESS");

	inline const static auto C2S_PLAYER_EVENT = Identifier("C2S_PLAYER_EVENT");
	inline const static auto C2S_PLAYER_REALTIME_CHANGE = Identifier("C2S_PLAYER_REALTIME_CHANGE");
	inline const static auto C2S_STATE_UPDATE = Identifier("C2S_STATE_UPDATE");
	inline const static auto C2S_GAME_EVENT = Identifier("C2S_GAME_EVENT");
	inline const static auto C2S_QUIT = Identifier("C2S_QUIT");
};