/* /~\_.-~-._,--.._
  |                ~-.._
   |     .     .        \
   |                    /
   |     --....,       | \
   /                      \
  |                        |
  |                        \
  /                         |
  \_.-._  __...___.._______/
        ~~                   */

#pragma once

#include <array>
#include <map>
#include <vector>

namespace tppi {
    struct vec2 { double x, y; };
    struct vec3 { double x, y, z; };
    struct vec4 { double x, y, z, w; }

    struct material_t {
        std::string name;

        std::array<float, 3> ambient;
        std::array<float, 3> diffuse;
        std::array<float, 3> specular;
        std::array<float, 3> transmittance;
        std::array<float, 3> emission;

        float shininess;
        float ior;
        float dissolve;

        int illum;

        std::string ambient_texname;
        std::string diffuse_texname;
        std::string specular_texname;
        std::string specular_highlight_texname;
        std::string bump_texname;
        std::string displacement_texname;
        std::string alpha_texname;
        std::map<std::string, std::string> unknown_parameter;
    };

    struct mesh_t {
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texcoords;
        std::vector<unsigned int> indices;
        std::vector<int> material_ids;
    };

    struct shape_t {
        std::string name;
        mesh_t mesh;
    };
}
