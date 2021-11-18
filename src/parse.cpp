#include "parse.hpp"

#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#define JSON_DIAGNOSTICS 1
#include <nlohmann/json.hpp>

#include "archiver/base.hpp"
#include "archiver/utils.hpp"
#include "proto/epics_event.pb.h"

using namespace Archiver;
static void PrintInfo(const Archiver::ArchiverProto *payload)
{
  spdlog::debug("Payload Info: {}", payload->ToString());
}

std::string slurp(std::ifstream &in)
{
  std::ostringstream sstr;
  sstr << in.rdbuf();
  return sstr.str();
}

struct Data
{
  std::unique_ptr<ArchiverProto> header;
  std::vector<std::unique_ptr<ArchiverProto>> data;
};

int JsonToProtobuff(const options &opt)
{

  spdlog::info("Loading json from file '{}'", opt.jsonFilename);
  const auto LoadJsonFromFile = [](const std::string &filename) -> nlohmann::json {
    std::ifstream ifs(filename);

    if (!ifs) {
      throw std::runtime_error(fmt::format("Failed to open json file '{}'", filename));
    }

    std::stringstream ss;
    nlohmann::json json;
    ss << ifs.rdbuf();
    ss >> json;
    ifs.close();
    spdlog::debug("Json dump'{}'", json.dump(2));
    return json;
  };

  const auto ParseHeader = [](const nlohmann::json &json) -> std::unique_ptr<ArchiverProto> {
    const auto header = json.at("header");
    const auto pvname = header.at("pvname").get<std::string>();
    const auto year = header.at("year").get<uint32_t>();
    const auto type = header.at("type").get<uint32_t>();
    return CreateArchiverPayloadInfo({ .pvname = pvname, .year = year, .type = static_cast<EPICS::PayloadType>(type) });
  };

  const auto ParseData = [](const nlohmann::json &json) -> std::vector<std::unique_ptr<ArchiverProto>> {
    std::vector<std::unique_ptr<ArchiverProto>> parsed;
    for (const auto &event : json.at("events")) {
      parsed.push_back(
        CreateArchiverScalarDouble(
          { .val = event.at("val").get<double>(),
            .secondsintoyear = event.at("secondsintoyear").get<uint32_t>(),
            .nano = event.at("nano").get<uint32_t>() }));
    }
    return parsed;
  };

  const auto ParseJson = [&ParseHeader, &ParseData](const nlohmann::json &json) -> Data {
    return {
      .header = ParseHeader(json),
      .data = ParseData(json)
    };
  };

  const auto DumpProtocolBuffer = [](const std::string &outFilename, const Data &data) {
    spdlog::info("Dumping protocol buffers into {}", outFilename);
    std::ofstream out(outFilename);
    if (!out) {
      throw std::runtime_error(fmt::format("Failed to open/create output file {}", outFilename));
    }

    out << data.header->SerializeToStringEscaped();
    out << '\n';
    for (const auto &d : data.data) {
      out << d->SerializeToStringEscaped() << '\n';
    }
    out.close();
    spdlog::info("Protocol buffer generated succesfully '{}'", outFilename);
  };

  try {
    const auto json = LoadJsonFromFile(opt.jsonFilename);
    const auto data = ParseJson(json);
    PrintInfo(data.header.get());
    for (const auto &e : data.data) {
      PrintInfo(e.get());
    }
    DumpProtocolBuffer(opt.outFilename, data);

    return 0;

  } catch (nlohmann::json::exception &e) {
    spdlog::critical("Unable to parse json, {}.", e.what());
    return -1;
  } catch (std::runtime_error &e) {
    spdlog::critical("{}", e.what());
    return -1;
  }
}