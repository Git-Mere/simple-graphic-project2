/**
 * \file GLTexture.cpp
 * \author Rudy Castan
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#include "GLTexture.hpp"

#include "GL.hpp"
#include "assets/Path.hpp"
#include "environment/OpenGL.hpp"
#include <GL/glew.h>
#include <stb_image.h>

GLTexture::~GLTexture() noexcept
{
    delete_texture();
}

GLTexture::GLTexture(GLTexture&& other) noexcept
    : texture_handle(other.texture_handle), width(other.width), height(other.height)
{
    other.texture_handle = 0;
    other.width          = 0;
    other.height         = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
    std::swap(texture_handle, other.texture_handle);
    std::swap(width, other.width);
    std::swap(height, other.height);
    return *this;
}

bool GLTexture::LoadFromFileImage(std::filesystem::path image_filepath, bool flip_vertical) noexcept
{
    if (!std::filesystem::exists(image_filepath))
    {
        // try prepending the asset directory path
        image_filepath = assets::get_base_path() / image_filepath;
        if (!std::filesystem::exists(image_filepath))
        {
            return false;
        }
    }
    int           pixel_width;
    int           pixel_height;
    int           files_channels_count  = 0;
    constexpr int desired_channel_count = 4;
    stbi_set_flip_vertically_on_load(flip_vertical);
    RGBA* const rgba_pixels =
        reinterpret_cast<RGBA*>(stbi_load(image_filepath.string().c_str(), &pixel_width, &pixel_height, &files_channels_count, desired_channel_count));
    if (rgba_pixels == nullptr)
        return false;
    const bool result = LoadFromMemory(pixel_width, pixel_height, rgba_pixels);
    stbi_image_free(rgba_pixels);
    return result;
}

bool GLTexture::LoadAsDepthTexture(int image_width, int image_height, DepthComponentSize bit_depth) noexcept
{
    delete_texture();
    width  = image_width;
    height = image_height;
    IF_CAN_DO_OPENGL(4, 5)
    {
        GL::CreateTextures(GL_TEXTURE_2D, 1, &texture_handle);
        GL::TextureStorage2D(texture_handle, 1, bit_depth, width, height);
        GL::TextureParameteri(texture_handle, GL_TEXTURE_MIN_FILTER, Filtering::Linear);
        GL::TextureParameteri(texture_handle, GL_TEXTURE_MAG_FILTER, Filtering::Linear);
        GL::TextureParameteri(texture_handle, GL_TEXTURE_WRAP_S, Wrapping::ClampToEdge);
        GL::TextureParameteri(texture_handle, GL_TEXTURE_WRAP_T, Wrapping::ClampToEdge);
        GL::TextureParameteri(texture_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        GL::TextureParameteri(texture_handle, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
    }
    else
    {
        GL::GenTextures(1, &texture_handle);
        GL::BindTexture(GL_TEXTURE_2D, texture_handle);

        int version = environment::opengl::MajorVersion * 100 + environment::opengl::MinorVersion * 10;
        int limit_version = 420;
        if (environment::opengl::IsOpenGL_ES || version >= limit_version)
        {
            GL::TexStorage2D(GL_TEXTURE_2D, 1, bit_depth, width, height);
        }
        else
        {
            GL::TexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(bit_depth), width, height, 0, bit_depth, GL_FLOAT, nullptr);
        }

        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filtering::Linear);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filtering::Linear);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrapping::ClampToEdge);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrapping::ClampToEdge);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        GL::BindTexture(GL_TEXTURE_2D, 0);
    }


    return true;
}


bool GLTexture::LoadAsRGBA(int image_width, int image_height) noexcept
{
    return LoadFromMemory(image_width, image_height, nullptr);
}

bool GLTexture::LoadAsFormat(int image_width, int image_height, ColorFormat format) noexcept
{
    return LoadFromMemory(image_width, image_height, nullptr, format);
}

bool GLTexture::LoadFromMemory(int image_width, int image_height, const RGBA* colors, ColorFormat format) noexcept
{
    delete_texture();
    width  = image_width;
    height = image_height;

    GLuint            tex_handle;
    constexpr GLsizei ONE_TEXTURE_LEVEL = 1;
    constexpr GLint   FIRST_LEVEL       = 0;
    constexpr GLsizei OFFSET_X = 0, OFFSET_Y = 0;
    IF_CAN_DO_OPENGL(4, 5)
    {
        GL::CreateTextures(GL_TEXTURE_2D, 1, &tex_handle);
        GL::TextureStorage2D(tex_handle, ONE_TEXTURE_LEVEL, format, width, height);
        if (colors != nullptr)
        {
            GL::TextureSubImage2D(tex_handle, FIRST_LEVEL, OFFSET_X, OFFSET_Y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colors);
        }
        GL::TextureParameteri(tex_handle, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
        GL::TextureParameteri(tex_handle, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
        GL::TextureParameteri(tex_handle, GL_TEXTURE_WRAP_S, wrapping[S]);
        GL::TextureParameteri(tex_handle, GL_TEXTURE_WRAP_T, wrapping[T]);
    }
    else
    {
        GL::GenTextures(1, &tex_handle);
        GL::BindTexture(GL_TEXTURE_2D, tex_handle);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping[S]);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping[T]);
        GL::TexImage2D(GL_TEXTURE_2D, FIRST_LEVEL, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);
        GL::BindTexture(GL_TEXTURE_2D, 0);
    }
    texture_handle = tex_handle;
    return true;
}


void GLTexture::UploadAsRGBA([[maybe_unused]]gsl::not_null<const RGBA*> colors) noexcept
{
    constexpr int base_mipmap_level = 0;
    constexpr int xoffset = 0, yoffset = 0;
    IF_CAN_DO_OPENGL(4, 5)
    {
        GL::TextureSubImage2D(GetHandle(), base_mipmap_level, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colors);
    }
    else
    {
        GL::BindTexture(GL_TEXTURE_2D, GetHandle());
        GL::TexSubImage2D(GL_TEXTURE_2D, base_mipmap_level, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colors);
        GL::BindTexture(GL_TEXTURE_2D, 0);
    }
}

void GLTexture::UseForSlot(unsigned texture_unit) const noexcept
{
    IF_CAN_DO_OPENGL(4, 5)
    {
        GL::BindTextureUnit(texture_unit, texture_handle);
    }
    else
    {
        GL::ActiveTexture(GL_TEXTURE0 + texture_unit);
        GL::BindTexture(GL_TEXTURE_2D, texture_handle);
    }
}

void GLTexture::SetFiltering(Filtering how_to_filter) noexcept
{
    if (filtering == how_to_filter || texture_handle == 0)
        return;

    filtering = how_to_filter;

    IF_CAN_DO_OPENGL(4, 5)
    {
        GL::TextureParameteri(texture_handle, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
        GL::TextureParameteri(texture_handle, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
    }
    else if (texture_handle != 0)
    {
        GL::BindTexture(GL_TEXTURE_2D, texture_handle);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
        GL::BindTexture(GL_TEXTURE_2D, 0);
    }
}

void GLTexture::SetWrapping(Wrapping how_to_wrap, Coordinate coord) noexcept
{
    if (texture_handle == 0)
        return;

    GLenum coords_to_set[2] = {};
    int    num_coords       = 1;
    switch (coord)
    {
        case Coordinate::S:
            coords_to_set[0] = GL_TEXTURE_WRAP_S;
            wrapping[S]      = how_to_wrap;
            break;
        case Coordinate::T:
            coords_to_set[0] = GL_TEXTURE_WRAP_T;
            wrapping[T]      = how_to_wrap;
            break;
        case Coordinate::Both:
            coords_to_set[0] = GL_TEXTURE_WRAP_S;
            coords_to_set[1] = GL_TEXTURE_WRAP_T;
            wrapping[S]      = how_to_wrap;
            wrapping[T]      = how_to_wrap;
            num_coords       = 2;
            break;
    }
    IF_CAN_DO_OPENGL(4, 5)
    {
        for (int i = 0; i < num_coords; ++i)
        {
            GL::TextureParameteri(texture_handle, coords_to_set[i], how_to_wrap);
        }
    }
    else
    {
        GL::BindTexture(GL_TEXTURE_2D, texture_handle);
        for (int i = 0; i < num_coords; ++i)
        {
            GL::TexParameteri(GL_TEXTURE_2D, coords_to_set[i], how_to_wrap);
        }
        GL::BindTexture(GL_TEXTURE_2D, 0);
    }
}

void GLTexture::delete_texture() noexcept
{
    GL::DeleteTextures(1, &texture_handle);
    texture_handle = 0;
    width          = 0;
    height         = 0;
}
