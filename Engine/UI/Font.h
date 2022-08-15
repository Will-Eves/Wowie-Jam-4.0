#pragma once

namespace Burst {
	struct Character {
		unsigned int TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

    struct Font {
        std::map<char, Character> chars;

        Font() {
            return;
        }

		Font(std::string filename);
    };

	namespace FontManager {
		std::vector<Font*> fonts;

		Font defaultFont;

		FT_Library ft;

		void Setup() {
			if (FT_Init_FreeType(&ft)){
				std::cout << "Could not start freetype." << std::endl;
			}

            FT_Face face;
			if (FT_New_Face(ft, "engine_res/Fonts/Roboto.ttf", 0, &face)) {
				std::cout << "Could not load font 'engine_res/Fonts/Roboto.ttf'" << std::endl;
            }
            else {
                FT_Face face;
                if (FT_New_Face(FontManager::ft, "engine_res/Fonts/Roboto.ttf", 0, &face)) {
                    std::cout << "Could not load font '" << "engine_res/Fonts/Roboto.ttf" << "'" << std::endl;
                    return;
                }

                FT_Set_Pixel_Sizes(face, 0, 48);

                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                for (unsigned char c = 0; c < 128; c++) {
                    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                        std::cout << "Failed to load font glyph on font : " << "engine_res/Fonts/Roboto.ttf" << std::endl;
                        continue;
                    }

                    unsigned int texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_ALPHA,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_ALPHA,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                    );

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    Character character = {
                        texture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        face->glyph->advance.x
                    };

                    defaultFont.chars.insert(std::pair<char, Character>(c, character));
                }

                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                FT_Done_Face(face);
                FT_Done_FreeType(ft);
            }
		}
	}

    Font::Font(std::string filename) {
        FT_Face face;
        if (FT_New_Face(FontManager::ft, "res/Fonts/Roboto-Medium.ttf", 0, &face)) {
            std::cout << "Failed to load font : " << filename << std::endl;
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, 50);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "Failed to load font glyph on font : " << filename << std::endl;
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

            chars.insert(std::pair<char, Character>(c, character));
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_Done_Face(face);
        FT_Done_FreeType(FontManager::ft);
    }
}