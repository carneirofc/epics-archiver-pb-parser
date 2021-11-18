#include <catch2/catch.hpp>

#include "archiver/base.hpp"
#include "archiver/utils.hpp"


TEST_CASE("Replace Inplace", "[StringReplace]")
{
  REQUIRE(Archiver::StringReplace("\x01", "\x01", "\x02") == "\x02");
  REQUIRE(Archiver::StringReplace("\x01""ASD""\x021", "\x01", "\x02") == "\x02""ASD""\x021");
  REQUIRE(Archiver::StringReplace("\x01""ASD""\x021", "\x01", "\x01\x02") == "\x01\x02""ASD""\x021");
  REQUIRE(Archiver::StringReplace(" ", "B", "A") == " ");
  REQUIRE(Archiver::StringReplace("", "B", "A") == "");
  REQUIRE(Archiver::StringReplace("B", "", "A") == "B");
  REQUIRE(Archiver::StringReplace("", "", "A") == "");
  REQUIRE(Archiver::StringReplace("A", "B", "A") == "A");
  REQUIRE(Archiver::StringReplace("A", "A", "") == "");
  REQUIRE(Archiver::StringReplace(" A AA ", " ", "") == "AAA");
  REQUIRE(Archiver::StringReplace("UmaStringQualquer Uma", "Uma", "Duas") == "DuasStringQualquer Duas");
  REQUIRE(Archiver::StringReplace("", "", "") == "");
}
