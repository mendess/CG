#include "../dependencies/rapidxml.hpp"
#include "group.hpp"
#include "parser.hpp"
#include "render.hpp"
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> args;
    for (int i = 1; i < argc; i++) {
        string a = string(argv[i]);
        if ("--help" == a || "-h" == a) {
            cerr << argv[0] << " [config.xml, ...]" << endl;
            return 2;
        }
        args.push_back(a);
    }
    bool render = true;
    for (auto it = args.begin(); it != args.end();) {
        if (*it == "-d" || *it == "--dry") {
            render = false;
            args.erase(it);
        } else {
            it++;
        }
    }
    auto scene = Parser::load(args);
    if (render)
        Render::render(argc, argv, move(scene));
}
