cmake_minimum_required(VERSION 3.8)

project(MyGRPCProject)

# Set the installation directory for gRPC and Protobuf
set(CMAKE_PREFIX_PATH "$ENV{MY_INSTALL_DIR}")


find_package(Protobuf CONFIG REQUIRED)
#set(Protobuf_PROTOC_EXECUTABLE /.local/bin/protoc)
set(Protobuf_PROTOC_EXECUTABLE "/home/raunit.jain/.local/bin/protoc")



# Find gRPC and Protobuf packages
find_package(gRPC CONFIG REQUIRED )
#find_package(Protobuf REQUIRED CONFIG)



# Include directories for generated protobuf files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Path to the proto file
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/message.proto")




# Generate gRPC code from the proto file
get_filename_component(PROTO_PATH "${PROTO_FILE}" ABSOLUTE)
get_filename_component(PROTO_DIR "${PROTO_PATH}" DIRECTORY)

set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/message.pb.cc")
set(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/message.pb.h")
set(GRPC_SRC "${CMAKE_CURRENT_BINARY_DIR}/message.grpc.pb.cc")
set(GRPC_HDR "${CMAKE_CURRENT_BINARY_DIR}/message.grpc.pb.h")


find_program(gRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin PATHS /home/raunit.jain/.local/bin)
if(NOT gRPC_CPP_PLUGIN_EXECUTABLE)
    message(FATAL_ERROR "Could not find grpc_cpp_plugin")
endif()

add_custom_command(
    OUTPUT "${PROTO_SRC}" "${PROTO_HDR}" "${GRPC_SRC}" "${GRPC_HDR}"
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
         -I "${PROTO_DIR}"
         --plugin=protoc-gen-grpc="${gRPC_CPP_PLUGIN_EXECUTABLE}"
         "${PROTO_FILE}"
    DEPENDS "${PROTO_FILE}"
)


# Create a library from the generated files
add_library(message_proto
    ${PROTO_SRC}
    ${GRPC_SRC}
)

# Link gRPC and Protobuf libraries
target_link_libraries(message_proto
    PUBLIC
    gRPC::grpc++
    protobuf::libprotobuf
)



# Add executables for client and server
add_executable(client client.cpp)
target_link_libraries(client message_proto)

# add_executable(server server.cpp)
# target_link_libraries(server message_proto)

