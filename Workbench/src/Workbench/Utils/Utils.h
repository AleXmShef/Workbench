#pragma once
#include "wbpch.h"

//Utility & helper macros
#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_custom_deleter : public std::shared_ptr<T> {
	explicit shared_ptr_with_custom_deleter(T* t = nullptr) : std::shared_ptr<T>(t, D()) {};

	void reset(T* t = nullptr) {
		std::shared_ptr<T>::reset(t, D()); 
	}
};
