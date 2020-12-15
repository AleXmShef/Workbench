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

inline float deg2rad(float deg) {
	return (float)(deg * (M_PI / 180));
}

inline mathfu::quat RotateQuat(mathfu::quat& q, mathfu::vec3& vec, float scale = 1.0f) {
	mathfu::quat _q(0,
		vec.x * scale,
		vec.y * scale,
		vec.z * scale);

	_q = _q * q;

	q = mathfu::quat(
		q.scalar() + _q.scalar() * 0.5f,
		q.vector().x + _q.vector().x * 0.5f,
		q.vector().y + _q.vector().y * 0.5f,
		q.vector().z + _q.vector().z * 0.5f
	);

	q.Normalize();

	return q;
}
