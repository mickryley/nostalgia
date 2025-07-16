#pragma once

namespace nostalgia::objects {

	struct Vector2D {
		float x, y;
		Vector2D() : x(0), y(0) {}
		Vector2D(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z; 
		}Vector2D operator+(const Vector2D& other) const {
			return Vector2D(x + other.x, y + other.y);
		}
		Vector2D operator-(const Vector2D& other) const {
			return Vector2D(x - other.x, y - other.y);
		}
		Vector2D operator*(float scalar) const {
			return Vector2D(x * scalar, y * scalar);
		}
	};

	struct Vector3D {
		float x, y, z;
		Vector3D() : x(0), y(0), z(0) {}
		Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3D operator+(const Vector3D& other) const {
			return Vector3D(x + other.x, y + other.y, z + other.z);
		}
		Vector3D operator-(const Vector3D& other) const {
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}
		Vector3D operator*(float scalar) const {
			return Vector3D(x * scalar, y * scalar, z * scalar);
		}
	};
}

static_assert(true, "End of objects header");