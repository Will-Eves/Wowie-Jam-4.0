namespace Burst {
	struct Mesh {
		std::vector<float> vertices;
		std::vector<unsigned int> indicies;

		Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indicies) {
			vertices = _vertices;
			indicies = _indicies;
		}

		Mesh(std::string filename) {
			objl::Loader loader;
			bool loaded = loader.LoadFile(filename);
			if (!loaded) {
				std::cout << "Error loading model : " << filename << std::endl;
			}

			std::vector<objl::Vertex> overts = loader.LoadedVertices;
			std::vector<unsigned int> inds = loader.LoadedIndices;

			unsigned int size = inds.size();

			float maxAmount = 0.0f;

			//Normalize data
			for (int i = 0; i < size; i++) {
				int index = inds[i];
				float x = overts[index].Position.X;
				float y = overts[index].Position.Y;
				float z = overts[index].Position.Z;
				if (abs(x) > maxAmount) maxAmount = abs(x);
				if (abs(y) > maxAmount) maxAmount = abs(y);
				if (abs(z) > maxAmount) maxAmount = abs(z);
			}

			maxAmount *= 2.0f;

			//Set Data
			int vertexLength = overts.size();
			for (int i = 0; i < vertexLength; i++) {
				objl::Vertex* vertex = &overts[i];
				vertices.push_back(vertex->Position.X / maxAmount);
				vertices.push_back(vertex->Position.Y / maxAmount);
				vertices.push_back(vertex->Position.Z / maxAmount);
				vertices.push_back(vertex->TextureCoordinate.X);
				vertices.push_back(vertex->TextureCoordinate.Y);
				vertices.push_back(vertex->Normal.X);
				vertices.push_back(vertex->Normal.Y);
				vertices.push_back(vertex->Normal.Z);
				// Normal Tangents
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				// Normal Bitangents
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
			indicies = inds;

			int triCount = inds.size() / 3;

			for (int i = 0; i < triCount; i++) {
				int index = i * 3;
				int v_Index1 = inds[index + 0] * 14;
				int v_Index2 = inds[index + 1] * 14;
				int v_Index3 = inds[index + 2] * 14;

				Vector3 pos0 = Vector3(vertices[v_Index1 + 0], vertices[v_Index1 + 1], vertices[v_Index1 + 2]);
				Vector3 pos1 = Vector3(vertices[v_Index2 + 0], vertices[v_Index2 + 1], vertices[v_Index2 + 2]);
				Vector3 pos2 = Vector3(vertices[v_Index3 + 0], vertices[v_Index3 + 1], vertices[v_Index3 + 2]);

				Vector2 uv0 = Vector2(vertices[v_Index1 + 3], vertices[v_Index1 + 4]);
				Vector2 uv1 = Vector2(vertices[v_Index2 + 3], vertices[v_Index2 + 4]);
				Vector2 uv2 = Vector2(vertices[v_Index3 + 3], vertices[v_Index3 + 4]);

				Vector3 delta_pos1 = pos1 - pos0;
				Vector3 delta_pos2 = pos2 - pos0;

				Vector2 delta_uv1 = uv1 - uv0;
				Vector2 delta_uv2 = uv2 - uv0;

				float r = 1.0 / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
				Vector3 tangent = delta_pos1 * delta_uv2.y;
				tangent -= delta_pos2 * delta_uv1.y;
				tangent *= r;
				Vector3 bitangent = delta_pos2 * delta_uv1.x;
				bitangent -= delta_pos1 * delta_uv2.x;
				bitangent *= r;

				vertices[v_Index1 + 8] += tangent.x;
				vertices[v_Index1 + 9] += tangent.y;
				vertices[v_Index1 + 10] += tangent.z;

				vertices[v_Index2 + 8] += tangent.x;
				vertices[v_Index2 + 9] += tangent.y;
				vertices[v_Index2 + 10] += tangent.z;

				vertices[v_Index3 + 8] += tangent.x;
				vertices[v_Index3 + 9] += tangent.y;
				vertices[v_Index3 + 10] += tangent.z;

				vertices[v_Index1 + 11] += bitangent.x;
				vertices[v_Index1 + 12] += bitangent.y;
				vertices[v_Index1 + 13] += bitangent.z;

				vertices[v_Index2 + 11] += bitangent.x;
				vertices[v_Index2 + 12] += bitangent.y;
				vertices[v_Index2 + 13] += bitangent.z;

				vertices[v_Index3 + 11] += bitangent.x;
				vertices[v_Index3 + 12] += bitangent.y;
				vertices[v_Index3 + 13] += bitangent.z;
			}

			overts.clear();
			inds.clear();
		}

		~Mesh() {
			vertices.clear();
			indicies.clear();
		}
	};
}