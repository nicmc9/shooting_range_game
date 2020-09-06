#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

// Создание статических переменных
std::map<std::string, Texture2D>    ResourceManager::textures_;
std::map<std::string, Shader>       ResourceManager::shaders_;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    shaders_[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return shaders_[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return shaders_[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    textures_[name] = LoadTextureFromFile(file, alpha);
    return textures_[name];

}

Texture2D& ResourceManager::GetTexture(std::string name)
{
    return textures_[name];
}

void ResourceManager::Clear()
{
    // удалим все шейдерные программы из памяти
    for (auto iter : shaders_)
        glDeleteProgram(iter.second.id_);
    // удалим все текстуры
    for (auto iter : textures_)
        glDeleteTextures(1, &iter.second.id_);
}

Shader ResourceManager::LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    //Получить исходный код по пути к файлу
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // открываем файлы
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        
        // читаем содержимое файлового буфера в поток
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // закрываем обработчики файлов
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // получим из потока строку
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // если есть путь для геометрического шейдера то загружаем и его
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // Теперь создаем объект шейдера из исходного кода
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
    // Создаем текстурный объект
    Texture2D texture;
    if (alpha)
    {
        texture.internal_format_ = GL_RGBA;
        texture.image_format_ = GL_RGBA;
    }
    // загружаем изображение 
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // генерируем текстуру
    texture.Generate(width, height, data);
    // освобождаем временную память
    stbi_image_free(data);
    return texture;
}