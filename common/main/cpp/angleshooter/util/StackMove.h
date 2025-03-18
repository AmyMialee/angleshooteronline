#pragma once

enum class StackMove : std::uint8_t {
	PUSH,
	POP,
	CLEAR
};

inline const char* toString(StackMove move) {
	switch (move) {
		case StackMove::PUSH: return "PUSH";
		case StackMove::POP: return "POP";
		case StackMove::CLEAR: return "CLEAR";
	}
	Logger::errorOnce("Invalid StackMove used for toString");
	return "UNKNOWN";
}