#include "utils.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <cstring>

#include "fmt/format.h"

namespace Archiver {

std::string StringToHexRepr(const std::string &in)
{
  std::stringstream sPayload;
  sPayload << '[';
  for (const auto &c : in) {
    sPayload << fmt::format("0x{:02x} ", static_cast<uint8_t>(c));
  }
  sPayload << ']';
  return sPayload.str();
}

std::string StringReplace(const std::string &source, const std::string &from, const std::string &replacement)
{
  std::string newString;
  newString.reserve(source.length());// avoids a few memory allocations
  if (from == "") {
    return source;
  }

  std::string::size_type lastPos = 0;
  std::string::size_type subStrIndex = 0;

  while (std::string::npos != (subStrIndex = source.find(from, lastPos))) {
    newString.append(source, lastPos, subStrIndex - lastPos);
    newString += replacement;
    lastPos = subStrIndex + from.length();
  }

  // Care for the rest after last occurrence
  newString += source.substr(lastPos);
  return newString;
}

void StringReplaceInplace(std::string &source, const std::string &from, const std::string &replacement)
{
  source.assign(StringReplace(source, from, replacement));
}
}// namespace Archiver