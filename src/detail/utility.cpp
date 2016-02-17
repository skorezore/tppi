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

#include <fstream>
#include <sstream>

#include "tppi/detail/utility.hpp"

using namespace tppi::detail::utility;

std::string file_to_string(const std::string& path) { return static_cast<std::stringstream&>(std::stringstream() << std::ifstream(path).rdbuf()).str(); } // Gotta love this atrocity.
