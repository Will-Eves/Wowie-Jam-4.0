#pragma once

#define TEXTURE_NORMAL GL_RGBA8
#define TEXTURE_HDR GL_RGBA16F

namespace Burst {
	struct Texture {
		unsigned int texture;

		int width;
		int height;
		int bpp;

		unsigned char* buffer;

		Texture(int textureMode=TEXTURE_NORMAL) {
			width = 1;
			height = 1;
			bpp = 4;
			buffer = new unsigned char[4]{
				255,
				255,
				255,
				255
			};

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, textureMode, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture(unsigned char* _buffer, int textureMode = TEXTURE_NORMAL) {
			width = 1;
			height = 1;
			bpp = 4;
			buffer = _buffer;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, textureMode, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture(std::string _filepath, bool pixelPerfect = false, bool destroyBuffer = true, int textureMode = TEXTURE_NORMAL) {
			stbi_set_flip_vertically_on_load(1);
			buffer = stbi_load(_filepath.c_str(), &width, &height, &bpp, 4);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			if (pixelPerfect) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL Error Here
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // GL Error Here

			glTexImage2D(GL_TEXTURE_2D, 0, textureMode, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glBindTexture(GL_TEXTURE_2D, 0);

			if (buffer && destroyBuffer) stbi_image_free(buffer);
		}

		void Bind(unsigned int slot = 0) {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
	};
}