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

#include <memory>
#include <string>
#include <vector>

#include "tppi/pods.hpp"

namespace tppi {
    class renderer;

    class renderable {
    public:
        renderable(const std::string& path);

        const vec3& coordinates() const noexcept;
        void coordinates(const vec3& new_coordinates) noexcept;

        operator bool() const noexcept;
        const std::string& error_string() const noexcept;

        friend class renderer;
    private:
        class renderable_implementation;
        std::unique_ptr<renderable_implementation> renderable_implementation_;

        std::vector<shape_t> shapes;
        std::vector<material_t> materials;
    };
}
