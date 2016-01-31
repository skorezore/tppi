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

#include <string>
#include <vector>

#include <tdpi/tdpi.h>

#include "tppi/detail/shader.hpp"

namespace tppi {
    namespace detail {
        class glsl_program {
        public:
            glsl_program(const vertex_shader& vertex_shader_, const fragment_shader& fragment_shader_, const std::vector<std::string>& attributes);
            glsl_program(glsl_program&& other) noexcept;

            ~glsl_program();

            operator bool() const noexcept;
            operator GLuint() const noexcept;

            const std::string& info_log() const noexcept;
            const std::string& shader_info_log(shader_type type) const;

            void bind() noexcept;
        private:
            GLuint program_handle;

            GLint link_status;
            std::string info_log_;

            const std::string vertex_shader_info_log;
            const std::string fragment_shader_info_log;
        };
    }
}
