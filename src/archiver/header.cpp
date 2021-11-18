#include "header.hpp"
#include "fmt/format.h"
#include "utils.hpp"

namespace Archiver {


ArchiverProtoHeader::ArchiverProtoHeader(Header data) : m_Data(std::move(data)) {}
std::string ArchiverProtoHeader::ToString() const
{
  return fmt::format(
    "PayloadInfo({},{},{})",
    m_Data.pvname,
    EPICS::PayloadType_Name(m_Data.type),
    StringToHexRepr(SerializeToString()));
}

const google::protobuf::Message &ArchiverProtoHeader::GetMessage() const
{
  MESSAGE.Clear();
  MESSAGE.set_pvname(m_Data.pvname);
  MESSAGE.set_type(m_Data.type);
  MESSAGE.set_year(m_Data.year);
  return MESSAGE;
}

ArchiverProtoHeader CreatePayloadInfo(Header data)
{
  return ArchiverProtoHeader(data);
}

}// namespace Archiver