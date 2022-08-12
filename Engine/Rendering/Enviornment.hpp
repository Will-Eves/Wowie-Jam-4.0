#pragma once

namespace Burst {
	namespace Enviornment {
		Texture* skyboxTexture;

        Shader skyboxShader;

        RenderObject skyboxRenderObject;

		void Setup() {
            skyboxShader = Shader("engine_res/Shaders/skybox_vertex.shader", "engine_res/Shaders/skybox_fragment.shader", true);

            std::vector<float> positions = {
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
            };

            std::vector<unsigned int> indicies = {
                // Front Face
                4, 6, 5,
                5, 6, 7,

                // Back Face
                0, 1, 2,
                1, 3, 2,

                // Top Face
                2, 3, 6,
                3, 7, 6,

                // Bottom Face
                0, 4, 1,
                1, 4, 5,

                // Right Face
                0, 2, 4,
                2, 6, 4,

                // Left Face
                1, 5, 3,
                3, 5, 7
            };

            Mesh skyboxMesh(positions, indicies);

            skyboxRenderObject = RenderObject(&skyboxMesh, &skyboxShader);
		}

        void SetSkybox(Texture* texture) {
            skyboxTexture = texture;
        }

        void SendSkyboxToShaders() {
            for (Shader* shader : ShaderManager::shaders) {
                shader->Bind();
                shader->SendInt1("enviornment.skybox", 6);
                skyboxTexture->Bind(6);
            }
        }

        void SetProjectionMatrix(glm::mat4 projection) {
            skyboxShader.SendMatrix4("projectionMatrix", projection);
        }

        void SetCameraMatrix(glm::mat4 camera) {
            skyboxShader.SendMatrix4("viewMatrix", camera);
        }

        void Render() {
            glDisable(GL_DEPTH_TEST);

            skyboxShader.Bind();
            skyboxShader.SendInt1("equirectangularMap", 7);
            skyboxTexture->Bind(7);
            skyboxRenderObject.Render();

            glEnable(GL_DEPTH_TEST);
        }
	}
}