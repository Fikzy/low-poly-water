#include "obj_loader.hh"

#include <fstream>
#include <regex>
#include <sstream>

bool loadObj(const std::string path, std::vector<glm::vec3> &outVertices,
             std::vector<glm::vec2> &outUVs, std::vector<glm::vec3> &outNormals)
{
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    std::ifstream file(path);
    if (!file)
    {
        printf("Unbale to open file %s\n", path.c_str());
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineSS(line);

        std::string header;
        lineSS >> header;

        if (header.compare("#") == 0)
        {
            continue;
        }
        if (header.compare("v") == 0)
        {
            glm::vec3 vertex;
            lineSS >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (header.compare("vt") == 0)
        {
            glm::vec2 uv;
            lineSS >> uv.x >> uv.y;
            tempUVs.push_back(uv);
        }
        else if (header.compare("vn") == 0)
        {
            glm::vec3 normal;
            lineSS >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (header.compare("f") == 0)
        {
            std::string face;
            unsigned int vertexIndex, uvIndex, normalIndex;

            while (lineSS >> face)
            {
                std::istringstream faceSS(face);

                if (faceSS >> vertexIndex)
                    outVertices.push_back(tempVertices[vertexIndex - 1]);

                faceSS.ignore(1);

                if (faceSS >> uvIndex)
                    outUVs.push_back(tempUVs[uvIndex - 1]);

                faceSS.clear();
                faceSS.ignore(1);

                if (faceSS >> normalIndex)
                    outNormals.push_back(tempNormals[normalIndex - 1]);
            }
        }
    }

    file.close();

    return true;
}
