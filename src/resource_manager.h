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

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

/*
Статический синглетон для управления ресурсами
может загружать текстуры и шейдеры и обеспечивает их хранение
*/
class ResourceManager
{
public:
    // хранилище ресурсов
    static std::map<std::string, Shader>    shaders_;
    static std::map<std::string, Texture2D> textures_;

    //Загружает шейдер из файла генерирует программу и сохраняет в хранилище
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    static Shader&   GetShader(std::string name);


    //Загружает и генерирует текстуру из файла и сохраняет в хранилище
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    static Texture2D& GetTexture(std::string name);

    // очиска шейдеров и текстур
    static void      Clear();
private:
    ResourceManager() = delete;

   // загружает и генерирует шейдерную программу
    static Shader    LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // загружает и генерирует текстуру
    static Texture2D LoadTextureFromFile(const char* file, bool alpha);
};

