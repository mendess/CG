#include "dependencies/rapidxml.hpp"
#include "group.hpp"
#include "render.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace Parser {

using namespace std;
using namespace rapidxml;

string read_file(string config)
{
    ifstream file(config);
    if (!file.good()) {
        cerr << config << ": file not found" << endl;
    }
    string text;
    file.seekg(0, ios::end);
    text.reserve(file.tellg());
    file.seekg(0, ios::beg);

    text.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    return text;
}

unique_ptr<Group> load(vector<string> configs)
{
    vector<string> texts;
    transform(configs.begin(), configs.end(),
        std::back_inserter(texts), [](string x) { return read_file(x); });
    xml_document<char>* doc = new xml_document<char>();
    if (configs.size() > 0) {
        try {
            doc->parse<0>((char*)texts[0].data());
        } catch (rapidxml::parse_error& e) {
            cerr << configs[0] << ": " << e.what() << " in " << e.where<char>() << endl;
        }
    }
    for (size_t i = 1; i < configs.size(); i++) {
        xml_document<char> group;
        try {
            group.parse<0>((char*)texts[i].data());
            for (auto g = group.first_node()->first_node(); g != NULL; g = g->next_sibling()) {
                doc->first_node()->append_node(memory_pool<>().clone_node(g));
            }
        } catch (rapidxml::parse_error& e) {
            cerr << configs[i] << ": " << e.what() << ": " << e.where<char>() << endl;
        }
    }
    auto scene = make_unique<Group>(doc->first_node("scene"));
    doc->clear();
    delete doc;
    return scene;
}

}
