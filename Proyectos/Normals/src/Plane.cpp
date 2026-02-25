#include "Plane.h"

//Genera un plane centrado en el origen, rango X,Y en [-1,1]
std::vector<glm::vec4> Plane::createPlane(int n)
{
    std::vector<glm::vec4> verts;
    if (n <= 0) return verts;

    //Tamaño
    float step = 2.0f / float(n); //De -1 a 1
    for (int iy = 0; iy < n; ++iy) {
        for (int ix = 0; ix < n; ++ix) {
            //Esquinas
            float x0 = -1.0f + ix * step;
            float y0 = -1.0f + iy * step;
            float x1 = x0 + step;
            float y1 = y0 + step;

            //Triángulo 1
            verts.push_back(glm::vec4(x0, y0, 0.0f, 1.0f));
            verts.push_back(glm::vec4(x1, y0, 0.0f, 1.0f));
            verts.push_back(glm::vec4(x1, y1, 0.0f, 1.0f));

            //Triángulo 2
            verts.push_back(glm::vec4(x0, y0, 0.0f, 1.0f));
            verts.push_back(glm::vec4(x1, y1, 0.0f, 1.0f));
            verts.push_back(glm::vec4(x0, y1, 0.0f, 1.0f));
        }
    }

    vertexSize = static_cast<int>(verts.size());
    return verts;
}