#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "rec_status.h"


 

class settingsManager{
private:
	std::vector<std::string> m_topics;	
	Status m_cur_status;
	std::string m_bag_path;

protected:
	std::vector<std::string> ParseFile(std::string fileName);		//YAML::NODE toParse, parse should fill the status field.
	bool isPathValid(std::string path) ;
public:
    settingsManager() = delete;
	settingsManager(std::string fileName,std::string bagPath = "");
    ~settingsManager() =default;
	const std::vector<std::string>& GetTopics() const;      
	void SetBagPath(const std::string path);
	Status GetStatus() const;
	bool GetBagPath(std::string& ) const;  //call only after check if path is set or change return type to pointer
};