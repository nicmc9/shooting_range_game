/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int texture_id; // ID handle of the glyph texture
    glm::ivec2   size;      // size of glyph
    glm::ivec2   bearing;   // offset from baseline to left/top of glyph
    unsigned int advance;   // horizontal offset to advance to next glyph
};


class TextRenderer
{
public:
     // constructor
    TextRenderer(GLuint width, GLuint height);
    // pre-compiles a list of characters from the given font
    void Load(std::string font, GLuint font_size);
    // renders a string of text using the precompiled list of characters
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    // render state
    GLuint vao_, vbo_;
    Shader text_shader_;
    std::map<char, Character> characters_; 
};

#endif 