#pragma once

template <typename T>
class Consumer : std::function<void(T)> {};