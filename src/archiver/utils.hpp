#pragma once

#include <string>

namespace Archiver {

std::string StringToHexRepr(const std::string &in);
std::string StringReplace(const std::string &source, const std::string &from, const std::string &replacement);
void StringReplaceInplace(std::string &source, const std::string &from, const std::string &replacement);
}// namespace Archiver