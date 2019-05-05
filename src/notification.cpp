#include<notification.h>

// void Notification::setDisplay(Display d){
//     disp=d;
// }
void Notification::setCode(int c){
    code=c;
};

int Notification::getCode(){
    return code;
};

void Notification::setTitle(char* t){
    title=t;
};
char* Notification::getTitle(){
    return title;
};

void Notification::setConversationTitle(char* t){
    conversationTitle=t;
};
char* Notification::getConversationTitle(){
    return conversationTitle;
};

void Notification::setSubText(char* t){
    subText=t;
};
char* Notification::getSubText(){
    return subText;
};

void Notification::setText(char* t){
    text=t;
};
char* Notification::getText(){
    return text;
};

void Notification::setSelfDisplayName(char* t){
    selfDisplayName=t;
};
char* Notification::getSelfDisplayName(){
    return selfDisplayName;
};

void Notification::setInfoText(char* t){
    infoText=t;
};
char* Notification::getInfoText(){
    return infoText;
};

void Notification::processNotification(){
    if(getCode()==5){
        MapsNotification mapNotification;
        if(strlen(subText)>0){
            char *found;
            char* eta;
            char* distance;
            char* eta2;
            int count=0;
            while((found=strsep(&text," "))!=NULL){
                char c1[50],c2[20];
                if(count==0){
                    strcpy (c1, found);
                }else if(count==1){
                    strcpy(c2,found);
                    strcat(c1, " ");
                    strcat(c1, c2);
                    eta=&c1[0];
                }else if(count==3){
                    distance=found;
                }else if(count==4){
                    strcpy (c1, found);
                }else if(count==5){
                    strcpy(c2,found);
                    strcat(c1, " ");
                    strcat(c1, c2);
                    eta2=&c1[0];
                }
                count++;
            }
            mapNotification.setEta(eta);
            mapNotification.setDistanceRemaining(distance);
            mapNotification.setTimeToEta(eta2);
        }
    }
};


