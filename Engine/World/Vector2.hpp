namespace Burst {
	struct Vector2 {
		float x;
		float y;

		Vector2(float _x = 0.0, float _y = 0.0) {
			x = _x;
			y = _y;
		}

		float Length() {
			return sqrt((x * x) + (y * y));
		}

		Vector2 Normalize() {
			float L = this->Length();
			x /= L;
			y /= L;
			return *this;
		}

		static float Dot(Vector2 a, Vector2 b) {
			return a.x*b.x + a.y*b.y;
		}

		const Vector2& operator+(const Vector2& other) {
			return Vector2(this->x + other.x, this->y + other.y);
		}

		const Vector2& operator-(const Vector2& other) {
			return Vector2(this->x - other.x, this->y - other.y);
		}

		const Vector2& operator*(const Vector2& other) {
			return Vector2(this->x * other.x, this->y * other.y);
		}

		const Vector2& operator/(const Vector2& other) {
			return Vector2(this->x / other.x, this->y / other.y);
		}

		void operator+=(const Vector2& other) {
			this->x += other.x;
			this->y += other.y;
		}

		void operator-=(const Vector2& other) {
			this->x -= other.x;
			this->y -= other.y;
		}

		void operator*=(const Vector2& other) {
			this->x *= other.x;
			this->y *= other.y;
		}

		void operator/=(const Vector2& other) {
			this->x /= other.x;
			this->y /= other.y;
		}

		const Vector2& operator*(const float& other) {
			return Vector2(this->x * other, this->y * other);
		}

		const Vector2& operator/(const float& other) {
			return Vector2(this->x / other, this->y / other);
		}

		void operator*=(const float& other) {
			this->x *= other;
			this->y *= other;
		}

		void operator/=(const float& other) {
			this->x /= other;
			this->y /= other;
		}
	};

	const Vector2& operator+(const Vector2& a, const Vector2& b) {
		return Vector2(a.x + b.x, a.y + b.y);
	}

	const Vector2& operator-(const Vector2& a, const Vector2& b) {
		return Vector2(a.x - b.x, a.y - b.y);
	}

	const Vector2& operator*(const Vector2& a, const Vector2& b) {
		return Vector2(a.x * b.x, a.y * b.y);
	}

	const Vector2& operator/(const Vector2& a, const Vector2& b) {
		return Vector2(a.x / b.x, a.y / b.y);
	}

	const Vector2& operator*(const Vector2& a, const float& b) {
		return Vector2(a.x * b, a.y * b);
	}

	const Vector2& operator/(const Vector2& a, const float& b) {
		return Vector2(a.x / b, a.y / b);
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vc) {
		os << "X: " << vc.x << " Y : " << vc.y;
		return os;
	}
}