#include "recordManager.h"


recordManager::recordManager(settingsManager& sm , rosbag::Bag* bag):
            m_is_recording(false),m_settings(sm),m_bag(bag){
    Status init = Status();
    if(m_settings.GetTopics().empty()){
        init.level=WARN;
        init.status=NO_TOPICS;
    }
    else if(m_settings.GetStatus().level!=1){
        init.level=m_settings.GetStatus().level;
        init.status=m_settings.GetStatus().status;
    }
    else if(m_bag->getMode()==rosbag::bagmode::Read ){              
        init.level = ERROR;
        init.status=BAG_AT_READ_MODE;
    }
    else{
        init.level=OK;
        init.status=VALID;
    }
    m_cur_status=init;
};

bool recordManager::startRecord(){
    Status cur = m_settings.GetStatus();
    if(m_cur_status.level!=OK){            //maybe WARN its ok for recording ?
        return false;
    }
    else if(cur.level!=OK){      //maybe 2 its ok for recording ?
        m_cur_status.status=cur.status;
        m_cur_status.level=cur.level;
        return false;
    }
    m_is_recording=true;
    return true;
}

bool recordManager::stopRecord(){
    if(!m_is_recording){
        return false;
    }
    //maybe should unsubscribe per topic
    //m_bag->close();       - thats user responsbility
    m_is_recording=false;
    return true;
}
Status recordManager::GetStatus() const{
    return m_cur_status;
}

bool recordManager::IsRecording() const{
    return m_is_recording;
}
