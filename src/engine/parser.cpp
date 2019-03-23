#include "../dependencies/rapidxml.hpp"
#include "group.hpp"
#include "render.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
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

xml_document<char>* merge_docs(vector<string> configs)
{
    xml_document<char>* doc = new xml_document<char>();
    if (configs.size() > 0) {
        string text = read_file(configs[0]);
        try {
            doc->parse<0>(strdup(text.data()));
        } catch (rapidxml::parse_error& e) {
            cerr << configs[0] << ": " << e.what() << ": " << e.where<char>() << endl;
        }
    }
    for (size_t i = 1; i < configs.size(); i++) {
        string config = configs[i];
        string text = read_file(config);
        xml_document<char> group;
        try {
            group.parse<0>(strdup(text.data()));
            for (auto g = group.first_node()->first_node(); g != NULL; g = g->next_sibling()) {
                doc->first_node()->append_node(memory_pool<>().clone_node(g));
            }
        } catch (rapidxml::parse_error& e) {
            cerr << config << ": " << e.what() << ": " << e.where<char>() << endl;
        }
    }
    return doc;
}

Group* load(vector<string> configs)
{
    xml_document<>* doc = merge_docs(configs);
    Group* group;
    group = new Group(doc->first_node("scene"));
    doc->clear();
    return group;
}

}
