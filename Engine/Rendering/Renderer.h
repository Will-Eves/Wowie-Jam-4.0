#pragma once

namespace Burst {
	namespace Renderer {
        void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void SetBackgroundColor(float r, float g, float b) {
            glClearColor(r, g, b, 1.0f);
        }

        void Render(Shader* shader, VertexObject* vo, IndexObject* io, unsigned int renderMode = GL_TRIANGLES) {
            shader->Bind();
            vo->Bind();
            io->Bind();

            glDrawElements(renderMode, io->indexCount, GL_UNSIGNED_INT, nullptr);
        }
	}
}