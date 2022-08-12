namespace Burst {
	class VertexObject {
	private:
		unsigned int buffer;
		unsigned int vao;
	public:
		unsigned int vertexCount = 0;

		VertexObject() {
			buffer = 0;
			vao = 0;
			vertexCount = 0;
		}

		VertexObject(float* data, unsigned int _vertexCount, AttribObject* ao, unsigned int drawtype = GL_STATIC_DRAW) {
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexCount, data, drawtype);

			ao->Generate();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			vertexCount = _vertexCount;
		}

		VertexObject(std::vector<float> data, AttribObject* ao, unsigned int drawtype = GL_STATIC_DRAW) {
			if (data.size() == 0) return;

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], drawtype);

			ao->Generate();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			vertexCount = data.size();
		}

		void Bind() {
			glBindVertexArray(vao);
		}

		void Unbind() {
			glBindVertexArray(0);
		}
	};
}