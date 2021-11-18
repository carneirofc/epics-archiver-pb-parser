#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <docopt/docopt.h>
#include <spdlog/spdlog.h>

#define JSON_USE_IMPLICIT_CONVERSIONS 0
#define JSON_DIAGNOSTICS 1
#include <nlohmann/json.hpp>

#include "archiver/base.hpp"
#include "archiver/utils.hpp"
#include "proto/epics_event.pb.h"

#include "options.hpp"
#include "parse.hpp"

constexpr const char *DOCOPT_NAME = "Archiver Protobuff Parser";
constexpr bool DOCOPT_SHOW_HELP = true;
static constexpr auto USAGE =
  R"(Archiver Protobuff Parser
    Usage:
          main --json=filename  --out=filename [--verbose]
          main (-h | --help)
          main --version
 Options:
          -h --help         Show this screen.
          --version         Show version.
          --json=filename   Input json file
          --out=filename    Output protocol buffer
          --verbose         Enable verbose logging
)";

using namespace Archiver;


static options ParseArgs(int argc, const char **argv)
{
  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    DOCOPT_SHOW_HELP,
    DOCOPT_NAME);

  return options{
    .jsonFilename = args.at("--json").asString(),
    .outFilename = args.at("--out").asString(),
    .loggingLevel = args.at("--verbose").asBool() ? spdlog::level::debug : spdlog::level::info
  };
}

int main(int argc, const char **argv)
{
  options opt = ParseArgs(argc, argv);
  spdlog::set_level(opt.loggingLevel);

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  exit(JsonToProtobuff(opt));
}
