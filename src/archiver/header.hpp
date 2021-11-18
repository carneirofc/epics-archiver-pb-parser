#pragma once
#include <iostream>

#include "proto/epics_event.pb.h"

#include "base.hpp"

namespace Archiver {

struct Header
{
  std::string pvname;
  uint32_t year;
  EPICS::PayloadType type;
};

class ArchiverProtoHeader : public ArchiverProto
{
public:
  explicit ArchiverProtoHeader(Header data);
  std::string ToString() const override;

protected:
  const google::protobuf::Message &GetMessage() const override;

private:
  const Header m_Data;
  static inline EPICS::PayloadInfo MESSAGE;
};

ArchiverProtoHeader CreatePayloadInfo(Header);

}// namespace Archiver