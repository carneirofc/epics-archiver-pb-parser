#pragma once
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>

#include "proto/epics_event.pb.h"
namespace Archiver {

void EscapePayloadString(std::string &in);

class ArchiverProto
{
public:
  virtual ~ArchiverProto() = default;
  [[nodiscard]] virtual std::string ToString() const = 0;
  [[nodiscard]] std::string SerializeToStringEscaped() const;
  [[nodiscard]] std::string SerializeToString() const;

protected:
  [[nodiscard]] virtual const google::protobuf::Message &GetMessage() const = 0;
};

struct ScalarDouble
{
  double val;
  uint32_t secondsintoyear;
  uint32_t nano;
};

class ArchiverProtoScalarDouble : public ArchiverProto
{
public:
  explicit ArchiverProtoScalarDouble(ScalarDouble data);
  std::string ToString() const override;

protected:
  const google::protobuf::Message &GetMessage() const override;

private:
  ScalarDouble m_Data;
  static inline EPICS::ScalarDouble MESSAGE;
};

ArchiverProtoScalarDouble CreateArchiverProtoScalarDouble(const ScalarDouble data);
ArchiverProtoScalarDouble CreateArchiverProtoScalarDouble(const double value, const uint32_t secondsintoyear, const uint32_t nanos);

}// namespace Archiver