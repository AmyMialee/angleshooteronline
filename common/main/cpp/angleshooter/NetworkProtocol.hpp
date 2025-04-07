#pragma once

class NetworkProtocol {
public:
	inline static PacketIdentifier S2C_INITIAL_SETUP;
	inline static PacketIdentifier S2C_BROADCAST_MESSAGE;
	inline static PacketIdentifier S2C_PLAY_MUSIC;
	inline static PacketIdentifier S2C_PLAY_SOUND;
	inline static PacketIdentifier S2C_PLAY_SOUND_3D;
	inline static PacketIdentifier S2C_LOAD_MAP;
	inline static PacketIdentifier S2C_SPAWN_PLAYER;
	inline static PacketIdentifier S2C_SPAWN_BULLET;
	inline static PacketIdentifier S2C_PLAYER_INPUT;
	inline static PacketIdentifier S2C_PLAYER_POSITION_SYNC;
	inline static PacketIdentifier S2C_BULLET_CHARGE;
	inline static PacketIdentifier S2C_HEALTH;
	inline static PacketIdentifier S2C_DEATH;
	inline static PacketIdentifier S2C_TELEPORT;
	inline static PacketIdentifier S2C_REMOVE_OBJECT;
	inline static PacketIdentifier S2C_UPDATE_SCORE;

	inline static PacketIdentifier C2S_JOIN;
	inline static PacketIdentifier C2S_CHANGE_NAME;
	inline static PacketIdentifier C2S_SEND_MESSAGE;
	inline static PacketIdentifier C2S_QUIT;
	inline static PacketIdentifier C2S_PLAYER_INPUT;
	inline static PacketIdentifier C2S_PLAYER_POSITION_SYNC;

	inline static PacketIdentifier PACKET_QUESTION;
	inline static PacketIdentifier PACKET_TRANSLATION;

	static void initialize() {
		S2C_INITIAL_SETUP = PacketIdentifier("S2C_INITIAL_SETUP");
		S2C_BROADCAST_MESSAGE = PacketIdentifier("S2C_BROADCAST_MESSAGE");
		S2C_PLAY_MUSIC = PacketIdentifier("S2C_PLAY_MUSIC");
		S2C_PLAY_SOUND = PacketIdentifier("S2C_PLAY_SOUND");
		S2C_PLAY_SOUND_3D = PacketIdentifier("S2C_PLAY_SOUND_3D");
		S2C_LOAD_MAP = PacketIdentifier("S2C_LOAD_MAP");
		S2C_SPAWN_PLAYER = PacketIdentifier("S2C_SPAWN_PLAYER");
		S2C_SPAWN_BULLET = PacketIdentifier("S2C_SPAWN_BULLET");
		S2C_PLAYER_INPUT = PacketIdentifier("S2C_PLAYER_INPUT");
		S2C_PLAYER_POSITION_SYNC = PacketIdentifier("S2C_PLAYER_POSITION_SYNC");
		S2C_BULLET_CHARGE = PacketIdentifier("S2C_BULLET_CHARGE");
		S2C_HEALTH = PacketIdentifier("S2C_HEALTH");
		S2C_DEATH = PacketIdentifier("S2C_DEATH");
		S2C_TELEPORT = PacketIdentifier("S2C_TELEPORT");
		S2C_REMOVE_OBJECT = PacketIdentifier("S2C_REMOVE_OBJECT");
		S2C_UPDATE_SCORE = PacketIdentifier("S2C_UPDATE_SCORE");

		C2S_JOIN = PacketIdentifier("C2S_JOIN");
		C2S_CHANGE_NAME = PacketIdentifier("C2S_CHANGE_NAME");
		C2S_SEND_MESSAGE = PacketIdentifier("C2S_SEND_MESSAGE");
		C2S_QUIT = PacketIdentifier("C2S_QUIT");
		C2S_PLAYER_INPUT = PacketIdentifier("C2S_PLAYER_INPUT");
		C2S_PLAYER_POSITION_SYNC = PacketIdentifier("C2S_PLAYER_POSITION_SYNC");

		PACKET_QUESTION = PacketIdentifier("PACKET_QUESTION");
		PACKET_TRANSLATION = PacketIdentifier("PACKET_TRANSLATION");
	}
};