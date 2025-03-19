#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Singleton.h"

template<typename T> T& Singleton<T>::get() {
	static T instance;
	return instance;
}