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

#include "tppi/renderable.hpp"

namespace tppi {
    class renderer {
    public:
        renderer(const renderable& renderable_);

        void operator()() const noexcept;

        void reset(const renderable& renderable_);
        void add(const renderable& renderable_);
    private:
        class renderer_implementation;
        std::unique_ptr<renderer_implementation> renderer_implementation_;
    };
}
