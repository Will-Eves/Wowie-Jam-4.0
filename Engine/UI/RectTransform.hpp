#pragma once

namespace Burst {
	struct RectTransform : ECS::Component {
		Vector2 position;
		float rotation = 0.0f;
		Vector2 scale = Vector2(100.0f, 100.0f);

		AnchorPoint anchorPoint;

		RectTransform(AnchorPoint _anchorPoint=Anchors::TopLeft) {
			anchorPoint = _anchorPoint;
		}

		virtual void Update() {
			UIManager::currentAnchor = anchorPoint;

			parent->transform.position = Vector3(position.x + anchorPoint.point.x + scale.x / 2.0f * anchorPoint.direction.x, position.y + anchorPoint.point.y + scale.y / 2.0f * anchorPoint.direction.y, 1.0f);
			parent->transform.rotation = Vector3(0.0f, 0.0f, rotation);
			parent->transform.scale = Vector3(scale.x, scale.y, 1.0f);

			UIManager::currentMin = Vector2(parent->transform.position.x - scale.x / 2.0f, parent->transform.position.y - scale.y / 2.0f);
			UIManager::currentMax = Vector2(parent->transform.position.x + scale.x / 2.0f, parent->transform.position.y + scale.y / 2.0f);
		}
	};
}