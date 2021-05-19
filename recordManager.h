#pragma once

#include <rosbag/bag.h>
#include "settingManager.h"
#include <ros/ros.h>



class recordManager{
private:
    Status m_cur_status;
    bool m_is_recording;
    settingsManager m_settings;
    rosbag::Bag* m_bag;
public:
    recordManager() = delete;
    recordManager(settingsManager& SM , rosbag::Bag* Bag);  //assume the bag is opened with write permissions and his path is legal at SM.
    ~recordManager () = default;
    bool startRecord();
    bool stopRecord();
    Status GetStatus() const;
    bool IsRecording() const;
};