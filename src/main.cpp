#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <docopt/docopt.h>

#include "archiver/header.hpp"
#include "archiver/base.hpp"
#include "archiver/utils.hpp"
#include "proto/epics_event.pb.h"

constexpr const char *DOCOPT_NAME = "Archiver Protobuff Parser";
constexpr bool DOCOPT_SHOW_HELP = true;
static constexpr auto USAGE =
  R"(Archiver Protobuff Parser
    Usage:
          main --json=filename
          main (-h | --help)
          main --version
 Options:
          -h --help         Show this screen.
          --version         Show version.
          --json=filename   Input json file
)";

using namespace Archiver;


static void PrintInfo(const Archiver::ArchiverProto &payload)
{
  // spdlog::info("Payload Info: {}", payload.ToString());
  spdlog::info("Original: {}", StringToHexRepr(payload.SerializeToString()));
  spdlog::info("Escaped:  {}", StringToHexRepr(payload.SerializeToStringEscaped()));
}

struct options
{
  std::string jsonFilename;
};

static options ParseArgs(int argc, const char **argv)
{
  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    DOCOPT_SHOW_HELP,
    DOCOPT_NAME);

  return options{ .jsonFilename = args.at("--json").asString() };
}

std::string slurp(std::ifstream &in)
{
  std::ostringstream sstr;
  sstr << in.rdbuf();
  return sstr.str();
}


int main(int argc, const char **argv)
{
  options opt = ParseArgs(argc, argv);
  spdlog::set_level(spdlog::level::debug);

  spdlog::info("Loading json from file {}", opt.jsonFilename);

  const auto LoadJsonFromFile = [](const std::string &filename) -> nlohmann::json {
    std::ifstream ifs(filename);

    if (!ifs) {
      spdlog::error("Failed to open json file {}", filename);
      exit(-1);
    }

    std::stringstream ss;
    /*
    std::string line;
    if (inpStream.is_open()) {

      while (!inpStream.eof()) {
        std::getline(inpStream, line);
        ss << line;
      }
    }*/
    nlohmann::json json;
    ss << ifs.rdbuf();
    ss >> json;
    ifs.close();
    return json;
  };

  const auto json = LoadJsonFromFile(opt.jsonFilename);
  spdlog::debug("Json dump'{}'", json.dump());


  GOOGLE_PROTOBUF_VERIFY_VERSION;
  const auto payload = CreatePayloadInfo("Test:PV:Name-Mon", 2021, EPICS::PayloadType::SCALAR_DOUBLE);
  PrintInfo(payload);
  PrintInfo(CreateArchiverProtoScalarDouble(121.101, 1, 0));
  PrintInfo(CreateArchiverProtoScalarDouble(122.101, 2, 0));
  PrintInfo(CreateArchiverProtoScalarDouble(123.101, 3, 0));
  PrintInfo(CreateArchiverProtoScalarDouble(124.101, 4, 0));
  PrintInfo(CreateArchiverProtoScalarDouble(125.101, 5, 0));
  PrintInfo(CreateArchiverProtoScalarDouble(126.101, 6, 0));
}
