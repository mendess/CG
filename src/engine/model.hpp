#ifndef MODEL_H
#define MODEL_H

#include "../common/point.hpp"

#include <string>
#include <vector>

class Model {
private:
    std::vector<Point> points;
    char* modelName;

public:
    Model(char*);
    Model(const Model& other);
    bool draw() const;
    char* name() const { return modelName; };
    ~Model() { free(modelName); };
};

#endif //MODEL_H
