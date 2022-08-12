namespace Burst {
	unsigned int BURST_RENDER_OBJECT_COUNT = 0;

	class RenderObject {
	private:
		unsigned int simID;
		VertexObject vo;
		IndexObject io;
		Shader* shader;
	public:
		RenderObject() {
			AttribObject blank;
			std::vector<float> blankVerts;
			std::vector<unsigned int> blankIndicies;
			vo = VertexObject(blankVerts, &blank);
			io = IndexObject(blankIndicies);
		}

		RenderObject(Mesh* mesh, Shader* _shader) {
			simID = BURST_RENDER_OBJECT_COUNT;
			BURST_RENDER_OBJECT_COUNT++;

			shader = _shader;

			vo = VertexObject(mesh->vertices, shader->GetAttribObject());
			io = IndexObject(mesh->indicies);
		}

		void Render() {
			Renderer::Render(shader, &vo, &io);
		}

		Shader* GetShader() {
			return shader;
		}

		VertexObject* GetVertexObject() {
			return &vo;
		}

		IndexObject* GetIndexObject() {
			return &io;
		}
	};
}