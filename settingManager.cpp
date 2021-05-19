
#include "settingManager.h"
#include <sys/types.h>
#include <sys/stat.h>


std::vector<std::string> settingsManager::ParseFile(std::string fileName)
{
	std::vector<std::string> topics ;
	topics.push_back("/test");
	return topics;
}



bool settingsManager::isPathValid(std::string path){
	Status init = Status();
	if(path.empty()){
		init.level=WARN;
		init.status= EMPTY_PATH;
		m_cur_status=init;
		return false;
	}
	struct stat info;
	if(stat(path.c_str(),&info) !=0){
		init.level=WARN;
		init.status= INVALID_PATH;
		m_cur_status=init;
		return false;
	}
	else if(info.st_mode & S_IFDIR){
		init.level=OK;
		init.status= VALID;
		m_cur_status=init;
		return true;
	}
	else{
		init.level=WARN;
		init.status=NO_DIR;
		return false;
	}
	return true;
};


settingsManager::settingsManager(std::string fileName,std::string bagPath):m_bag_path(bagPath) {
	isPathValid(fileName);
	m_topics =ParseFile(fileName);

};

const std::vector<std::string>& settingsManager::GetTopics() const {return m_topics;}

Status settingsManager::GetStatus() const{return m_cur_status;}

void settingsManager::SetBagPath(const std::string path){m_bag_path = path;}

bool settingsManager::GetBagPath(std::string& s) const{if(!(m_bag_path.empty())){s=m_bag_path;} return !(m_bag_path.empty())?true:false;}

