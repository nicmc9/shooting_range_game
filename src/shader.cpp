/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include "shader.h"
#include <iostream>

Shader& Shader::Use()
{
    glUseProgram(this->id_);
    return *this;
}

void Shader::Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
    GLuint shader_vertex, shader_fragment, shader_geometry;
    // вершинный Шейдер
     shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, &vertex_source, NULL);
    glCompileShader(shader_vertex);
    CheckCompileErrors(shader_vertex, "VERTEX");
    // фрагментный Шейдер
    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, &fragment_source, NULL);
    glCompileShader(shader_fragment);
    CheckCompileErrors(shader_fragment, "FRAGMENT");
    // если предоставлени код геометрического шейдера также добавляем его
    if (geometry_source != nullptr)
    {
        shader_geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(shader_geometry, 1, &geometry_source, NULL);
        glCompileShader(shader_geometry);
        CheckCompileErrors(shader_geometry, "GEOMETRY");
    }
    // шейдерная программа
    id_ = glCreateProgram();
    glAttachShader(id_, shader_vertex);
    glAttachShader(id_, shader_fragment);
    if (geometry_source != nullptr)
        glAttachShader(id_, shader_geometry);
    glLinkProgram(id_);
    CheckCompileErrors(id_, "PROGRAM");
    //удаление шейдеров так как они уже записаны в программу и больше не нужны
    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);
    if (geometry_source != nullptr)
        glDeleteShader(shader_geometry);
}

void Shader::SetFloat(const char* name, float value, bool use_shader)
{
    if (use_shader) 
        Use();
    glUniform1f(glGetUniformLocation(id_, name), value);
}
void Shader::SetInteger(const char* name, int value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform1i(glGetUniformLocation(id_, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform2f(glGetUniformLocation(id_, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool use_shader)
{
    if (use_shader)
        Use();
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, false, glm::value_ptr(matrix));
}

void Shader::CheckCompileErrors(unsigned int object, std::string type)
{
    int success;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, info_log);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << info_log << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, info_log);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << info_log << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}