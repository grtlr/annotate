#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H

#include <fstream>
#include <set>

#include "label.h"

// TODO this should be replaced by a proper config parser
//  Qt5 offers JSON support, so this could be an alternative.
std::set<Label> readLabelsFromFile (const std::string & file)
{
    using namespace std;
    set<Label> labels;
    string line, tcolor;
    unsigned int tr, tg, tb;

    // these 2 labels are hardcoded and are always present
    labels.emplace(Label{"Background",  0,   0,   0});
    labels.emplace(Label{"Mixed",     128, 128, 128});

    ifstream ifs(file);
    while(getline(ifs, line))
    {
        istringstream is(line);
        is >> tcolor;
        is >> tr;
        is >> tg;
        is >> tb;
        labels.emplace(Label{tcolor,
                static_cast<unsigned char>(tr),
                static_cast<unsigned char>(tg),
                static_cast<unsigned char>(tb)});
    }
    return labels;
}

#endif // PARSE_CONFIG_H
