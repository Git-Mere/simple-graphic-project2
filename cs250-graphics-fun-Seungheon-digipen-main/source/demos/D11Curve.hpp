/**
 * \file
 * \author Rudy Castan
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#pragma once

#include "IDemo.hpp"
#include "assets/Reloader.hpp"
#include "graphics/Material.hpp"
#include "opengl/GLShader.hpp"
#include "opengl/GLTexture.hpp"
#include "opengl/GLVertexArray.hpp"
#include "graphics/Mesh.hpp"

namespace demos
{
    class D11Curve : public IDemo
    {
    public:
        D11Curve();

        void Update() override;

        void Draw() const override;
        void ImGuiDraw() override;

        void SetDisplaySize([[maybe_unused]] int width, [[maybe_unused]] int height) override;

    private:
        GLShader                                            shader;
        assets::Reloader                                    assetReloader;
        glm::mat4                                           ProjectionMatrix{ 1.0f };
        graphics::Material                                  material;
        GLVertexArray                                       Mesh;
        glm::vec4                                           targetOutsideTessellation{ 1.0f };
        std::array<glm::vec4, 10>                           depart_velocities{};
        std::array<glm::vec4, 10>                           arrive_velocities{};
        std::array<glm::vec4, 11>                           points_position{};
        bool                                                catmull = false;

    private:
        void buildMeshes();
    };

}
