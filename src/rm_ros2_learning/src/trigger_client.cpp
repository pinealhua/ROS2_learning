
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
        //客户端必须与服务端一致

    while (!client->wait_for_service(1s)) {//client可能比server先启动，直接发请求可能会失败
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
        std::make_shared<std_srvs::srv::Trigger::Request>();//request没有字段，不用填内容

    auto future = client->async_send_request(request);//异步，future暂时为空，等server处理完之后就有response

    auto result = rclcpp::spin_until_future_complete(
        node,
        future
    );  //让node继续处理ros事件，直到future里面的响应回来

    if (result == rclcpp::FutureReturnCode::SUCCESS) {
        auto response = future.get(); //从future里面读到真实的响应

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
