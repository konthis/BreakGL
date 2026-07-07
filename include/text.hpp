#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>
#include <map>
#include <glm/vec2.hpp>
#include "shader.hpp"


/*
https://learnopengl.com/In-Practice/Text-Rendering
*/

struct Character {
    GLuint textureID;
    glm::ivec2 size;     // bitmap width/height
    glm::ivec2 bearing;  // offset from baseline to top-left of glyph
    long int advance;      // horizontal offset to next character
};


class TextRenderer{
    private:
        FT_Library mFTLib;
        FT_Face mFTFace;
        std::map<char, Character> mCharacters;
    public:
        ~TextRenderer(){
            for (auto& [c, ch] : mCharacters)
                glDeleteTextures(1, &ch.textureID);
        }
        void init(unsigned int pixelSize){
            if (FT_Init_FreeType(&mFTLib))
            {
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            }

            if (FT_New_Face(mFTLib, FONTS_DIR "/GeistPixel.ttf", 0, &mFTFace))
            {
                std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
            }
            
            FT_Set_Pixel_Sizes(mFTFace, 0, pixelSize);  

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            for (unsigned char c = 32; c < 127; c++){
                // load character glyph 
                if (FT_Load_Char(mFTFace, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    mFTFace->glyph->bitmap.width,
                    mFTFace->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    mFTFace->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture, 
                    glm::ivec2(mFTFace->glyph->bitmap.width, mFTFace->glyph->bitmap.rows),
                    glm::ivec2(mFTFace->glyph->bitmap_left, mFTFace->glyph->bitmap_top),
                    mFTFace->glyph->advance.x
                };
                mCharacters.insert(std::pair<char, Character>(c, character));
            }
            FT_Done_Face(mFTFace);
            FT_Done_FreeType(mFTLib);
        }

        const std::map<char, Character>& getCharacters() const { return mCharacters; }

};

inline float getTextWidth(const std::map<char, Character>& chars, const std::string& text, float scale) {
    float width = 0;
    for (char c : text)
        width += (chars.at(c).advance >> 6) * scale;
    return width;
}
