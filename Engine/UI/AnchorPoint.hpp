#pragma once

namespace Burst {
	struct AnchorPoint {
		Vector2 point;

		Vector2 direction;

		AnchorPoint(Vector2 _point=Vector2(0.0f, 0.0f), Vector2 _direction=Vector2(0.0f, 0.0f)) {
			point = _point;
			direction = _direction;
		}
	};

	namespace Anchors {
		static AnchorPoint TopLeft = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(1.0f, -1.0f));
		static AnchorPoint TopMiddle = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(0.0f, -1.0f));
		static AnchorPoint TopRight = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(-1.0f, -1.0f));

		static AnchorPoint MiddleLeft = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f));
		static AnchorPoint MiddleMiddle = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f));
		static AnchorPoint MiddleRight = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(-1.0f, 0.0f));

		static AnchorPoint BottomLeft = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
		static AnchorPoint BottomMiddle = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(0.0f, 1.0f));
		static AnchorPoint BottomRight = AnchorPoint(Vector2(0.0f, 0.0f), Vector2(-1.0f, 1.0f));
	}
}