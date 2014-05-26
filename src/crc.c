#include "crc.h"

const char CRC7_POLY = 0x91;

char getCRC(char message[], char length) {
    int i, j = 0;
    char crc = 0;
    for (i = 0; i < length; i++) {
        crc ^= message[i];
        for (j = 0; j < 8; j++) {
            if (crc & 1) crc ^= CRC7_POLY;
            crc >>= 1;
        }
    }
    return crc;
}
