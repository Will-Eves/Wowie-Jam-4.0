namespace Burst {
	class IndexObject {
	private:
		unsigned int iao;
		unsigned int drawtype;
	public:
		unsigned int indexCount = 0;

		IndexObject() {
			iao = 0;
			indexCount = 0;
		}

		IndexObject(unsigned int* data, unsigned int _indexCount, unsigned int _drawtype = GL_STATIC_DRAW) {
			drawtype = _drawtype;

			glGenBuffers(1, &iao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indexCount, data, drawtype);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			indexCount = _indexCount;
		}

		IndexObject(std::vector<unsigned int> data, unsigned int _drawtype = GL_STATIC_DRAW) {
			drawtype = _drawtype;

			if (data.size() == 0) return;

			glGenBuffers(1, &iao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data[0], drawtype);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			indexCount = data.size();
		}

		void BufferData(std::vector<unsigned int> data) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data[0], drawtype);
		}

		void Bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
		}

		void Unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void DeleteData() {
			glDeleteBuffers(1, &iao);
		}
	};
}