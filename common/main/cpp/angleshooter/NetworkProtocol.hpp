#pragma once

class NetworkProtocol {
public:
	inline static PacketIdentifier* PING;
	inline static PacketIdentifier* PONG;
	inline static PacketIdentifier* ACK;

	static void initialize() {
		PING = PacketIdentifier::of(true, "PING");
		PONG = PacketIdentifier::of(true, "PONG");
		ACK = PacketIdentifier::of(false, "ACK");
	}
};