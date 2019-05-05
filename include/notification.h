#include<string.h>
#include <display.h>
#include<mapsnotification.h>
class Notification{
    public:
        int code;
        char* title;
        char* conversationTitle;
        char* subText;
        char* text;
        char* selfDisplayName;
        char* infoText;
        //MapsNotification* map;
        Display* disp;

        Notification(Display* );

        void setDisplay(Display *);

        void setCode(int);
        int getCode();

        void setTitle(char*);
        char* getTitle();

        void setConversationTitle(char*);
        char* getConversationTitle();

        void setSubText(char*);
        char* getSubText();

        void setText(char*);
        char* getText();

        void setSelfDisplayName(char*);
        char* getSelfDisplayName();

        void setInfoText(char*);
        char* getInfoText();

        void processNotification();
};
