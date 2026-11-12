
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = example_interfaces::srv::AddTwoInts;

class AddServer : public rclcpp::Node
{
public:
    AddServer()
    : Node("add_server")
    {
        service_ = this->create_service<AddTwoInts>(
            "add_two_ints",
            std::bind(
                &AddServer::addCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );
    }

private:
    void addCallback(
        const std::shared_ptr<AddTwoInts::Request> request,
        std::shared_ptr<AddTwoInts::Response> response
    )
    {
        response->sum = request->a + request->b;

        RCLCPP_INFO(
            this->get_logger(),
            "Received a calculation request"
        );
    }

    rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<AddServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}
