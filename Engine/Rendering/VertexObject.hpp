namespace Burst {
	class VertexObject {
	private:
		unsigned int buffer;
		unsigned int vao;
		unsigned int drawtype;
	public:
		unsigned int vertexCount = 0;

		VertexObject() {
			buffer = 0;
			vao = 0;
			vertexCount = 0;
		}

		VertexObject(float* data, unsigned int _vertexCount, AttribObject* ao, unsigned int _drawtype = GL_STATIC_DRAW) {
			drawtype = _drawtype;

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

		VertexObject(std::vector<float> data, AttribObject* ao, unsigned int _drawtype = GL_STATIC_DRAW) {
			if (data.size() == 0) return;

			drawtype = _drawtype;

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

		void BufferData(std::vector<float> data) {
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], drawtype);
		}

		void Bind() {
			glBindVertexArray(vao);
		}

		void Unbind() {
			glBindVertexArray(0);
		}

		void DeleteData() {
			glDeleteBuffers(1, &vao);
		}
	};
}