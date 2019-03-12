#include "../dependencies/rapidxml.hpp"
#include "group.hpp"
#include "render.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace rapidxml;

Group* load(string config)
{
    ifstream file(config);
    if (!file.good()) {
        cerr << config << ": file not found" << endl;
        return NULL;
    }
    string text;
    file.seekg(0, ios::end);
    text.reserve(file.tellg());
    file.seekg(0, ios::beg);

    text.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    xml_document<> doc;
    char* t = strdup(text.data());
    Group* group;
    try {
        doc.parse<0>(t);
        group = new Group(doc.first_node("scene"));
    } catch (rapidxml::parse_error& e) {
        cout << config << ": " << e.what() << " " << endl;
        group = NULL;
    }
    doc.clear();
    free(t);
    return group;
}

int main(int argc, char** argv)
{
    string config;
    if (argc < 2) {
        config = "config.xml";
    } else if ("--help" == string(argv[1]) || "-h" == string(argv[1])) {
        cerr << argv[0] << " [config.xml]" << endl;
        return 2;
    } else {
        config = string(argv[1]);
    }
    Group* scene = load(config);
    Render::render(argc, argv, scene);
}
