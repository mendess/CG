#include "group.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <vector>

using namespace std;

void Group::draw() const
{
    for (const auto &transformation : transformations) {
        transformation->transform();
    }
    for (const auto &model : models) {
        model.draw();
    }
    for (const auto &subgroup : subgroups) {
        subgroup.draw();
    }
}

void Group::draw(int max_depth) const
{
    if (max_depth > 0) {
        for (const auto &transformation : transformations) {
            transformation->transform();
        }
        for (const auto &model : models) {
            model.draw();
        }
        for (const auto subgroup : subgroups) {
            subgroup.draw(max_depth - 1);
        }
    }
}
