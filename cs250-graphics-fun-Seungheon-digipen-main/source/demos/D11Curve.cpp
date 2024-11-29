/**
 * \file
 * \author Jeon Seungheon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#include "D11Curve.hpp"

#include "environment/Environment.hpp"
#include "environment/OpenGL.hpp"
#include "opengl/GL.hpp"
#include <array>
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/ext/matrix_transform.hpp>  // translate, rotate
#include <glm/trigonometric.hpp>         // all the GLSL trigonometric functions: radians, cos, asin, etc.
#include <imgui.h>
#include <span>

#include "graphics/Mesh.hpp"


using namespace std::string_literals;

namespace
{
    using namespace std::string_literals;

    namespace Uniforms
    {
        const auto Projection           = "uProjection"s;
        const auto DiffuseMaterial = "uDiffuseMaterial"s;
        const auto OutsideTessellation  = "uOutsideTessellation"s;
        const auto Catmull  = "uCatmull"s;

        const std::string points_position[11] = { 
            "uPosition"s, "uPosition2"s, "uPosition3"s, "uPosition4"s, "uPosition5"s, "uPosition6"s,
                                                    "uPosition7"s, "uPosition8"s, "uPosition9"s, "uPosition10"s, "uPosition11"s };
        const std::string Depart_velocities[10] = {
            "uDepart"s, "uDepart2"s, "uDepart3"s, "uDepart4"s, "uDepart5"s, "uDepart6"s, "uDepart7"s, "uDepart8"s, "uDepart9"s, "uDepart10"s
        };
        const std::string Arrive_velocities[10] = { 
            "uArrive"s, "uArrive2"s, "uArrive3"s, "uArrive4"s, "uArrive5"s, "uArrive6"s, "uArrive7"s, "uArrive8"s, "uArrive9"s, "uArrive10"s
        };
    }

    namespace asset_paths
    {
        constexpr auto Vertex                 = "D11Curve/vert.vert";
        constexpr auto Tessel_con             = "D11Curve/tessel_con.tesc";
        constexpr auto Tessel_eval            = "D11Curve/tessel_eval.tese";
        constexpr auto Geomerty               = "D11Curve/geometry.geom";
        constexpr auto Fragment               = "D11Curve/frag.frag";

        const auto     ShaderName             = "shader";

    }

}

namespace demos
{
    D11Curve::D11Curve()
    {
        GL::ClearColor(0.392f, 0.584f, 0.929f, 1.0f);
        assetReloader.SetAndAutoReloadShader(
            shader, asset_paths::ShaderName,
            {
                asset_paths::Vertex,
                asset_paths::Tessel_con, 
                asset_paths::Tessel_eval, 
                asset_paths::Geomerty, 
                asset_paths::Fragment
            });

        material = graphics::Material(&shader, asset_paths::ShaderName);
        material.Culling.Enabled = false;
        material.SetMaterialUniform(Uniforms::DiffuseMaterial, glm::vec3(0.19f, 0.191f, 0.191f));

        float help = 1.5f / (points_position.size() - 1);
        float starting = -0.75f;
        for (int i = 0; i < points_position.size(); i++)
        {
            points_position[i] = glm::vec4(starting + (help * i), 0.f, 0.f, 1.f);
        }

        buildMeshes();
    }

    void D11Curve::Update()
    {
        assetReloader.Update();
        material.SetMaterialUniform(Uniforms::Projection, ProjectionMatrix);
        material.SetMaterialUniform(Uniforms::OutsideTessellation, targetOutsideTessellation);
        material.SetMaterialUniform(Uniforms::Catmull, catmull);

        //position
        for (int i = 0; i < points_position.size(); i++)
        {
            material.SetMaterialUniform(Uniforms::points_position[i], points_position[i]);
        }

        //velocity
        material.SetMaterialUniform(Uniforms::Depart_velocities[0], depart_velocities[0]);
        for (int i = 1; i < targetOutsideTessellation.y; i++)
        {
            material.SetMaterialUniform(Uniforms::Arrive_velocities[i - 1], arrive_velocities[i - 1]);
            material.SetMaterialUniform(Uniforms::Depart_velocities[i], depart_velocities[i]);
        }
        material.SetMaterialUniform(Uniforms::Arrive_velocities[arrive_velocities.size() - 1], arrive_velocities[arrive_velocities.size() - 1]);
    }

    void D11Curve::Draw() const
    {
        GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GL::Enable(GL_PROGRAM_POINT_SIZE);
        material.ForceApplyAllSettings();

#if !defined(OPENGL_ES3_ONLY)
        GL::PatchParameteri(GL_PATCH_VERTICES, 4);
#endif
        shader.Use();

        Mesh.Use();
        GLDrawIndexed(Mesh);
    }

    void D11Curve::ImGuiDraw()
    {
        ImGui::Checkbox("Catmull-Rom Splines", &catmull);
        ImGui::SliderFloat("Points number", &targetOutsideTessellation.y, 1, 10);
        

        if (ImGui::CollapsingHeader("Points position"))
        {
            ImGui::SliderFloat2("first position", &points_position[0].x, -1, 1);
            for (int i = 1; i < targetOutsideTessellation.y; i++)
            {
                std::string point = "point" + std::to_string(i) + " position";
                ImGui::SliderFloat2(point.c_str(), &points_position[i].x, -1, 1);
            }
            ImGui::SliderFloat2("last position", &points_position[points_position.size() - 1].x, -1, 1);
        }


        if (ImGui::CollapsingHeader("Points velocity"))
        {
            ImGui::SliderFloat2("first point", &depart_velocities[0].x, -2, 2);
            if (catmull == false)
            {
                for (int i = 1; i < targetOutsideTessellation.y; i++)
                {
                    std::string point  = "point" + std::to_string(i) + " arrive";
                    std::string point2 = "point" + std::to_string(i) + " depart";
                    ImGui::SliderFloat2(point.c_str(), &arrive_velocities[i - 1].x, -2, 2);
                    ImGui::SliderFloat2(point2.c_str(), &depart_velocities[i].x, -2, 2);
                }
            }
            ImGui::SliderFloat2("last point", &arrive_velocities[arrive_velocities.size() - 1].x, -2, 2);
        }
    }

    void D11Curve::SetDisplaySize([[maybe_unused]] int width, [[maybe_unused]] int height)
    {
        /*const float aspect = static_cast<float>(width) / static_cast<float>(height);
        ProjectionMatrix   = glm::perspective(camera::FOV, aspect, camera::NearDistance, camera::FarDistance);*/
    }

    void D11Curve::buildMeshes()
    {
        {
            GLVertexArray model(GLPrimitive::Patches);

            std::vector<glm::vec3> positions = {
                glm::vec3{-0.75f, 0.f, 0.},
                glm::vec3{0.75f, 0.f, 0.},
                glm::vec3{ 0.5f, -0.5f, 0.},
                glm::vec3{ 0.5f,  0.5f, 0.}
            };
            std::vector<glm::vec3> colors = {
                glm::vec3{1},
                glm::vec3{1},
                glm::vec3{1},
                glm::vec3{1}
            };

            std::vector<unsigned> indices{ 0, 1, 2, 3 };

            GLVertexBuffer position_buffer(std::span{ positions });
            GLVertexBuffer color_buffer(std::span{ colors });

            model.SetVertexCount(static_cast<int>(positions.size()));

            GLAttributeLayout position_coor;
            position_coor.component_type         = GLAttributeLayout::Float;
            position_coor.component_dimension    = GLAttributeLayout::_3;
            position_coor.normalized             = false;
            position_coor.vertex_layout_location = 0; // 1st field is 0 index based
            position_coor.stride                 = sizeof(glm::vec3);
            position_coor.offset                 = 0;
            position_coor.relative_offset        = 0;

            GLAttributeLayout color;
            color.component_type         = GLAttributeLayout::Float;
            color.component_dimension    = GLAttributeLayout::_3;
            color.normalized             = false;
            color.vertex_layout_location = 1;
            color.stride                 = sizeof(glm::vec3);
            color.offset                 = 0;
            color.relative_offset        = 0;

            model.AddVertexBuffer(std::move(position_buffer), { position_coor });
            model.AddVertexBuffer(std::move(color_buffer), { color });

            GLIndexBuffer index_buffer(indices);
            model.SetIndexBuffer(std::move(index_buffer));
            Mesh = (std::move(model));
        }
    }


   
}
