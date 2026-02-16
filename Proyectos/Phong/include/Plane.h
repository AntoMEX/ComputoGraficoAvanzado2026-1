#pragma once
#include <vector>
#include <glm/glm.hpp>

class Plane {
public:
    int vertexSize = 0;

    //Devuelve un vector de glm::vec4 con los vértices (x,y,z,w)
    std::vector<glm::vec4> createPlane(int n);
};