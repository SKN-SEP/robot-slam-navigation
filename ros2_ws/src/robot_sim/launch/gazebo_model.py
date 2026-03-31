import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    pkg_name = 'robot_sim'
    pkg_share = get_package_share_directory(pkg_name)
    
    xacro_file = os.path.join(pkg_share, 'model', 'robot.xacro')
    bridge_params = os.path.join(pkg_share, 'parameters', 'bridge_parameters.yaml')
    
    # Setup the world path
    default_world_path = os.path.join(pkg_share, 'worlds', 'sim_env.sdf')

    # Tell Gazebo where to find package resources
    gz_resource_path = os.environ.get('GZ_SIM_RESOURCE_PATH', '')
    os.environ['GZ_SIM_RESOURCE_PATH'] = pkg_share + ':' + gz_resource_path

    world_arg = DeclareLaunchArgument(
        'world',
        default_value=default_world_path,
        description='Full path to World file'
    )

    robot_description_raw = Command(['xacro ', xacro_file])

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description_raw,
            'use_sim_time': True
        }]
    )

    # Use LaunchConfiguration to properly pass the string
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(get_package_share_directory('ros_gz_sim'), 'launch', 'gz_sim.launch.py')
        ]),
        launch_arguments={
            'gz_args': [LaunchConfiguration('world'), ' -r']
        }.items(),
    )

    gz_spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=['-topic', 'robot_description',
                   '-name', 'mobile_robot',
                   '-allow_renaming', 'true'],
    )

    gz_ros_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{'config_file': bridge_params}],
        output='screen'
    )

    return LaunchDescription([
        world_arg,
        node_robot_state_publisher,
        gz_sim,
        gz_spawn_entity,
        gz_ros_bridge
    ])