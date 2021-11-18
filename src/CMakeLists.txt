option(CPP_STARTER_USE_IMGUI "Enable compilation of ImGui sample" OFF)

# imgui example
if(CPP_STARTER_USE_IMGUI)
  message("Using Dear ImGui")
  add_subdirectory(imgui)
endif()

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(docopt REQUIRED)
find_package(Protobuf REQUIRED)
find_package(nlohmann_json REQUIRED)

add_subdirectory(proto)
add_subdirectory(archiver)


file(GLOB main_SRC "*.cpp")
message("Main SRCs ${main_SRC}")
# Generic test that uses conan libs
add_executable(archiver_pb_parser ${main_SRC})
target_include_directories(archiver_pb_parser PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(
    archiver_pb_parser
  PRIVATE
    project_options
    project_warnings
    proto
    archiver
    docopt::docopt
    fmt::fmt
    spdlog::spdlog
    protobuf::protobuf
    nlohmann_json::nlohmann_json)

install(
    TARGETS
      archiver_pb_parser
    DESTINATION
      archiver_pb_parser
    )
