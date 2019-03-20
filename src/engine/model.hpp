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
    bool loaded();
    bool draw() const;
    bool draw_random() const;
    char* name() const { return modelName; };
    ~Model() { if(modelName) free(modelName); };
};

#endif //MODEL_H
