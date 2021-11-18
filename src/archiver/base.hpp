#pragma once
#include <functional>
#include <iostream>
#include <sstream>
#include <cstring>

#include "proto/epics_event.pb.h"
namespace Archiver {

void EscapePayloadString(std::string &in);

class ArchiverProto
{
public:
  virtual ~ArchiverProto() = default;
  virtual std::string ToString() const = 0;
  std::string SerializeToStringEscaped() const;
  std::string SerializeToString() const;

protected:
  virtual const google::protobuf::Message &GetMessage() const = 0;
};

class ArchiverProtoScalarDouble : public ArchiverProto
{
public:
  explicit ArchiverProtoScalarDouble(EPICS::ScalarDouble message);
  std::string ToString() const override;

protected:
  const google::protobuf::Message &GetMessage() const override;

private:
  EPICS::ScalarDouble m_Message;
};

ArchiverProtoScalarDouble CreateArchiverProtoScalarDouble(double value, uint32_t secondsintoyear, uint32_t nanos);

}// namespace Archiver