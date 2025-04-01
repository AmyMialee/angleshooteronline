#pragma once

class NetworkProtocol {
public:
	inline const static auto S2C_ANNOUNCE_JOIN = Identifier("S2C_ANNOUNCE_JOIN");
	inline const static auto S2C_ANNOUNCE_LEAVE = Identifier("S2C_ANNOUNCE_LEAVE");
	inline const static auto S2C_INITIAL_SETUP = Identifier("S2C_INITIAL_SETUP");
	inline const static auto S2C_BROADCAST_MESSAGE = Identifier("S2C_BROADCAST_MESSAGE");
	inline const static auto S2C_PLAY_MUSIC = Identifier("S2C_PLAY_MUSIC");
	inline const static auto S2C_PLAY_SOUND = Identifier("S2C_PLAY_SOUND");
	inline const static auto S2C_PLAY_SOUND_3D = Identifier("S2C_PLAY_SOUND_3D");
	inline const static auto S2C_LOAD_MAP = Identifier("S2C_LOAD_MAP");

	inline const static auto C2S_JOIN = Identifier("C2S_JOIN");
	inline const static auto C2S_CHANGE_NAME = Identifier("C2S_CHANGE_NAME");
	inline const static auto C2S_SEND_MESSAGE = Identifier("C2S_SEND_MESSAGE");
	inline const static auto C2S_QUIT = Identifier("C2S_QUIT");

	inline const static auto PACKET_QUESTION = Identifier("PACKET_QUESTION");
	inline const static auto PACKET_TRANSLATION = Identifier("PACKET_TRANSLATION");
};