namespace Burst {
	struct Vector3 {
		float x;
		float y;
		float z;

		Vector3(float _x = 0.0, float _y = 0.0, float _z = 0.0) {
			x = _x;
			y = _y;
			z = _z;
		}

		float Length() {
			return sqrt((x*x) + (y*y) + (z*z));
		}

		Vector3 Normalize() {
			float L = this->Length();
			x /= L;
			y /= L;
			z /= L;
			return *this;
		}

		static float Dot(Vector3 a, Vector3 b) {
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}

		static Vector3 Cross(Vector3 a, Vector3 b) {
			return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
		}

		const Vector3& operator+(const Vector3& other) {
			return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		const Vector3& operator-(const Vector3& other) {
			return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
		}

		const Vector3& operator*(const Vector3& other) {
			return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
		}

		const Vector3& operator/(const Vector3& other) {
			return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
		}

		void operator+=(const Vector3& other) {
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
		}

		void operator-=(const Vector3& other) {
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
		}

		void operator*=(const Vector3& other) {
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
		}

		void operator/=(const Vector3& other) {
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
		}
		
		const Vector3& operator*(const float& other) {
			return Vector3(this->x * other, this->y * other, this->z * other);
		}

		const Vector3& operator/(const float& other) {
			return Vector3(this->x / other, this->y / other, this->z / other);
		}

		void operator*=(const float& other) {
			this->x *= other;
			this->y *= other;
			this->z *= other;
		}

		void operator/=(const float& other) {
			this->x /= other;
			this->y /= other;
			this->z /= other;
		}
	};

	const Vector3& operator+(const Vector3& a, const Vector3& b) {
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	const Vector3& operator-(const Vector3& a, const Vector3& b) {
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	const Vector3& operator*(const Vector3& a, const Vector3& b) {
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	const Vector3& operator/(const Vector3& a, const Vector3& b) {
		return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	const Vector3& operator*(const Vector3& a, const float& b) {
		return Vector3(a.x * b, a.y * b, a.z * b);
	}

	const Vector3& operator/(const Vector3& a, const float& b) {
		return Vector3(a.x / b, a.y / b, a.z / b);
	}

	std::ostream & operator<<(std::ostream& os, const Vector3& vc){
		os << "X: " << vc.x << " Y: " << vc.y << " Z: " << vc.z;
		return os;
	}
}