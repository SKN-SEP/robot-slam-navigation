#include "wave_rover_controller/WaveRoverControllerNode.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "wave_rover_controller/InternalApi.hpp"
#include "wave_rover_controller/Types.hpp"
#include <rclcpp/logging.hpp>
#include <tf2/LinearMath/Quaternion.hpp>

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WRController::WaveRoverControllerNode>());
    rclcpp::shutdown();
    return 0;
}

WRController::WaveRoverControllerNode::WaveRoverControllerNode::WaveRoverControllerNode() : Node("wave_rover_controller") {
    this->declare_parameter("linear_speed", 0.75);
    this->declare_parameter("angular_speed", 1.5);
    this->declare_parameter("loop_frequency", 50.0);
    this->declare_parameter("imu_loop_frequency", 25.0);
    this->declare_parameter("teleop_timeout", 0.5);

    const double ls = this->get_parameter("linear_speed").as_double();
    const double as = this->get_parameter("angular_speed").as_double();
    const double freq = this->get_parameter("loop_frequency").as_double();
    const double imu_freq = this->get_parameter("imu_loop_frequency").as_double();
    const double timeout = this->get_parameter("teleop_timeout").as_double();

    this->interpreter = std::make_unique<JoystickInputInterpreter>();
    this->motion = std::make_unique<MotionFactory>(ls, as);
    this->stateMachine = std::make_unique<StateMachine>(timeout);
    this->driver = std::make_unique<SerialDriver>(
        "/dev/ttyAMA0",
        B115200
    );

    if (!this->driver->Connect()) {
        RCLCPP_ERROR(this->get_logger(), "Failed to connect to WaveRover serial port /dev/ttyAMA0.");
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

    this->imuPub = this->create_publisher<sensor_msgs::msg::Imu>(
        "/wave_rover/imu/raw", 10
    );

    this->timer = this->create_wall_timer(
        std::chrono::milliseconds(static_cast<int>(1000.0 / freq)), std::bind(
            &WaveRoverControllerNode::Loop, this
        )
    );

    this->imuTimer = this->create_wall_timer(
        std::chrono::milliseconds(static_cast<int>(1000.0 / imu_freq)), std::bind(
            &WaveRoverControllerNode::ImuLoop, this
        )
    );
    
    RCLCPP_INFO(this->get_logger(), "Wave Rover Controller Node has been initialized.");
}

void WRController::WaveRoverControllerNode::JoyCb(const sensor_msgs::msg::Joy::SharedPtr msg) {
    this->lastInput.axes = msg->axes;
    this->lastInput.buttons = msg->buttons;
}

void WRController::WaveRoverControllerNode::PublishCmd(const Motion & cmd) {
    geometry_msgs::msg::Twist msg;

    msg.linear.x = cmd.vx;
    msg.angular.z = cmd.wz;

    this->cmdPub->publish(msg);

    this->driver->SendCommand(cmd);
}

void WRController::WaveRoverControllerNode::PublishCmd(const ImuData & data) {
    sensor_msgs::msg::Imu msg;

    tf2::Quaternion quaternion;

    quaternion.setRPY(data.roll * DEG_TO_RAD, data.pitch * DEG_TO_RAD, data.yaw * DEG_TO_RAD);

    msg.orientation.x = quaternion.x();
    msg.orientation.y = quaternion.y();
    msg.orientation.z = quaternion.z();
    msg.orientation.w = quaternion.w();

    msg.angular_velocity.x = data.gx;
    msg.angular_velocity.y = data.gy;
    msg.angular_velocity.z = data.gz;

    msg.linear_acceleration.x = data.ax;
    msg.linear_acceleration.y = data.ay;
    msg.linear_acceleration.z = data.az;

    this->imuPub->publish(msg);
}

void WRController::WaveRoverControllerNode::Loop() {
    const rclcpp::Time now = this->now();

    Input input = this->interpreter->Interpret(this->lastInput);

    this->stateMachine->Update(input.controllerPressed, input.valid, now);

    State state = stateMachine->GetState(now);

    if (state != State::TELEOP) {
        this->PublishCmd(Motion{});

        return;
    }

    Motion cmdMotion = this->motion->Create(input);
    CmdIMUReceive cmdImu;
    
    this->PublishCmd(cmdMotion);
    this->driver->SendCommand(cmdImu);
}

void WRController::WaveRoverControllerNode::ImuLoop() {
    CmdIMUReceive cmd;

    this->driver->SendCommand(cmd);

    std::string resp = this->driver->ReadResponse();

    if (resp.empty()) return;

    ImuData imu;
    
    imu.roll = WRController::InternalApi::Extract(resp, "\"r\"");
    imu.pitch = WRController::InternalApi::Extract(resp, "\"p\"");
    imu.yaw = WRController::InternalApi::Extract(resp, "\"y\"");

    imu.ax = WRController::InternalApi::Extract(resp, "\"ax\"") * WRController::DEG_TO_RAD;
    imu.ay = WRController::InternalApi::Extract(resp, "\"ay\"") * WRController::DEG_TO_RAD;
    imu.az = WRController::InternalApi::Extract(resp, "\"az\"") * WRController::DEG_TO_RAD;

    imu.gx = WRController::InternalApi::Extract(resp, "\"gx\"") * WRController::M_S2_FACTOR;
    imu.gy = WRController::InternalApi::Extract(resp, "\"gy\"") * WRController::M_S2_FACTOR;
    imu.gz = WRController::InternalApi::Extract(resp, "\"gz\"") * WRController::M_S2_FACTOR;

    this->PublishCmd(imu);
}