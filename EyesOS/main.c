#include <stdio.h>

void ADA_turnOn() {
    printf("ADA: turn on\n");
}

void ADA_useCameraAndLIDAR() {
    printf("ADA: use alpha camera & beta camera, LIDAR\n");
}

void ADA_objectDetection() {
    printf("ADA: use OEClassifier to classify objects\n");
}

void ADA_objectSpotted(char* position, int distance) {
    printf("ADA: Spotted to your %s, distance: %d. Still left? Step.\n", position, distance);
}

int main() {
    ADA_turnOn();
    ADA_useCameraAndLIDAR();
    ADA_objectDetection();
    
    char* objectPosition = "left";
    int objectDistance = 10;
    
    ADA_objectSpotted(objectPosition, objectDistance);
    
    return 0;
}