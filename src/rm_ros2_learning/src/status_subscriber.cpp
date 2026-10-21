#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"


class StatusSubscriber : public rclcpp::Node
{
public:
    StatusSubscriber()
    : Node("status_subscriber")
    {
        subscription_ =
            this->create_subscription<std_msgs::msg::String>(
                "robot_status",
                10,
                //告诉ros，这边收到一个参数，就传给当前对象的statuscallback()
                std::bind( 
                    &StatusSubscriber::statusCallback,
                    this,
                    std::placeholders::_1 //ros2把传进来的第一个参数放进来
                )
            );
    }

private:
    void statusCallback(
        const std_msgs::msg::String::SharedPtr msg //这里需要msg这个参数,收到的消息参数
    )//收到的消息需要有一个生命周期管理方式，可以使用智能指针。
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Received: %s",
            msg->data.c_str()//因为msg是智能指针所以用->
        );
    }

    rclcpp::Subscription<
        std_msgs::msg::String
    >::SharedPtr subscription_;
};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node =
        std::make_shared<StatusSubscriber>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}