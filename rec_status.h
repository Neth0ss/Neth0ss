#ifndef STATUS_H
#define STATUS_H

enum StatusType{
    VALID=0,INVALID_PATH, NO_TOPICS,BAG_AT_READ_MODE,EMPTY_PATH, NO_DIR
    };

enum Level{
    OK=0,WARN,ERROR
};
struct Status{
	int level;
	StatusType status;
};

inline std::string ConvertLevel(Level level){
    if(level==0){
        return "OK";
    }
    else if(level==1){
        return "WARN";
    }
    else{
        return "ERROR";
    }
}

inline std::string ConvertStatus(StatusType status){
    if(status==0){
        return "VALID";
    }
    else if(status==1){
        return "INVALID_PATH";
    }
    else if(status==2){
        return "NO_TOPICS";
    }
    else if(status==3){
        return "BAG_AT_READ_MODE";
    }
    else if(status==4){
        return "EMPTY_PATH";
    }
    else if(status==5){
        return "NO_DIR";
    }
    else{
        return "UNKNOWN_ERROR";
    }
}
inline void printStatus(int level, int status){
    std::cout<< ConvertLevel(Level(level)) << "::" << ConvertStatus(StatusType(status)) << std::endl;
}


#endif