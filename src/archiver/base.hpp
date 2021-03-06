#pragma once
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

#include "proto/epics_event.pb.h"
namespace Archiver {

void EscapePayloadString(std::string &in);

class ArchiverProto
{
public:
  virtual ~ArchiverProto() = default;

protected:
  // C.67: A polymorphic class should suppress public copy/move
  // https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c67-a-polymorphic-class-should-suppress-public-copymove
  ArchiverProto() = default;
  ArchiverProto(const ArchiverProto &) = default;
  ArchiverProto &operator=(const ArchiverProto &) = default;
  ArchiverProto(ArchiverProto &&) = default;
  ArchiverProto &operator=(ArchiverProto &&) = default;

public:
  [[nodiscard]] virtual std::string ToString() const
  {
    return GetMessage().DebugString();
  };

  [[nodiscard]] std::string SerializeToStringEscaped() const;
  [[nodiscard]] std::string SerializeToString() const;

protected:
  [[nodiscard]] virtual const google::protobuf::Message &GetMessage() const = 0;
};

struct Header
{
  std::string pvname;
  int32_t year;
  EPICS::PayloadType type;
};

struct ScalarDouble
{
  double val;
  uint32_t secondsintoyear;
  uint32_t nano;
};

template<typename TMessage>
class ArchiverGenericData : public ArchiverProto
{

public:
  explicit ArchiverGenericData(TMessage m) : m_Message(std::move(m)){};
  [[nodiscard]] std::string ToString() const override
  {
    return GetMessage().ShortDebugString();
  };

protected:
  [[nodiscard]] const google::protobuf::Message &GetMessage() const override
  {
    return m_Message;
  };

private:
  TMessage m_Message;
};

[[nodiscard]] std::unique_ptr<ArchiverGenericData<EPICS::ScalarDouble>> CreateArchiverScalarDouble(ScalarDouble data);
[[nodiscard]] std::unique_ptr<ArchiverGenericData<EPICS::PayloadInfo>> CreateArchiverPayloadInfo(Header data);

}// namespace Archiver