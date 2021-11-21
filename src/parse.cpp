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
#include <stdexcept>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#define JSON_DIAGNOSTICS 1
#include <nlohmann/json.hpp>
#include <fmt/format.h>

#include "archiver/base.hpp"
#include "archiver/utils.hpp"
#include "proto/epics_event.pb.h"

using namespace Archiver;
static void PrintInfo(const Archiver::ArchiverProto *payload)
{
  spdlog::debug("Payload Info: {}", payload->ToString());
}


struct Data
{
  std::shared_ptr<ArchiverProto> header;
  std::vector<std::string> data;
};

template<typename T, typename D>
std::string SerializeToPB(const D &d)
{
  T t;
  t.Clear();
  t.set_val(d.val);
  t.set_nano(d.nano);
  t.set_secondsintoyear(d.secondsintoyear);
  return t.SerializeAsString();
};

template<typename T>
struct D
{
  T val;
  uint32_t nano;
  uint32_t secondsintoyear;
};

template<typename T>
D<T> CreatePayloadData(const nlohmann::json &json)
{
  return {
    .val = json.at("val").get<T>(),
    .nano = json.at("nano").get<uint32_t>(),
    .secondsintoyear = json.at("secondsintoyear").get<uint32_t>(),
  };
}

std::string SerializeJsonToPB(const nlohmann::json &json, const EPICS::PayloadType type)
{
  switch (type) {

  case EPICS::PayloadType::SCALAR_DOUBLE: {
    return SerializeToPB<EPICS::ScalarDouble, D<double>>(CreatePayloadData<double>(json));
  }

  case EPICS::PayloadType::SCALAR_INT: {
    return SerializeToPB<EPICS::ScalarInt, D<int>>(CreatePayloadData<int>(json));
  }

  default:
    throw std::runtime_error(fmt::format("Payload type '{}':'{}' not implemented", type, EPICS::PayloadType_Name(type)));
  }
}

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

  const auto ParseHeader = [](const nlohmann::json &json, std::ofstream &out) {
    const auto header = json.at("header");
    const auto pvname = header.at("pvname").get<std::string>();
    const auto year = header.at("year").get<int32_t>();
    const auto type = header.at("type").get<int32_t>();
    if (!EPICS::PayloadType_IsValid(type)) {
      throw std::runtime_error(fmt::format("Invalid payload type '{}'", type));
    }

    const auto p = CreateArchiverPayloadInfo({ .pvname = pvname, .year = year, .type = static_cast<EPICS::PayloadType>(type) });
    out << p->SerializeToStringEscaped() << '\n';
  };

  const auto ParseData = [](const nlohmann::json &json, const EPICS::PayloadType type, std::ofstream &out) {
    std::vector<std::string> parsed;
    for (const auto &event : json.at("events")) {
      std::string d = SerializeJsonToPB(event, type);
      EscapePayloadString(d);
      out << d << '\n';
    }
    return parsed;
  };

  const auto ParseJson = [&ParseHeader, &ParseData](const nlohmann::json &json, std::ofstream &out) {
    const auto ParsePayloadType = [](const nlohmann::json &json) -> EPICS::PayloadType {
      const auto type = json.at("header").at("type").get<int32_t>();
      if (!EPICS::PayloadType_IsValid(type)) {
        throw std::runtime_error(fmt::format("Invalid payload type '{}'", type));
      }
      return static_cast<EPICS::PayloadType>(type);
    };

    auto type = ParsePayloadType(json);

    ParseHeader(json, out);
    ParseData(json, type, out);
  };

  const auto DumpProtocolBuffer = [&ParseJson](const std::string &outFilename, const nlohmann::json &json) {
    spdlog::info("Dumping protocol buffers into {}", outFilename);
    std::ofstream out(outFilename, std::ios::binary | std::ios::out | std::ios::app);
    if (!out) {
      throw std::runtime_error(fmt::format("Failed to open/create output file {}", outFilename));
    }

    ParseJson(json, out);

    out.close();
    spdlog::info("Protocol buffer generated succesfully '{}'", outFilename);
  };

  try {
    const auto json = LoadJsonFromFile(opt.jsonFilename);
    DumpProtocolBuffer(opt.outFilename, json);

    return 0;

  } catch (nlohmann::json::exception &e) {
    spdlog::critical("Unable to parse json, {}.", e.what());
    return -1;
  } catch (std::runtime_error &e) {
    spdlog::critical("{}", e.what());
    return -1;
  }
}
