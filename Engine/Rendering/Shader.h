namespace Burst {
	struct Shader {
		unsigned int program;
		AttribObject ao;

		Shader() {
			return;
		}

		Shader(std::string vertexShader, std::string fragmentShader, bool effect=false);

		void Bind() {
			glUseProgram(program);
		}

		void Unbind() {
			glUseProgram(0);
		}

		void SendInt1(std::string name, int data) {
			glProgramUniform1i(program, glGetUniformLocation(program, name.c_str()), data);
		}

		void SendUnsignedInt1(std::string name, unsigned int data) {
			glProgramUniform1ui(program, glGetUniformLocation(program, name.c_str()), data);
		}

		void SendFloat1(std::string name, float data) {
			glProgramUniform1f(program, glGetUniformLocation(program, name.c_str()), data);
		}

		void SendFloat2(std::string name, float data1, float data2) {
			glProgramUniform2f(program, glGetUniformLocation(program, name.c_str()), data1, data2);
		}

		void SendFloat3(std::string name, float data1, float data2, float data3) {
			glProgramUniform3f(program, glGetUniformLocation(program, name.c_str()), data1, data2, data3);
		}

		void SendFloat4(std::string name, float data1, float data2, float data3, float data4) {
			glProgramUniform4f(program, glGetUniformLocation(program, name.c_str()), data1, data2, data3, data4);
		}

		void SendMatrix2(std::string name, glm::mat2 matrix) {
			glProgramUniformMatrix2fv(program, glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
		}

		void SendMatrix3(std::string name, glm::mat3 matrix) {
			glProgramUniformMatrix3fv(program, glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
		}

		void SendMatrix4(std::string name, glm::mat4 matrix) {
			glProgramUniformMatrix4fv(program, glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
		}

		unsigned int GetShaderProgram() {
			return program;
		}

		AttribObject* GetAttribObject() {
			return &ao;
		}
	};

	namespace ShaderManager {
		unsigned int shaderCount = 0;
		std::vector<Shader*> shaders;

		void AddShader(Shader* shader) {
			shaderCount++;
			shaders.push_back(shader);
		}
	}

	Shader::Shader(std::string vertexShader, std::string fragmentShader, bool effect) {
		if(!effect) ShaderManager::AddShader(this);

		const char* vertex_path = vertexShader.c_str();
		const char* fragment_path = fragmentShader.c_str();

		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_path, std::ios::in);
		std::string vline;
		while (std::getline(VertexShaderStream, vline)) {
			VertexShaderCode += vline + "\n";
		}

		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
		std::string fline;
		while (std::getline(FragmentShaderStream, fline)) {
			FragmentShaderCode += fline + "\n";
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			std::cout << &VertexShaderErrorMessage[0] << std::endl;
		}

		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			std::cout << &FragmentShaderErrorMessage[0] << std::endl;
		}

		program = glCreateProgram();
		glAttachShader(program, VertexShaderID);
		glAttachShader(program, FragmentShaderID);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &Result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			std::cout << &ProgramErrorMessage[0] << std::endl;
		}

		glDetachShader(program, VertexShaderID);
		glDetachShader(program, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		glUseProgram(0);

		std::ifstream file(vertex_path);
		std::string line;
		while (std::getline(file, line)) {
			const char* chars = line.c_str();
			unsigned int length = line.length();

			bool layoutStarted = false;
			bool varStarted = false;
			int vari = 3;
			std::string varName = "";
			for (int i = 0; i < length; i++) {
				if (!layoutStarted && chars[i] == 'l' && chars[i + 1] == 'a' && chars[i + 2] == 'y' && chars[i + 3] == 'o' && chars[i + 4] == 'u' && chars[i + 5] == 't') {
					layoutStarted = true;
				}
				if (!varStarted && layoutStarted && chars[i] == 'i' && chars[i + 1] == 'n') {
					varStarted = true;
				}
				if (varStarted) {
					vari -= 1;
					if (vari < 0) {
						if (chars[i] == ' ') {
							break;
						}
						varName += chars[i];
					}
				}
			}

			if (varStarted) {
				//A Single Type
				if (varName == "bool") {
					ao.AddAttrib(Attrib(1, GL_BOOL, false));
				}
				else if (varName == "int") {
					ao.AddAttrib(Attrib(1, GL_INT, false));
				}
				else if (varName == "uint") {
					ao.AddAttrib(Attrib(1, GL_UNSIGNED_INT, false));
				}
				else if (varName == "float") {
					ao.AddAttrib(Attrib(1, GL_FLOAT, false));
				}
				else if (varName == "double") {
					ao.AddAttrib(Attrib(1, GL_DOUBLE, false));
				}

				//A Vector
				const char* chars = varName.c_str();
				int slen = varName.length();
				if (chars[0] == 'v') {
					std::stringstream ss;
					ss << chars[3];
					length = stoi(ss.str());
					ao.AddAttrib(Attrib(length, GL_FLOAT, false));
				}
				else if (chars[1] == 'v') {
					char type = chars[0];
					std::stringstream ss;
					ss << chars[3];
					length = stoi(ss.str());
					if (type == 'i') {
						ao.AddAttrib(Attrib(length, GL_INT, false));
					}
					else if (type == 'u') {
						ao.AddAttrib(Attrib(length, GL_UNSIGNED_INT, false));
					}
					else if (type == 'd') {
						ao.AddAttrib(Attrib(length, GL_DOUBLE, false));
					}
				}

				//A Matrix
				if (chars[0] == 'm') {
					int x;
					int y;
					if (slen == 6) {
						x = (int)chars[3];
						y = (int)chars[5];
					}
					else {
						x = (int)chars[3];
						y = (int)chars[3];
					}

					for (int a = 0; a < y; a++) {
						ao.AddAttrib(Attrib(x, GL_FLOAT, false));
					}
				}
			}
		}
	}
}