#pragma once

template <typename T>
class InstancedSingleton {
public:
	static T& getInstance() {
		static T instance;
		return instance;
	}
	InstancedSingleton(const InstancedSingleton&) = delete;
	InstancedSingleton& operator=(const InstancedSingleton&) = delete;
protected:
	InstancedSingleton() = default;
	virtual ~InstancedSingleton() = default;
};