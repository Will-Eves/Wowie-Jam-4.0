namespace Burst {
	struct Attrib {
		unsigned int size;
		unsigned int type = GL_FLOAT;
		unsigned int typesize;
		bool normalized = false;

		Attrib(unsigned int _size = 0, unsigned int _type = GL_FLOAT, bool _normalized = false) {
			size = _size;
			type = _type;
			normalized = _normalized;

			switch (type) {
			case GL_FLOAT:
				typesize = sizeof(float);
				break;
			case GL_INT:
				typesize = sizeof(int);
				break;
			case GL_UNSIGNED_INT:
				typesize = sizeof(unsigned int);
				break;
			case GL_DOUBLE:
				typesize = sizeof(double);
				break;
			case GL_BOOL:
				typesize = sizeof(bool);
				break;

			default:
				typesize = sizeof(float);
			}
		}
	};

	class AttribObject {
	private:
		std::vector<Attrib> attribs;

	public:
		void AddAttrib(Attrib attrib) {
			attribs.push_back(attrib);
		}

		void Generate() {
			int size = attribs.size();
			unsigned int stride = 0;
			unsigned int currentAmount = 0;
			if (size > 1) {
				for (int i = 0; i < size; i++) {
					stride += attribs[i].size * attribs[i].typesize;
				}
			}
			for (int i = 0; i < size; i++) {
				glVertexAttribPointer(i, attribs[i].size, attribs[i].type, attribs[i].normalized, stride, (void*)currentAmount);
				glEnableVertexAttribArray(i);
				currentAmount += attribs[i].size * attribs[i].typesize;
			}
		}
	};
}