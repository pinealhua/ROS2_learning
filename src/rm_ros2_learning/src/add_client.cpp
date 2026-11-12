
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::chrono_literals;
using AddTwoInts = example_interfaces::srv::AddTwoInts;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>(
        "add_client"
    );

    auto client = node->create_client<AddTwoInts>(
        "add_two_ints"
    );

    // 等待 Server 上线
    while (!client->wait_for_service(1s))
    {
        if (!rclcpp::ok())
        {
            return 1;
        }

        RCLCPP_INFO(
            node->get_logger(),
            "Waiting for server..."
        );
    }

    // 创建 Request 并填写数据
    auto request = std::make_shared<AddTwoInts::Request>();

    request->a = 3;
    request->b = 5;

    // 发送请求
    auto future = client->async_send_request(request);

    // 等待响应
    auto result = rclcpp::spin_until_future_complete(
        node,
        future
    );

    if (result == rclcpp::FutureReturnCode::SUCCESS)
    {
        auto response = future.get();

        RCLCPP_INFO(
            node->get_logger(),
            "Result: %lld",
            static_cast<long long>(response->sum)
        );
    }
    else
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Service call failed"
        );
    }

    rclcpp::shutdown();

    return 0;
}
