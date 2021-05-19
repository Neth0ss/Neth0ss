
#include "ros_type_introspection/ros_introspection.hpp"
#include <ros/ros.h>
#include <topic_tools/shape_shifter.h>
#include <std_msgs/Int32.h>
#include "recordManager.h"
#include "assert.h"
#include "settingManager.h"



using namespace RosIntrospection;
using topic_tools::ShapeShifter;


void topicCallback(const ShapeShifter::ConstPtr& msg,const std::string &topic_name,RosIntrospection::Parser& parser,rosbag::Bag& bag ) 
{           //instead of massage type try to put a generic type
    const std::string&  datatype   =  msg->getDataType();
    const std::string&  definition =  msg->getMessageDefinition();

    // don't worry if you do this more than once: already registered message are not overwritten.
    parser.registerMessageDefinition( topic_name,
                                      RosIntrospection::ROSType(datatype),
                                      definition );

    // reuse these opbects to improve efficiency ("static" makes them persistent)
    std::vector<uint8_t> buffer;
    FlatMessage   flat_container;
    RenamedValues renamed_value;

    // copy raw memory into the buffer
    buffer.resize( msg->size() );
    ros::serialization::OStream stream(buffer.data(), buffer.size());
    msg->write(stream);

    // deserialize and rename the vectors
    parser.deserializeIntoFlatContainer( topic_name, Span<uint8_t>(buffer), &flat_container, 100);
    parser.applyNameTransform( topic_name, flat_container, &renamed_value );
    //write to bag 
    for (auto it: renamed_value)
    {
        const Variant& value   = it.second;
        double a = value.convert<double>();
        std_msgs::Int32 i;
        i.data = (int)a;
        bag.write(topic_name,ros::Time::now(),2);
    }
    // for (auto it: flat_container.name)
    // {
    //     const std::string& value  = it.second;
    //     bag.write(topic_name,ros::Time::now(),value.c_str());
    // }
}


int main(int argc, char **argv){
    ros::init(argc, argv, "record_manager");
    ros::NodeHandle pnh("RM");
    rosbag::Bag test_bag; 
    test_bag.open("test.bag", rosbag::bagmode::Write);
    settingsManager SM = settingsManager("/home/makery/catkin_ws_PreRecord/src/PreRecord/src");
    std::cout<< ConvertLevel(Level(SM.GetStatus().level)) << "::" << ConvertStatus(StatusType(SM.GetStatus().status)) << std::endl;     //valid checking
    recordManager RM(SM, &test_bag);
    std::cout<< ConvertLevel(Level(RM.GetStatus().level)) << "::" << ConvertStatus(StatusType(RM.GetStatus().status)) << std::endl;     //valid checking
    ros::Rate bit_loop_rate_params(1);
    while(!RM.GetStatus().level!=1){
        std::cout<< ConvertLevel(Level(RM.GetStatus().level)) << "::" << ConvertStatus(StatusType(RM.GetStatus().status)) << std::endl;     //valid checking
            //trying to fix the problem (=recover)
    }
    assert(!RM.GetStatus().level);       //assert status is Valid 
    Parser parser;
    const std::string topic_name ;         //should be argv[2]
        //add here check 
    boost::function<void(const topic_tools::ShapeShifter::ConstPtr&) > callback;
    callback = [&parser, topic_name,&test_bag](const topic_tools::ShapeShifter::ConstPtr& msg) -> void
    {
        topicCallback(msg, topic_name, parser,test_bag) ;           //maybe we need to give a pointer to bag?
    }; 

    std::vector<std::string> topics=SM.GetTopics();
    std::vector<ros::Subscriber> subscribers;
    for (auto it = topics.begin() ; it != topics.end(); ++it){
        //make subscribe, #no need to check if its is a valid topic?.
        ros::Subscriber subscriber = pnh.subscribe(*it, 100, callback);
        subscribers.push_back(subscriber);
    }
    bool IsRecord= RM.startRecord();                //replace that with while loop for waiting the user command to start record
    if(!IsRecord){
        std::cout<< ConvertLevel(Level(RM.GetStatus().level)) << "::" << ConvertStatus(StatusType(RM.GetStatus().status)) << std::endl;     //valid checking
    }
    ros::spin();
    test_bag.close();
    return 0;


}