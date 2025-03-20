#pragma once

template <typename T>
class Consumer : public std::function<void(T)> {};