#pragma once
#include <iostream>

#include "proto/epics_event.pb.h"

#include "base.hpp"

namespace Archiver {

class ArchiverProtoHeader : public ArchiverProto
{
public:
  explicit ArchiverProtoHeader(EPICS::PayloadInfo message);
  std::string ToString() const override;

protected:
  const google::protobuf::Message &GetMessage() const override;

private:
  EPICS::PayloadInfo m_Message;
};

ArchiverProtoHeader CreatePayloadInfo(const char *pvname, int32_t year, EPICS::PayloadType type);

}// namespace Archiver