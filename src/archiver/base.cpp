#include "base.hpp"
#include "fmt/format.h"
#include "proto/epics_event.pb.h"
#include "utils.hpp"

namespace Archiver {
ArchiverProtoScalarDouble::ArchiverProtoScalarDouble(ScalarDouble message) : m_Data(std::move(message)) {}

const google::protobuf::Message &ArchiverProtoScalarDouble::GetMessage() const
{
  MESSAGE.Clear();
  MESSAGE.set_nano(m_Data.nano);
  MESSAGE.set_val(m_Data.val);
  MESSAGE.set_secondsintoyear(m_Data.secondsintoyear);
  return MESSAGE;
}

std::string ArchiverProtoScalarDouble::ToString() const
{

  return fmt::format(
    "ScalarDouble({},{},{},{})",
    m_Data.val,
    m_Data.secondsintoyear,
    m_Data.nano,
    StringToHexRepr(SerializeToString()));
}
ArchiverProtoScalarDouble CreateArchiverProtoScalarDouble(ScalarDouble data)
{
  return ArchiverProtoScalarDouble(data);
}

ArchiverProtoScalarDouble CreateArchiverProtoScalarDouble(const double value, const uint32_t secondsintoyear, const uint32_t nanos)
{
  const auto data = ScalarDouble{ .val = value, .secondsintoyear = secondsintoyear, .nano = nanos };
  return ArchiverProtoScalarDouble(data);
}

std::string ArchiverProto::SerializeToStringEscaped() const
{
  std::string escaped = SerializeToString();
  EscapePayloadString(escaped);
  return escaped;
}

std::string ArchiverProto::SerializeToString() const
{
  std::string result;
  GetMessage().SerializeToString(&result);
  return result;
}

/**
  The ASCII escape character 0x1B is escaped to the following two characters 0x1B 0x01
  The ASCII newline character \n or 0x0A is escaped to the following two characters 0x1B 0x02
  The ASCII carriage return character 0x0D is escaped to the following two characters 0x1B 0x03
  */
void EscapePayloadString(std::string &in)
{
  StringReplaceInplace(in, "\x1B", "\x1B\x01");
  StringReplaceInplace(in, "\x0A", "\x1B\x02");
  StringReplaceInplace(in, "\x0D", "\x1B\x03");
}

}// namespace Archiver
