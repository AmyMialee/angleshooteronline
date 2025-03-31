#pragma once

template <typename T>
class Singleton {
public:
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;

	static T& get() {
		static T instance;
		return instance;
	}
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
};