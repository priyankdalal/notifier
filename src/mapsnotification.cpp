#include<mapsnotification.h>
void MapsNotification::setEta(char* s){
    eta=s;
};
char* MapsNotification::getEta(){
    return eta;
};

void MapsNotification::setTimeToEta(char* s){
    TimeToEta=s;
};
char* MapsNotification::getTimeToEta(){
    return TimeToEta;
};

void MapsNotification::setHeading(char* s){
    heading=s;
};
char* MapsNotification::getHeading(){
    return heading;
};

void MapsNotification::setTitle(char* s){
    title=s;
};
char* MapsNotification::getTitle(){
    return title;
};

void MapsNotification::setDistanceRemaining(char* s){
    distanceRemaining=s;
};
char* MapsNotification::getDistanceRemaining(){
    return distanceRemaining;
};

void MapsNotification::setDestination(char* s){
    destination=s;
};
char* MapsNotification::getDestination(){
    return destination;
};