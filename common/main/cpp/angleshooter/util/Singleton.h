#pragma once

template <typename T>
class Singleton {
public:
	static T& get();
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
};