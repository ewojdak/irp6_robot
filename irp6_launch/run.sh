#!/bin/bash

rosrun xacro xacro.py irp6p.xml.xacro -o example.xml
export RTT_COMPONENT_PATH=/opt/ros/cturtle/stacks/orocos_toolchain_ros/rtt/install/lib/orocos 
`rospack find ocl`/bin/deployer-gnulinux -s example.xml
