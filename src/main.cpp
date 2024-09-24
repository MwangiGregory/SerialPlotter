#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "serialib.h"

int main(int argc, char **argv)
{
    char buffer[1024] = {0};
    const unsigned int baud_rate = 500000;
    const char *serial_port_name = "/dev/ttyUSB0";

    serialib serial;

    if (serial.openDevice(serial_port_name, baud_rate) != 1)
    {
        printf("Failed to connect to %s\n", serial_port_name);
        return EXIT_FAILURE;
    }
    printf("Successful connection to %s\n", serial_port_name);

    while (true)
    {
        if (serial.available() > 0)
        {
            serial.readString(buffer, '\n', 1024);
            printf("%s\n", buffer);
        }
    }

    serial.closeDevice();
    return EXIT_SUCCESS;
}
