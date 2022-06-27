#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

bool loadObj(const std::string path, std::vector<glm::vec3> &outVertices,
             std::vector<glm::vec2> &outUVs,
             std::vector<glm::vec3> &outNormals);
