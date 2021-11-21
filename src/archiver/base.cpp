#include "base.hpp"
#include "fmt/format.h"
#include "proto/epics_event.pb.h"
#include "utils.hpp"

namespace Archiver {


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
  @brief The ASCII escape character 0x1B is escaped to the following two characters 0x1B 0x01
  The ASCII newline character \n or 0x0A is escaped to the following two characters 0x1B 0x02
  The ASCII carriage return character 0x0D is escaped to the following two characters 0x1B 0x03
*/
void EscapePayloadString(std::string &in)
{
  StringReplaceInplace(in, "\x1B", "\x1B\x01");
  StringReplaceInplace(in, "\x0A", "\x1B\x02");
  StringReplaceInplace(in, "\x0D", "\x1B\x03");
}

/**
  @brief Removed escaped bytes from input protocol buffer string
*/
void EscapeRemoveFromPayloadString(std::string &in)
{
  StringReplaceInplace(in, "\x1B\x03", "\x0D");
  StringReplaceInplace(in, "\x1B\x02", "\x0A");
  StringReplaceInplace(in, "\x1B\x01", "\x1B");
}

[[nodiscard]] ArchiverGenericData<EPICS::ScalarDouble> CreateArchiverScalarDouble(ScalarDouble data)
{
  EPICS::ScalarDouble m;
  m.set_nano(data.nano);
  m.set_val(data.val);
  m.set_secondsintoyear(data.secondsintoyear);
  return ArchiverGenericData(m);
};

[[nodiscard]] ArchiverGenericData<EPICS::PayloadInfo> CreateArchiverPayloadInfo(Header data)
{
  EPICS::PayloadInfo m;
  m.set_pvname(data.pvname);
  m.set_type(data.type);
  m.set_year(data.year);
  return ArchiverGenericData<EPICS::PayloadInfo>(m);
}

}// namespace Archiver
