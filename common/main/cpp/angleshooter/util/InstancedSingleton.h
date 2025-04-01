#pragma once

template <typename T>
class InstancedSingleton {
public:
	InstancedSingleton(const InstancedSingleton&) = delete;
	InstancedSingleton& operator=(const InstancedSingleton&) = delete;

	static T& getInstance() {
		static T instance;
		return instance;
	}
protected:
	InstancedSingleton() = default;
	virtual ~InstancedSingleton() = default;
};