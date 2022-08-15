#pragma once

namespace Burst {
	struct PanelRenderer : ECS::Component {
		RenderObject renderObject;

		Color color;

		float radius = 10.0f;

		PanelRenderer() {
			this->renderObject = RenderObject(&UIManager::panelMesh, &UIManager::uiShader);
		}

		virtual void UIRender() {
			UIManager::uiShader.SendMatrix4("modelMatrix", this->parent->transform.GetModelMatrix());

			UIManager::uiShader.SendFloat4("ui.color", color.r, color.g, color.b, color.a);
			UIManager::uiShader.SendInt1("ui.hasImage", 0);
			UIManager::uiShader.SendInt1("ui.isText", 0);
			UIManager::uiShader.SendFloat2("ui.minBound", UIManager::currentMin.x, UIManager::currentMin.y);
			UIManager::uiShader.SendFloat2("ui.maxBound", UIManager::currentMax.x, UIManager::currentMax.y);
			UIManager::uiShader.SendFloat1("ui.radius", this->radius);

			renderObject.Render();
		}
	};
}