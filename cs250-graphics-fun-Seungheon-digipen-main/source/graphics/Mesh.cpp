/**
 * \file
 * \author Rudy Castan
 * \author Jeon Seungheon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#include "Mesh.hpp"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <gsl/gsl>
#include <numbers>

namespace
{
    std::vector<graphics::MeshVertex> create_plane_vertices(int stacks, int slices);
    std::vector<unsigned>             build_index_buffer(int stacks, int slices);
    std::vector<unsigned>             convert_to_lines_pattern(const std::vector<unsigned>& indices);
    glm::vec3                         evaluate_trefoil(float row, float col);

}

namespace graphics
{
    Geometry create_plane(int stacks, int slices)
    {
        auto vertices = create_plane_vertices(stacks, slices);
        auto indices  = build_index_buffer(stacks, slices);
        return Geometry{ std::move(vertices), std::move(indices) };
    }

    Geometry create_cube(int stacks, int slices)
    {
        const auto plane_vertices = create_plane_vertices(stacks, slices);
        const auto plane_indices  = build_index_buffer(stacks, slices);

        enum Axis
        {
            X,
            Y
        };

        constexpr glm::vec3 AxisVectors[2] = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0) };

        constexpr glm::vec3 translate_array[] = {
            glm::vec3(+0.0f, +0.0f, +0.5f), // Z+
            glm::vec3(+0.0f, +0.0f, -0.5f), // Z-
            glm::vec3(+0.5f, +0.0f, +0.0f), // X+
            glm::vec3(-0.5f, +0.0f, +0.0f), // X-
            glm::vec3(+0.0f, +0.5f, +0.0f), // Y+
            glm::vec3(+0.0f, -0.5f, +0.0f), // Y-
        };

        struct rotation
        {
            Axis  axis;
            float angle;
        };

        constexpr rotation rotate_array[] = {
            {Axis::Y,                 0.0f}, // Z+
            {Axis::Y, glm::radians(180.0f)}, // Z-
            {Axis::Y,  glm::radians(90.0f)}, // X+
            {Axis::Y, glm::radians(-90.0f)}, // X-
            {Axis::X, glm::radians(-90.0f)}, // Y+
            {Axis::X,  glm::radians(90.0f)}  // Y-
        };

        std::vector<MeshVertex> vertices;
        std::vector<unsigned>   indices;
        vertices.reserve(plane_vertices.size() * 6u);
        indices.reserve(plane_indices.size() * 6u);
        /*  Transform the plane to 6 positions to form the faces of the cube */
        for (unsigned i = 0; i < 6; ++i)
        {
            constexpr glm::mat4 identity(1.0f);
            const auto          axis             = AxisVectors[rotate_array[i].axis];
            const auto          angle            = rotate_array[i].angle;
            const auto          rotation_matrix  = glm::rotate(identity, angle, axis);
            const auto          transform_matrix = glm::translate(identity, translate_array[i]) * rotation_matrix;
            for (const auto plane_vertex : plane_vertices)
            {
                MeshVertex v;
                v.position = glm::vec3(transform_matrix * glm::vec4(plane_vertex.position, 1.0));
                v.normal   = glm::vec3(rotation_matrix * glm::vec4(plane_vertex.normal, 1.0));
                v.uv       = plane_vertex.uv;
                vertices.push_back(v);
            }
            for (const auto plane_index : plane_indices)
            {
                indices.push_back(plane_index + static_cast<unsigned>(plane_vertices.size()) * i);
            }
        }
        return Geometry{ std::move(vertices), std::move(indices) };
    }

    Geometry create_sphere(int stacks, int slices)
    {
        std::vector<MeshVertex> vertices;
        int              numVertices = (stacks + 1) * (slices + 1);
        vertices.reserve(static_cast<size_t>(numVertices));
        for (int stack = 0; stack < stacks + 1; stack++)
        {
            const auto row      = static_cast<float>(stack) / static_cast<float>(stacks);
            const auto beta     = PI * (row - 0.5f);
            const auto sin_beta = sin(beta);
            const auto cos_beta = cos(beta);
            for (int slice = 0; slice < slices + 1; slice++)
            {
                const auto           radius = 0.5f;
                const auto           col    = static_cast<float>(slice) / static_cast<float>(slices);
                const auto           alpha  = col * 2 * PI;
                graphics::MeshVertex v;
                v.position = glm::vec3(radius * sin(alpha) * cos_beta, radius * sin_beta, radius * cos(alpha) * cos_beta);
                v.normal   = v.position / glm::vec3(radius);
                v.uv       = glm::vec2(col, row);
                vertices.push_back(v);
            }
        }
        std::vector<unsigned> indices = build_index_buffer(stacks, slices);
        return Geometry(vertices, indices);
    }

    Geometry create_torus(int stacks, int slices, float start_angle, float end_angle)
    {
        std::vector<MeshVertex> vertices;
        int                  numVertices = (stacks + 1) * (slices + 1);
        vertices.reserve(static_cast<size_t>(numVertices));
        const auto R = 0.35f;
        const auto r = 0.15f;
        for (int stack = 0; stack < stacks + 1; stack++)
        {
            const auto row       = static_cast<float>(stack) / static_cast<float>(stacks);
            const auto alpha    = std::lerp(start_angle, end_angle, row);
            const auto sin_alpha = sin(alpha);
            const auto cos_alpha = cos(alpha);
            for (int slice = 0; slice < slices + 1; slice++)
            {
                const auto           col      = static_cast<float>(slice) / static_cast<float>(slices);
                const auto           beta = std::lerp(2 * PI, 0.f, col);
                const auto           cos_beta = cos(beta);
                graphics::MeshVertex v;
                v.position.x      = (R + r * cos_beta) * sin_alpha;
                v.position.y      = r * sin(beta);
                v.position.z      = (R + r * cos_beta) * cos_alpha;
                const auto center = glm::vec3(R * sin_alpha, 0.f, R * cos_alpha);
                v.normal          = (v.position - center) / glm::vec3(r);
                v.uv              = glm::vec2(col, row);
                vertices.push_back(v);
            }       
        }   
        std::vector<unsigned> indices = build_index_buffer(stacks, slices);
        return Geometry(vertices, indices);
    }
    

    void add_cap(std::vector<MeshVertex>& vertices, std::vector<unsigned>& indices, float center_y, int slices)
    {
        const auto   R           = 0.5f;
        unsigned   centerIndex = static_cast<unsigned>(vertices.size());
        MeshVertex   vertex;
        auto textureCoordScale = center_y > 0 ? 1.0f : -1.0f;

        vertex.normal   = center_y > 0 ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(0.0f, -1.0f, 0.0f);
        vertex.position = glm::vec3(0.0f, center_y, 0.0f);
        vertex.uv       = glm::vec2(0.5f, 0.5f);

        vertices.push_back(vertex);

        for (int slice = 0; slice < slices + 1; slice++)
        {
            const auto col      = static_cast<float>(slice) / static_cast<float>(slices);
            const auto alpha    = col * PI * 2.0f;
            const auto sinAlpha = sin(alpha);
            const auto cosAlpha = cos(alpha);
            vertex.position.x   = R * sinAlpha;
            vertex.position.z   = R * cosAlpha;
            vertex.uv           = glm::vec2(textureCoordScale * 0.5f * cosAlpha + 0.5f, 0.5f * sinAlpha + 0.5f);
            vertices.push_back(vertex);
        }

        unsigned k                 = centerIndex + 1;
        unsigned secondIndexOffset = 0;
        unsigned thirdIndexOffset  = 0;
        secondIndexOffset          = center_y > 0 ? 0 : 1;
        thirdIndexOffset           = center_y > 0 ? 1 : 0;

        for (int slice = 0; slice < slices - 1; slice++)
        {
            indices.push_back(centerIndex);
            indices.push_back(k + secondIndexOffset);
            indices.push_back(k + thirdIndexOffset);
            k++;
        }
        if (center_y > 0){
            indices.push_back(centerIndex);
            indices.push_back(k);
            indices.push_back(centerIndex + 1);
        }
        else{
            indices.push_back(centerIndex);
            indices.push_back(centerIndex + 1);
            indices.push_back(k);
        }
    }

    Geometry create_cylinder(int stacks, int slices)
    {
        std::vector<MeshVertex> vertices;
        const auto              numVertices = (stacks + 1) * (slices + 1) + (slices * 2 + 2);
        vertices.reserve(static_cast<size_t>(numVertices));

        const auto R = 0.5f;
        for (int stack = 0; stack < stacks + 1; stack++)
        {
            const auto row = static_cast<float>(stack) / static_cast<float>(stacks);
            for (int slice = 0; slice < slices + 1; slice++)
            {
                const auto col   = static_cast<float>(slice) / static_cast<float>(slices);
                const auto alpha = col * PI * 2.0f;
                MeshVertex vertex;
                const auto sinAlpha = sin(alpha);
                const auto cosAlpha = cos(alpha);
                vertex.position.x   = R * sinAlpha;
                vertex.position.y   = row - 0.5f;
                vertex.position.z   = R * cosAlpha;
                vertex.normal       = glm::vec3(sinAlpha, 0, cosAlpha);
                vertex.uv           = glm::vec2(col, row);
                vertices.push_back(vertex);
            }
        }
        std::vector<unsigned> indices = build_index_buffer(stacks, slices);
        add_cap(vertices, indices, 0.5f, slices);
        add_cap(vertices, indices, -0.5f, slices);

        return Geometry(vertices, indices);
    }

    Geometry create_cone(int stacks, int slices)
    {
        std::vector<MeshVertex> vertices;
        const auto              numVertices = (stacks + 1) * (slices + 1) + (slices + 1);
        vertices.reserve(static_cast<size_t>(numVertices));
        const auto R            = 0.5f;
        const auto TopRadius    = 0.0f;
        const auto BottomRadius = R;
        const auto TopYValue    = 0.5f;
        const auto BottomYValue = -0.5f;
        const auto Rise         = TopYValue - BottomYValue;
        const auto Run          = TopRadius - BottomRadius;
        const auto Slope        = Rise / Run;
        const auto TangentSlope = -1.0f / Slope;
        for (int stack = 0; stack < stacks + 1; stack++)
        {
            const auto row = static_cast<float>(stack) / static_cast<float>(stacks);
            const auto h   = row - 0.5f;
            for (int slice = 0; slice < slices + 1; slice++)
            {
                const auto col      = static_cast<float>(slice) / static_cast<float>(slices);
                const auto alpha    = col * PI * 2.0f;
                const auto sinAlpha = sin(alpha);
                const auto cosAlpha = cos(alpha);
                MeshVertex vertex;
                vertex.position.x = R * (0.5f - h) * sinAlpha;
                vertex.position.y = h;
                vertex.position.z = R * (0.5f - h) * cosAlpha;
                if (stack != stacks){
                    vertex.normal = glm::normalize(glm::vec3(sinAlpha, TangentSlope, cosAlpha));
                }
                else{
                    vertex.normal = glm::vec3(0, 1, 0);
                }
                vertex.uv = glm::vec2(col, row);
                vertices.push_back(vertex);
            }
        }

        std::vector<unsigned> indices = build_index_buffer(stacks, slices);
        add_cap(vertices, indices, -0.5, slices);
        return Geometry(vertices, indices);
    }

    

    Geometry create_trefoil(int stacks, int slices)
    {
        std::vector<MeshVertex> vertices;
        const size_t            slices_st    = gsl::narrow<size_t>(slices);
        const size_t            num_vertices = slices_st * gsl::narrow<size_t>(stacks);
        vertices.reserve(num_vertices);
        const auto stacks_f = gsl::narrow<float>(stacks);
        const auto slices_f = gsl::narrow<float>(slices);
        glm::vec3  center{};
        glm::vec3  min = evaluate_trefoil(0, 0);
        glm::vec3  max = min;
        for (int stack = 0; stack <= stacks; ++stack)
        {
            const float row = static_cast<float>(stack) / stacks_f;
            for (int slice = 0; slice <= slices; ++slice)
            {
                const float     col = static_cast<float>(slice) / slices_f;
                constexpr float E   = 0.01f;
                const glm::vec3 p   = evaluate_trefoil(row, col);
                const glm::vec3 u   = evaluate_trefoil(row + E, col) - p;
                const glm::vec3 v   = evaluate_trefoil(row, col + E) - p;
                const glm::vec3 n   = glm::normalize(glm::cross(v, u));
                const glm::vec2 uv{ col, row };
                vertices.emplace_back(p, n, uv);

                min.x = (p.x < min.x) ? p.x : min.x;
                min.y = (p.y < min.y) ? p.y : min.x;
                min.z = (p.z < min.z) ? p.z : min.z;

                max.x = (p.x > max.x) ? p.x : max.x;
                max.y = (p.y > max.y) ? p.y : max.x;
                max.z = (p.z > max.z) ? p.z : max.z;

                center += p;
            }
        }
        center /= static_cast<float>(num_vertices);
        const auto diff  = max - min;
        const auto scale = std::max(std::max(diff.x, diff.y), diff.z);
        for (auto& v : vertices)
        {
            v.position -= center;
            v.position /= scale;
        }
        auto indices = build_index_buffer(stacks, slices);
        return Geometry{ std::move(vertices), std::move(indices) };
    }

    void describe_meshvertex_layout(GLAttributeLayout& position, GLAttributeLayout& normal, GLAttributeLayout& uv)
    {
        position.component_type         = GLAttributeLayout::Float;
        position.component_dimension    = GLAttributeLayout::_3;
        position.normalized             = false;
        position.vertex_layout_location = 0;
        position.stride                 = sizeof(MeshVertex);
        position.offset                 = 0;
        position.relative_offset        = offsetof(MeshVertex, position);

        normal.component_type         = GLAttributeLayout::Float;
        normal.component_dimension    = GLAttributeLayout::_3;
        normal.normalized             = false;
        normal.vertex_layout_location = 1;
        normal.stride                 = sizeof(MeshVertex);
        normal.offset                 = 0;
        normal.relative_offset        = offsetof(MeshVertex, normal);

        uv.component_type         = GLAttributeLayout::Float;
        uv.component_dimension    = GLAttributeLayout::_2;
        uv.normalized             = false;
        uv.vertex_layout_location = 2;
        uv.stride                 = sizeof(MeshVertex);
        uv.offset                 = 0;
        uv.relative_offset        = offsetof(MeshVertex, uv);
    }

    SubMesh to_submesh_as_triangles(const Geometry& geometry, Material* material)
    {
        GLAttributeLayout position;
        GLAttributeLayout normal;
        GLAttributeLayout uv;
        describe_meshvertex_layout(position, normal, uv);
        SubMesh sub_mesh;
        sub_mesh.VertexArrayObj.SetPrimitivePattern(GLPrimitive::Triangles);
        sub_mesh.VertexArrayObj.AddVertexBuffer(GLVertexBuffer(std::span{ geometry.Vertices }), { position, normal, uv });
        sub_mesh.VertexArrayObj.SetIndexBuffer(GLIndexBuffer(std::span{ geometry.Indicies }));
        sub_mesh.Material = material;
        return sub_mesh;
    }

    SubMesh to_submesh_as_lines(const Geometry& geometry, Material* material)
    {
        GLAttributeLayout position;
        GLAttributeLayout normal;
        GLAttributeLayout uv;
        describe_meshvertex_layout(position, normal, uv);
        std::vector<unsigned> lines_indices = convert_to_lines_pattern(geometry.Indicies);
        SubMesh               sub_mesh;
        sub_mesh.VertexArrayObj.SetPrimitivePattern(GLPrimitive::Lines);
        sub_mesh.VertexArrayObj.AddVertexBuffer(GLVertexBuffer(std::span{ geometry.Vertices }), { position, normal, uv });
        sub_mesh.VertexArrayObj.SetIndexBuffer(GLIndexBuffer(std::span{ lines_indices }));
        sub_mesh.Material = material;
        return sub_mesh;
    }
}

namespace
{
    std::vector<graphics::MeshVertex> create_plane_vertices(int stacks, int slices)
    {
        std::vector<graphics::MeshVertex> vertices;
        const auto                        numVertices = (stacks + 1) * (slices + 1);
        vertices.reserve(static_cast<size_t>(numVertices));

        for (int stack = 0; stack < stacks + 1; stack++)
        {
            const auto row = static_cast<float>(stack) / static_cast<float>(stacks);
            for (int slice = 0; slice < slices + 1; slice++)
            {
                const auto           col = static_cast<float>(slice) / static_cast<float>(slices);
                graphics::MeshVertex v;
                v.position = glm::vec3(col - 0.5f, row - 0.5f, 0.0f);
                v.normal   = glm::vec3(0.0f, 0.0f, 1.0f);
                v.uv       = glm::vec2(col, row);
                vertices.push_back(v);
            }
        }

        return vertices;
    }

    std::vector<unsigned> build_index_buffer(int stacks, int slices)
    {
        unsigned              p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
        std::vector<unsigned> indices;

        const auto numTriangles = stacks * slices * 2;
        const auto numIndices   = numTriangles * 3;
        indices.reserve(static_cast<size_t>(numIndices));
        const auto stride = slices + 1;

        for (int stack = 0; stack < stacks; stack++)
        {
            const auto currRow = stack * stride;
            for (int slice = 0; slice < slices; slice++)
            {
                p0 = static_cast<unsigned>(currRow + slice);
                p1 = p0 + 1;
                p2 = p1 + static_cast<unsigned>(stride);

                indices.push_back(p0);
                indices.push_back(p1);
                indices.push_back(p2);

                p3 = p2;
                p4 = p3 - 1;
                p5 = p0;

                indices.push_back(p3);
                indices.push_back(p4);
                indices.push_back(p5);
            }
        }
        return indices;
    }

    std::vector<unsigned> convert_to_lines_pattern(const std::vector<unsigned>& indices)
    {
        std::vector<unsigned> linesIndices;
        unsigned              i = 0;

        if (indices.size() > 6)
        {
            const auto limit = (indices.size() % 6 == 0) ? indices.size() : indices.size() - 5;
            while (i < limit)
            {
                unsigned p0 = indices[i];
                unsigned p1 = indices[i + 1];
                unsigned p2 = indices[i + 2];
                unsigned p3 = indices[i + 3];
                unsigned p4 = indices[i + 4];
                unsigned p5 = indices[i + 5];

                if (p1 != (p0 + 1) || p3 != p2 || p4 != (p3 - 1) || p5 != p0)
                {
                    break;
                }

                linesIndices.push_back(p0);
                linesIndices.push_back(p1);
                linesIndices.push_back(p1);
                linesIndices.push_back(p2);
                linesIndices.push_back(p2);
                linesIndices.push_back(p4);
                linesIndices.push_back(p4);
                linesIndices.push_back(p0);

                i += 6;
            }
        }

        if (i < indices.size() && indices.size() - i >= 3)
        {
            const auto limit = (indices.size() % 3 == 0) ? indices.size() : indices.size() - 2;
            while (i < limit)
            {
                unsigned p0 = indices[i];
                unsigned p1 = indices[i + 1];
                unsigned p2 = indices[i + 2];

                linesIndices.push_back(p0);
                linesIndices.push_back(p1);
                linesIndices.push_back(p1);
                linesIndices.push_back(p2);
                linesIndices.push_back(p2);
                linesIndices.push_back(p0);

                i += 3;
            }
        }

        return linesIndices;
    }

    glm::vec3 evaluate_trefoil(float row, float col)
    {
        constexpr float TwoPi     = 2.0f * graphics::PI;
        constexpr float a         = 0.5f;
        constexpr float b         = 0.3f;
        constexpr float c         = 0.5f;
        constexpr float d         = 0.1f;
        const float     u         = row * 2 * TwoPi;
        const float     v         = col * TwoPi;
        const float     cos_1_5_u = std::cos(1.5f * u);
        const float     r         = a + b * cos_1_5_u;
        const float     cos_u     = std::cos(u);
        const float     sin_u     = std::sin(u);
        const float     x         = r * cos_u;
        const float     y         = r * sin_u;
        const float     sin_1_5_u = std::sin(1.5f * u);
        const float     z         = c * sin_1_5_u;

        glm::vec3 dv;
        dv.x = -1.5f * b * sin_1_5_u * cos_u - (a + b * cos_1_5_u) * sin_u;
        dv.y = -1.5f * b * sin_1_5_u * sin_u + (a + b * cos_1_5_u) * cos_u;
        dv.z = 1.5f * c * cos_1_5_u;

        const glm::vec3 q   = glm::normalize(dv);
        const glm::vec3 qvn = glm::normalize(glm::vec3(q.y, -q.x, 0));
        const glm::vec3 ww  = glm::cross(q, qvn);

        const float cos_v = std::cos(v);
        const float sin_v = std::sin(v);
        glm::vec3   range;
        range.x = x + d * (qvn.x * cos_v + ww.x * sin_v);
        range.y = y + d * (qvn.y * cos_v + ww.y * sin_v);
        range.z = z + d * ww.z * sin_v;
        return range;
    }
}
