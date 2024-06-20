#include <stdio.h>
#include "lidar.h"

void ultrasound_init() {
    printf("LiDAR: Initializing ultrasound...\n");
}

void ultrasound_send_pulse() {
    printf("LiDAR: Sending pulse...\n");
}

float ultrasound_receive_echo() {
    printf("LiDAR: Receiving echo...\n");
    return 0.0;
}
