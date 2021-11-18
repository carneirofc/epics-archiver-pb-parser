#include "header.hpp"
#include "utils.hpp"
#include "fmt/format.h"

namespace Archiver {


ArchiverProtoHeader::ArchiverProtoHeader(EPICS::PayloadInfo message) : m_Message(std::move(message)) {}
std::string ArchiverProtoHeader::ToString() const
{
  return fmt::format(
    "PayloadInfo({},{},{})",
    m_Message.pvname(),
    EPICS::PayloadType_Name(m_Message.type()),
    StringToHexRepr(SerializeToString()));
}

const google::protobuf::Message &ArchiverProtoHeader::GetMessage() const
{
  return m_Message;
}

ArchiverProtoHeader CreatePayloadInfo(const char *pvname, int32_t year, EPICS::PayloadType type)
{
  std::string response;

  EPICS::PayloadInfo payloadInfo;
  payloadInfo.Clear();
  payloadInfo.set_pvname(pvname);
  payloadInfo.set_year(year);
  payloadInfo.set_type(type);
  payloadInfo.SerializeToString(&response);
  return ArchiverProtoHeader(payloadInfo);
}

}// namespace ArchiverProto