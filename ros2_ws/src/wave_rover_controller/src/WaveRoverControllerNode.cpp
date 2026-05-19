#include "wave_rover_controller/WaveRoverControllerNode.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WVController::WaveRoverControllerNode>());
    rclcpp::shutdown();
    return 0;
}

WVController::WaveRoverControllerNode::WaveRoverControllerNode::WaveRoverControllerNode() : Node("wave_rover_controller") {
    this->declare_parameter("linear_speed", 0.75);
    this->declare_parameter("angular_speed", 1.5);
    this->declare_parameter("loop_frequency", 50.0);
    this->declare_parameter("teleop_timeout", 0.5);

    const double ls = this->get_parameter("linear_speed").as_double();
    const double as = this->get_parameter("angular_speed").as_double();
    const double freq = this->get_parameter("loop_frequency").as_double();
    const double timeout = this->get_parameter("teleop_timeout").as_double();

    this->interpreter = std::make_unique<JoystickInputInterpreter>();
    this->motion = std::make_unique<MotionFactory>(ls, as);
    this->stateMachine = std::make_unique<StateMachine>(timeout);
    this->driver = std::make_unique<SerialDriver>(
        "/dev/ttyUSB0",
        B115200
    );

    if (!this->driver->Connect()) {
        RCLCPP_ERROR(this->get_logger(), "Failed to connect to WaveRover serial port.");
    }

    this->joySub = this->create_subscription<sensor_msgs::msg::Joy>(
        "joy", 10, std::bind(
            &WaveRoverControllerNode::JoyCb,
            this,
            std::placeholders::_1
        )
    );

    this->cmdPub = this->create_publisher<geometry_msgs::msg::Twist>(
        "/wave_rover/cmd_vel", 10
    );

    this->timer = this->create_wall_timer(
        std::chrono::milliseconds(static_cast<int>(1000.0 / freq)), std::bind(
            &WaveRoverControllerNode::Loop, this
        )
    );
    
    RCLCPP_INFO(this->get_logger(), "Wave Rover Controller Node has been initialized.");
}

void WVController::WaveRoverControllerNode::JoyCb(const sensor_msgs::msg::Joy::SharedPtr msg) {
    this->lastInput.axes = msg->axes;
    this->lastInput.buttons = msg->buttons;
}

void WVController::WaveRoverControllerNode::PublishCmd(const Motion & cmd) {
    geometry_msgs::msg::Twist msg;

    msg.linear.x = cmd.vx;
    msg.angular.z = cmd.wz;

    this->cmdPub->publish(msg);

    this->driver->SendCommand(cmd);
}

void WVController::WaveRoverControllerNode::Loop() {
    const rclcpp::Time now = this->now();

    Input input = this->interpreter->Interpret(this->lastInput);

    this->stateMachine->Update(input.controllerPressed, input.valid, now);

    State state = stateMachine->GetState(now);

    if (state != State::TELEOP) {
        this->PublishCmd(Motion{});

        return;
    }

    Motion cmd = this->motion->Create(input);

    this->PublishCmd(cmd);
}
