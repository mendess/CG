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
    bool draw_model() const;
    char* name() const { return modelName; };
    ~Model() { free(modelName); };
};

class Models {
private:
    std::vector<Model> models;
    Models() {};

public:
    static Models* get()
    {
        static Models m;
        return &m;
    }
    int num_models() { return models.size(); }
    const Model* get_model(int i) { return &models[i]; };
    void add_model(Model m) { models.push_back(m); }
    static bool load(std::string config);
};

#endif //MODEL_H
