#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class StatusPublisher : public rclcpp::Node  //继承
{
public:
<<<<<<< HEAD
    StatusPublisher()
    : Node("status_publisher") //
    {
=======
    StatusPublisher()//构造函数
    : Node("status_publisher")//构造函数初始化列表   这句话的意思是调用父类 Node 的构造函数，并将节点名称设置为 "status_publisher"
    {                         //Node 需要知道节点叫什么名字。创建子类对象时，就得先把父类初始化好
                              //初始化列表可以初始化父类
>>>>>>> 2f6c040 (保存宿舍pub改动)
        publisher_ =
            this->create_publisher<std_msgs::msg::String>( //模板，创建一个发string消息的pub
                "robot_status",//话题名称
                10
            );
            //在这个节点创建一个发布者，通过这个话题发布字符串消息，把创建好的发布者保存到publisher里面

<<<<<<< HEAD
        timer_ =
            this->create_wall_timer(  
=======
        timer_ =   //下划线没意义表示成员变量
            this->create_wall_timer(
>>>>>>> 2f6c040 (保存宿舍pub改动)
                500ms,
                std::bind(
                    &StatusPublisher::timerCallback,//这里指定执行哪个成员函数，this就指定这个函数属于哪个对象
                    this
                )
            );
    }

private:
    void timerCallback()
    {
        std_msgs::msg::String msg;

        msg.data = "Robot OK";

        publisher_->publish(msg); //publisher是智能指针，publish就是把msg发送出去

        RCLCPP_INFO( //输出一条info等级的日志
            this->get_logger(),//使用当前node的logger
            "Publishing: %s",
            msg.data.c_str()//转换成c风格的字符串，作为参数在打印的时候供使用
        );
    }

    //声明一个指向 String Publisher 的 shared pointer，名字叫 publisher_
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char * argv[])//接受命令行参数
{
    rclcpp::init(argc, argv);

    auto node =
        std::make_shared<StatusPublisher>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}