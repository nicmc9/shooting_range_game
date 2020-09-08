/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    GLuint id_;
    Shader() { }
    //Делает текущий шейдер активным
    Shader& Use();
    //компилирует шейдер из исходного кода
    //геометрический шейдер может быть опциональным
    void    Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr);
    
    // вспомогательные функции
    void    SetFloat(const char* name, float value, bool use_shader = false);
    void    SetInteger(const char* name, int value, bool use_shader  = false);
    void    SetVector2f(const char* name, float x, float y, bool use_shader  = false);
    void    SetVector2f(const char* name, const glm::vec2& value, bool use_shader  = false);
    void    SetVector3f(const char* name, float x, float y, float z, bool use_shader  = false);
    void    SetVector3f(const char* name, const glm::vec3& value, bool use_shader  = false);
    void    SetVector4f(const char* name, float x, float y, float z, float w, bool use_shader  = false);
    void    SetVector4f(const char* name, const glm::vec4& value, bool use_shader  = false);
    void    SetMatrix4(const char* name, const glm::mat4& matrix, bool use_shader  = false);
private:
    void    CheckCompileErrors(unsigned int object, std::string type);
};

