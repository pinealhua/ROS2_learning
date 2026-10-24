
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/trigger.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class TriggerServer : public rclcpp::Node
{
public:
    TriggerServer()
    : Node("trigger_server")
    {
        service_ = this->create_service<std_srvs::srv::Trigger>(
            "check_robot",
            std::bind(
                &TriggerServer::handleService,
                this,
                _1,
                _2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Service check_robot is ready"
        );
    }

private:
    void handleService(
        const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response)
    {
        (void)request;

        response->success = true;
        response->message = "Robot is OK";

        RCLCPP_INFO(
            this->get_logger(),
            "Received a robot status request"
        );
    }

    rclcpp::Service<
        std_srvs::srv::Trigger
    >::SharedPtr service_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<TriggerServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}
