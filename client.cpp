//Author: Raunit Jain
//2024


#include <iostream>
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using messenger::Messenger;
using messenger::MessageRequest;
using messenger::MessageResponse;

class MessengerClient {
public:
    MessengerClient(std::shared_ptr<Channel> channel)
        : stub_(Messenger::NewStub(channel)) {}

    std::string SendMessage(const std::string& message) {
        MessageRequest request;
        request.set_content(message);

        MessageResponse response;
        ClientContext context;

        Status status = stub_->SendMessage(&context, request, &response);

        if (status.ok()) {
            return response.content();
        } else {
            std::cerr << "RPC failed" << std::endl;
            return "Error";
        }
    }

private:
    std::unique_ptr<Messenger::Stub> stub_;
};

int main() {
    MessengerClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string message = "Hello Server!";
    std::string reply = client.SendMessage(message);
    std::cout << "Received from server: " << reply << std::endl;
    return 0;
}
