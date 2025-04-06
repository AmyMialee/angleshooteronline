#pragma once

class NetworkProtocol {
public:
	inline const static auto S2C_INITIAL_SETUP = PacketIdentifier("S2C_INITIAL_SETUP");
	inline const static auto S2C_BROADCAST_MESSAGE = PacketIdentifier("S2C_BROADCAST_MESSAGE");
	inline const static auto S2C_PLAY_MUSIC = PacketIdentifier("S2C_PLAY_MUSIC");
	inline const static auto S2C_PLAY_SOUND = PacketIdentifier("S2C_PLAY_SOUND");
	inline const static auto S2C_PLAY_SOUND_3D = PacketIdentifier("S2C_PLAY_SOUND_3D");
	inline const static auto S2C_LOAD_MAP = PacketIdentifier("S2C_LOAD_MAP");
	inline const static auto S2C_SPAWN_PLAYER = PacketIdentifier("S2C_SPAWN_PLAYER");
	inline const static auto S2C_SPAWN_BULLET = PacketIdentifier("S2C_SPAWN_BULLET");
	inline const static auto S2C_PLAYER_INPUT = PacketIdentifier("S2C_PLAYER_INPUT");
	inline const static auto S2C_PLAYER_POSITION_SYNC = PacketIdentifier("S2C_PLAYER_POSITION_SYNC");
	inline const static auto S2C_BULLET_CHARGE = PacketIdentifier("S2C_BULLET_CHARGE");
	inline const static auto S2C_HEALTH = PacketIdentifier("S2C_HEALTH");
	inline const static auto S2C_DEATH = PacketIdentifier("S2C_DEATH");
	inline const static auto S2C_TELEPORT = PacketIdentifier("S2C_TELEPORT");
	inline const static auto S2C_REMOVE_OBJECT = PacketIdentifier("S2C_REMOVE_OBJECT");

	inline const static auto C2S_JOIN = PacketIdentifier("C2S_JOIN");
	inline const static auto C2S_CHANGE_NAME = PacketIdentifier("C2S_CHANGE_NAME");
	inline const static auto C2S_SEND_MESSAGE = PacketIdentifier("C2S_SEND_MESSAGE");
	inline const static auto C2S_QUIT = PacketIdentifier("C2S_QUIT");
	inline const static auto C2S_PLAYER_INPUT = PacketIdentifier("C2S_PLAYER_INPUT");
	inline const static auto C2S_PLAYER_POSITION_SYNC = PacketIdentifier("C2S_PLAYER_POSITION_SYNC");

	inline const static auto PACKET_QUESTION = PacketIdentifier("PACKET_QUESTION");
	inline const static auto PACKET_TRANSLATION = PacketIdentifier("PACKET_TRANSLATION");
};
