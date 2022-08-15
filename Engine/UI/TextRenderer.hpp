#pragma once

namespace Burst {
	struct TextRenderer : ECS::Component {
		Color color;

		std::string text = "Text";
        float fontSize = 50.0f;

        Font* font = &FontManager::defaultFont;

        VertexObject vo;
        IndexObject io;

        TextRenderer() {
            vo = VertexObject({ 0.0f }, UIManager::uiShader.GetAttribObject(), GL_DYNAMIC_DRAW);

            std::vector<unsigned int> indices = {
                0, 2, 1,
                0, 3, 2
            };

            io = IndexObject(indices);
        }

		virtual void UIRender() {
            float xs = this->parent->transform.scale.x;
            this->parent->transform.scale.x = this->parent->transform.scale.y;
			UIManager::uiShader.SendMatrix4("modelMatrix", this->parent->transform.GetModelMatrix());
            this->parent->transform.scale.x = xs;

            UIManager::uiShader.SendFloat4("ui.color", color.r, color.g, color.b, color.a);
            UIManager::uiShader.SendInt1("ui.hasImage", 1);
            UIManager::uiShader.SendInt1("ui.isText", 1);
            UIManager::uiShader.SendFloat2("ui.minBound", UIManager::currentMin.x, UIManager::currentMin.y);
            UIManager::uiShader.SendFloat2("ui.maxBound", UIManager::currentMax.x, UIManager::currentMax.y);
            UIManager::uiShader.SendFloat1("ui.radius", 0.0f);

            float x = 0.0f;
            float y = 0.0f;
            float scale = ((1.0f / 48.0f) * (fontSize / 80.0f)) / 2.0f;

            glActiveTexture(GL_TEXTURE0);
            vo.Bind();

            float maxY = 0.0f;

            for (char c : text) {
                Character ch = font->chars[c];
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;

                if (ypos + h > maxY) maxY = ypos + h;

                x += (ch.Advance >> 6) * scale;
            }

            float width = x;
            float height = maxY;

            x = 0.0f;
            y = 0.0f;

            float xMod = width / 2.0 - (UIManager::currentAnchor.direction.x * width / 2.0);
            float yMod = height / 2.0 - (UIManager::currentAnchor.direction.y * height / 2.0);

            for (char c : text) {
                Character ch = font->chars[c];
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;

                std::vector<float> vertices = {
                    xpos - xMod,     ypos - yMod,     0.0f, 1.0f,
                    xpos - xMod,     ypos + h - yMod, 0.0f, 0.0f,
                    xpos + w - xMod, ypos + h - yMod, 1.0f, 0.0f,
                    xpos + w - xMod, ypos - yMod,     1.0f, 1.0f
                };

                vo.BufferData(vertices);

                glBindTexture(GL_TEXTURE_2D, ch.TextureID);

                Renderer::Render(&UIManager::uiShader, &vo, &io);

                x += (ch.Advance >> 6) * scale;
            }

            PRINT_GL_ERROR();
		}
	};
}