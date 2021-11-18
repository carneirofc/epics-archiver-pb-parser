#include "base.hpp"
#include "utils.hpp"
#include "proto/epics_event.pb.h"
#include "fmt/format.h"

namespace Archiver {
ArchiverProtoScalarDouble::ArchiverProtoScalarDouble(EPICS::ScalarDouble message) : m_Message(message) {}

const google::protobuf::Message &ArchiverProtoScalarDouble::GetMessage() const
{
  return m_Message;
}

std::string ArchiverProtoScalarDouble::ToString() const
{

  return fmt::format(
    "ScalarDouble({},{},{},{})",
    m_Message.val(),
    m_Message.secondsintoyear(),
    m_Message.nano(),
    StringToHexRepr(SerializeToString()));
}

ArchiverProtoScalarDouble CreateArchiverProtoScalarDouble(double value, uint32_t secondsintoyear, uint32_t nanos)
{
  EPICS::ScalarDouble proto;
  proto.Clear();
  proto.set_val(value);
  proto.set_nano(nanos);
  proto.set_secondsintoyear(secondsintoyear);
  return ArchiverProtoScalarDouble(proto);
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
