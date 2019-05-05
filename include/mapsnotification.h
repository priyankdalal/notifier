class MapsNotification{
    public:
        char* eta;
        char* TimeToEta;
        char* heading;
        char* title;
        char* distanceRemaining;
        char* destination;

        void setEta(char*);
        char* getEta();

        void setTimeToEta(char*);
        char* getTimeToEta();

        void setHeading(char*);
        char* getHeading();

        void setTitle(char*);
        char* getTitle();

        void setDistanceRemaining(char*);
        char* getDistanceRemaining();

        void setDestination(char*);
        char* getDestination();
};