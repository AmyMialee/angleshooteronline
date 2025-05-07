#pragma once

class AngleShooterCommon {
public:
	static constexpr auto PORT = 62565;
	static constexpr auto TIME_PER_TICK = 1. / 60.;
	static constexpr auto TIMEOUT = 30.f;
};