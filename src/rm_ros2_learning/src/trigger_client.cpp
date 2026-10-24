
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/trigger.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>(
        "trigger_client"
    );

    auto client =
        node->create_client<std_srvs::srv::Trigger>(
            "check_robot"
        );

    while (!client->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(
                node->get_logger(),
                "Interrupted while waiting for service"
            );
            rclcpp::shutdown();
            return 1;
        }

        RCLCPP_INFO(
            node->get_logger(),
            "Waiting for service check_robot..."
        );
    }

    auto request =
        std::make_shared<std_srvs::srv::Trigger::Request>();

    auto future = client->async_send_request(request);

    auto result = rclcpp::spin_until_future_complete(
        node,
        future
    );

    if (result == rclcpp::FutureReturnCode::SUCCESS) {
        auto response = future.get();

        RCLCPP_INFO(
            node->get_logger(),
            "Success: %s, message: %s",
            response->success ? "true" : "false",
            response->message.c_str()
        );
    } else {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to call service"
        );
    }

    rclcpp::shutdown();

    return 0;
}
