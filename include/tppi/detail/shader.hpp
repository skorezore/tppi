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

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>

#include <tdpi/tdpi.h>

#include "tppi/window.hpp"

namespace tppi {
    namespace detail {
        enum class shader_type : GLenum {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };

        template<shader_type S>
        class shader {
        public:
            shader(const std::string& shader_source) : shader_handle(0), compile_status(GL_FALSE) {
                if (window::count() == 0) throw std::runtime_error("A window must be created prior to an instantiation of a 'shader' object.");

                shader_handle = glCreateShader(static_cast<GLenum>(S));

                const char* shader_source_c_str = shader_source.c_str();

                glShaderSource(shader_handle, 1, &shader_source_c_str, nullptr);
                glCompileShader(shader_handle);

                glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);

                GLint log_length = 0;
                glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);

                if (log_length > 1) {
                    info_log_.reserve(log_length);
                    glGetShaderInfoLog(shader_handle, log_length, &log_length, &info_log_.front());
                }
            }

            shader(shader<S>&& other) noexcept : shader_handle(other.shader_handle), compile_status(other.compile_status), info_log_(std::move(other.info_log_)) {
                other.shader_handle = 0;
                other.compile_status = GL_FALSE;
            }

            ~shader() { glDeleteShader(shader_handle); }

            operator bool() const noexcept { return compile_status == GL_TRUE; }
            operator GLuint() const noexcept { return shader_handle; }

            const std::string& info_log() const noexcept { return info_log_; }
        private:
            GLuint shader_handle;

            int compile_status;
            std::string info_log_;
        };

        using vertex_shader = shader<shader_type::vertex>;
        using fragment_shader = shader<shader_type::fragment>;
    }
}
