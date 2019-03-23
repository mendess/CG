#include "../dependencies/rapidxml.hpp"
#include "group.hpp"
#include "render.hpp"
#include "parser.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    string config;
    for (int i = 1; i < argc; i++) {
        string a = string(argv[i]);
        if ("--help" == a || "-h" == a) {
            cerr << argv[0] << " [config.xml, ...]" << endl;
            return 2;
        }
    }
    vector<string> configs;
    for (int i = 1; i < argc; i++) {
        configs.push_back(string(argv[i]));
    }
    Group* scene = Parser::load(configs);
    Render::render(argc, argv, scene);
}
