#pragma once
#include <spdlog/spdlog.h>
#include <string>

struct options
{
  std::string jsonFilename;
  std::string outFilename;
  spdlog::level::level_enum loggingLevel;
};