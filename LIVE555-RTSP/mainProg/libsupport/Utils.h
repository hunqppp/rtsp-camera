#ifndef __Utils_h
#define __Utils_h

#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

extern bool writeRawFile(const string data, const string& filename);
extern bool readRawFile(std::string &data, const string &filename);

extern bool readJSONFile(json &data, const std::string &filename);

#endif