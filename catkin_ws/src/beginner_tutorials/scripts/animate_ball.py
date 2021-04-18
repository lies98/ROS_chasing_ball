#!/usr/bin/env python
import rospy
from math import sin, cos, pi
from gazebo_msgs.msg import ModelState
import rospy

def animate():
    pub = rospy.Publisher('/gazebo/set_model_state', ModelState, queue_size=3)
    rospy.init_node('animate_ball')
    rate = rospy.Rate(20) # 15 Hz
    x1 = 0.3
    y1 = 0.1
    z = 0.12
    ballState1 = ModelState()
    ballState1.model_name = 'ball'
    ballState1.pose.orientation.x = 0
    ballState1.pose.orientation.y = 0
    ballState1.pose.orientation.z = 0
    ballState1.pose.orientation.w = 1
    ballState1.reference_frame = 'world'
    direction = 'left'

    x2 = 0.3
    y2 = 0.0
    ballState2 = ModelState()
    ballState2.model_name = 'yellow ball'
    ballState2.pose.orientation.x = 0
    ballState2.pose.orientation.y = 0
    ballState2.pose.orientation.z = 0
    ballState2.pose.orientation.w = 1
    ballState2.reference_frame = 'world'

    x3 = 0.3
    y3 = -0.1
    ballState3 = ModelState()
    ballState3.model_name = 'red ball'
    ballState3.pose.orientation.x = 0
    ballState3.pose.orientation.y = 0
    ballState3.pose.orientation.z = 0
    ballState3.pose.orientation.w = 1
    ballState3.reference_frame = 'world'

    angle = 0

    while not rospy.is_shutdown():

        ballState1.pose.position.x = x1 + angle
        ballState1.pose.position.y = y1 + angle / 2
        ballState1.pose.position.z = z


 
        ballState2.pose.position.x = x2 + angle
        ballState2.pose.position.y = y2
        ballState2.pose.position.z = z 

        ballState3.pose.position.x = x3 + angle
        ballState3.pose.position.y = y3 - angle / 2
        ballState3.pose.position.z = z

        angle += 0.005     
        
        pub.publish(ballState1)
        pub.publish(ballState2)
        pub.publish(ballState3)
        rate.sleep()

if __name__ == '__main__':
    try:
        animate()
    except rospy.ROSInterruptException:
        pass
