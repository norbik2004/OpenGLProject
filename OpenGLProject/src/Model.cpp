#include "../include/Model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include "../include/Texture.h"

struct VertexHash
{
	size_t operator()(const Vertex& v) const {
		return std::hash<float>()(v.position.x) ^ std::hash<float>()(v.position.y) ^ std::hash<float>()(v.position.z);
	}
};

struct VertexEqual
{
	bool operator()(const Vertex& a, const Vertex& b) const {
		return a.position == b.position && a.normal == b.normal && a.texUV == b.texUV;
	}
};

void Model::loadFromFile(const std::string& path)
{
    std::ifstream in(path);
    if (!in) { std::cerr << "Cannot open OBJ: " << path << std::endl; return; }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vn")
        {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        }
        else if (prefix == "vt")
        {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y; // flip Y
            temp_texcoords.push_back(uv);
        }
    }

    // normalize model to fit in unit cube
    glm::vec3 center(0);
    for (auto& pos : temp_positions) center += pos;
    center /= (float)temp_positions.size();
    float maxDist = 0.0f;
    for (auto& pos : temp_positions)
    {
        pos -= center;
        maxDist = std::max(maxDist, glm::length(pos));
    }
    for (auto& pos : temp_positions) pos /= maxDist;

    // second pass: faces
    in.clear();
    in.seekg(0, std::ios::beg);

    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "f")
        {
            std::string vertexStr;
            std::vector<GLuint> faceIndices;
            while (iss >> vertexStr)
            {
                std::istringstream viss(vertexStr);
                std::string v, vt, vn;
                std::getline(viss, v, '/');
                std::getline(viss, vt, '/');
                std::getline(viss, vn, '/');

                GLuint vi = std::stoi(v) - 1;
                GLuint ti = vt.empty() ? 0 : std::stoi(vt) - 1;
                GLuint ni = vn.empty() ? 0 : std::stoi(vn) - 1;

                Vertex vert{};
                vert.position = temp_positions[vi];
                vert.normal = temp_normals.size() > 0 ? temp_normals[ni] : glm::vec3(0, 1, 0);
                vert.texUV = temp_texcoords.size() > 0 ? temp_texcoords[ti] : glm::vec2(0, 0);
                vert.color = glm::vec3(1.0f);

                vertices.push_back(vert);
                faceIndices.push_back(vertices.size() - 1);
            }

            // triangulacja
            if (faceIndices.size() >= 3)
            {
                for (size_t i = 1; i < faceIndices.size() - 1; i++)
                {
                    indices.push_back(faceIndices[0]);
                    indices.push_back(faceIndices[i]);
                    indices.push_back(faceIndices[i + 1]);
                }
            }
        }
    }

    std::vector<Texture> textures;
    meshes.emplace_back(vertices, indices, textures);

    std::cout << "Loaded OBJ: " << path
        << ", vertices: " << vertices.size()
        << ", indices: " << indices.size() << std::endl;
}

void Model::Draw(Shader& shader, Camera& camera, const glm::mat4& modelMatrix)
{
	for (auto& mesh : meshes)
	{
		shader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.getCameraMatrix()));
		glUniform1f(glGetUniformLocation(shader.getID(), "uScale"), 8.0f);

		mesh.Draw(shader, camera);
	}
}
