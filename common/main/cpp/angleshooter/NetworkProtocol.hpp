#pragma once

class NetworkProtocol {
public:
	inline static PacketIdentifier* PING;
	inline static PacketIdentifier* PONG;
	inline static PacketIdentifier* ACK;
	inline static PacketIdentifier* CHAT_MESSAGE;

	static void initialize() {
		PING = PacketIdentifier::of(true, "PING");
		PONG = PacketIdentifier::of(true, "PONG");
		ACK = PacketIdentifier::of(false, "ACK");
		CHAT_MESSAGE = PacketIdentifier::of(true, "CHAT_MESSAGE");
	}
};