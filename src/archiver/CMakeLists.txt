file(GLOB main_SRC "*.cpp" "*.cc")
# Generic test that uses conan libs

add_library(archiver ${main_SRC})

target_include_directories(
  archiver
    PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
  archiver
    PRIVATE project_options
  project_warnings
  proto
  docopt::docopt
  fmt::fmt
  spdlog::spdlog
  protobuf::protobuf)

