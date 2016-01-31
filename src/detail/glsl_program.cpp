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

#include <string>
#include <stdexcept>
#include <utility>

#include "tppi/detail/glsl_program.hpp"

using namespace tppi;
using namespace tppi::detail;

glsl_program::glsl_program(const vertex_shader& vertex_shader_, const fragment_shader& fragment_shader_, const std::vector<std::string>& attributes) : program_handle(glCreateProgram()), link_status(GL_FALSE), info_log_(), vertex_shader_info_log(vertex_shader_.info_log()), fragment_shader_info_log(fragment_shader_.info_log()) {
    glAttachShader(program_handle, vertex_shader_);
    glAttachShader(program_handle, fragment_shader_);

    for (auto& attribute : attributes) {
        glBindAttribLocation(program_handle, &attribute - &attributes.front(), attribute.c_str());
    }

    glLinkProgram(program_handle);

    glDetachShader(program_handle, vertex_shader_);
    glDetachShader(program_handle, fragment_shader_);

    glGetProgramiv(program_handle, GL_LINK_STATUS, &link_status);

    GLint log_length = 0;
    glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 1) {
        info_log_.reserve(log_length);
        glGetProgramInfoLog(program_handle, log_length, &log_length, &info_log_[0]);
    }
}

glsl_program::glsl_program(glsl_program&& other) noexcept : program_handle(other.program_handle), link_status(other.link_status), info_log_(std::move(other.info_log_)), vertex_shader_info_log(std::move(other.vertex_shader_info_log)), fragment_shader_info_log(std::move(other.fragment_shader_info_log)) {
    other.program_handle = 0;
    other.link_status = GL_FALSE;
}

glsl_program::~glsl_program() { glDeleteProgram(program_handle); }

glsl_program::operator bool() const noexcept { return link_status == GL_TRUE; }
glsl_program::operator GLuint() const noexcept { return program_handle; }

const std::string& glsl_program::info_log() const noexcept { return info_log_; }

const std::string& glsl_program::shader_info_log(shader_type type) const {
    switch (type) {
        case shader_type::vertex: return vertex_shader_info_log;
        case shader_type::fragment: return fragment_shader_info_log;

        default: throw std::invalid_argument(std::to_string(static_cast<GLenum>(type)) + " is not a valid shader_type.");
    }
}

void glsl_program::bind() noexcept { glUseProgram(program_handle); }
