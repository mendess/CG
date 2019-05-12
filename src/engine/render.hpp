#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include "group.hpp"

namespace Render {

extern bool LIGHTS;

void render(int argc, char** argv, std::vector<std::string> scene);

}

#endif // __RENDER_HPP__
