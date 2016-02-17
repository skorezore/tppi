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

#include <vector>

#include <tdpi/tdpi.h>

#include "tppi/renderer.hpp"

using namespace tppi;

class renderer::renderer_implementation {
public:
    renderer_implementation(const renderable& renderable_) noexcept : vertex_buffer_object(0), vertex_array_object(0) {
        glGenBuffers(1, &vertex_buffer_object);
        glGenVertexArrays(1, &vertex_array_object);

        add(renderable_);
    }

    ~renderer_implementation() {
        glDeleteBuffers(1, &vertex_buffer_object);
        glDeleteVertexArrays(1, &vertex_array_object);
    }

    void operator()() const noexcept {
        glBindVertexArray(vertex_array_object);
        glDrawArrays(GL_TRIANGLES, 0, vertex_data.size() / 8);
    }

    void reset(const renderable& renderable_) {
        vertex_data.clear();
        add(renderable_);
    }

    void add(const renderable& renderable_) {
        for (const auto& shape : renderable_.shapes) {
            for (const auto& index : shape.mesh.indices) {
                vertex_data.push_back(shape.mesh.positions.at(index * 3));
                vertex_data.push_back(shape.mesh.positions.at(index * 3 + 1));
                vertex_data.push_back(shape.mesh.positions.at(index * 3 + 2));

                vertex_data.push_back(shape.mesh.normals.at(index * 3));
                vertex_data.push_back(shape.mesh.normals.at(index * 3 + 1));
                vertex_data.push_back(shape.mesh.normals.at(index * 3 + 2));

                vertex_data.push_back(shape.mesh.texcoords.at(index * 2));
                vertex_data.push_back(shape.mesh.texcoords.at(index * 2 + 1));
            }
        }

        glBindVertexArray(vertex_array_object);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<void*>(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<void*>(6 * sizeof(float)));
    }
private:
    GLuint vertex_buffer_object;
    GLuint vertex_array_object;

    std::vector<float> vertex_data;
};

renderer::renderer(const renderable& renderable_) : renderer_implementation_(std::make_unique<renderer_implementation>(renderable_)) {}

void renderer::operator()() const noexcept { renderer_implementation_->operator()(); }

void renderer::reset(const renderable& renderable_) { renderer_implementation_->reset(renderable_); }
void renderer::add(const renderable& renderable_) { renderer_implementation_->add(renderable_); }
