#pragma once
#include <string>
#include "spdlog/spdlog.h"

struct options
{
  std::string jsonFilename;
  std::string outFilename;
  spdlog::level::level_enum loggingLevel;
};
