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

#include <glad/glad.h>

/*
Класс для хранения и управления настройками 2D текстур
*/
class Texture2D
{
public:
    //Идентификатор объекта текстуры в памяти все функции работают через него
    GLuint id_;
    // формат текстурного объекта 
    GLuint internal_format_; 
    //формат загруженного изображения
    GLuint image_format_; 
   
    Texture2D();
    //создает текстуру из загруженного изображения
    void Generate(GLuint width, GLuint height, unsigned char* data);
    // привязывает текстуру как текущий GL_TEXTURE_2D объект
    void Bind() const;

private:
 // ширина и высота в пикселях загруженного изображения
    GLuint width_, height_; 
  
    GLuint wrap_s_; 
    GLuint wrap_t_; 
    GLuint filter_min_; 
    GLuint filter_max_; 
};

