#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include "group.hpp"

namespace Render {

void render(int argc, char** argv, std::unique_ptr<Group> scene);

}

#endif // __RENDER_HPP__
