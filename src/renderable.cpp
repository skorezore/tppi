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

#include <tiny_obj_loader.h>

#include "tppi/detail/utility.hpp"
#include "tppi/renderable.hpp"
#include "tppi/window.hpp"

using namespace tppi;
using namespace tppi::detail;

class renderable::renderable_implementation {
public:
    renderable_implementation(const std::string& path) : load_status(tinyobj::LoadObj(shapes, materials, error_string_, utility::file_to_string(path).c_str())) {
        if (window::count() == 0) throw std::runtime_error("A window must be created prior to an instantiation of a 'renderable' object.");
    }

    const coordinate_3d& coordinates() const noexcept { return coordinates_; }
    void coordinates(const coordinate_3d& new_coordinates) noexcept { coordinates_ = new_coordinates; }

    operator bool() const noexcept { return load_status; }
    const std::string& error_string() const noexcept { return error_string_; }

    friend class renderable;
private:
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    coordinate_3d coordinates_;

    bool load_status;
    std::string error_string_;
};

renderable::renderable(const std::string& path) : renderable_implementation_(std::make_unique<renderable_implementation>(path)) {
    for (const auto& tinyobj_shape : renderable_implementation_->shapes) { shapes.push_back({tinyobj_shape.name, {tinyobj_shape.mesh.positions, tinyobj_shape.mesh.normals, tinyobj_shape.mesh.texcoords, tinyobj_shape.mesh.indices, tinyobj_shape.mesh.material_ids}}); }
    for (const auto& tinyobj_material : renderable_implementation_->materials) { materials.push_back({tinyobj_material.name, {tinyobj_material.ambient[0], tinyobj_material.ambient[1], tinyobj_material.ambient[2]}, {tinyobj_material.diffuse[0], tinyobj_material.diffuse[1], tinyobj_material.diffuse[2]}, {tinyobj_material.specular[0], tinyobj_material.specular[1], tinyobj_material.specular[2]}, {tinyobj_material.transmittance[0], tinyobj_material.transmittance[1], tinyobj_material.transmittance[2]}, {tinyobj_material.emission[0], tinyobj_material.emission[1], tinyobj_material.emission[2]}, tinyobj_material.shininess, tinyobj_material.ior, tinyobj_material.dissolve, tinyobj_material.illum, tinyobj_material.ambient_texname, tinyobj_material.diffuse_texname, tinyobj_material.specular_texname, tinyobj_material.specular_highlight_texname, tinyobj_material.bump_texname, tinyobj_material.displacement_texname, tinyobj_material.alpha_texname, tinyobj_material.unknown_parameter}); }
}

const coordinate_3d& renderable::coordinates() const noexcept { return renderable_implementation_->coordinates(); }
void renderable::coordinates(const coordinate_3d& new_coordinates) noexcept { renderable_implementation_->coordinates(new_coordinates); }

renderable::operator bool() const noexcept { return static_cast<bool>(renderable_implementation_); }
const std::string& renderable::error_string() const noexcept { return renderable_implementation_->error_string(); }
