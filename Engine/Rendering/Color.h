namespace Burst {
	struct Color {
		float r = 255.0f;
		float g = 255.0f;
		float b = 255.0f;
		float a = 1.0f;

		Color(float _r = 255.0f, float _g = 255.0f, float _b = 255.0f, float _a = 1.0f) {
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}
	};
}