#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

#define BUF_SIZE 1024
#define BACKLOG 5

#define OTHER 0
#define AFRINIC 1
#define APNIC 2
#define ARIN 3
#define LACNIC 4
#define RIPE_NCC 5
#define HOME 6

// see https://en.wikipedia.org/wiki/Regional_Internet_registry
char *nics[] = {
    "OTHER",
    "AFRINIC",
    "APNIC",
    "ARIN",
    "LACNIC",
    "RIPE_NCC"
};

// see https://pinout.xyz > GPIO
int pins[] = {
    -1,
    24,
    5,
    25,
    27,
    23,
    22
};

// see http://www.iana.org/assignments/ipv4-address-space
int nic(int b) {
    int nic;
    switch (b) {
        case 1: nic = APNIC; break;
        case 2: nic = RIPE_NCC; break;
        case 3: nic = ARIN; break; // LEGACY
        case 4: nic = ARIN; break; // LEGACY
        case 5: nic = RIPE_NCC; break;
        case 6: nic = OTHER; break; // Army, LEGACY
        case 7: nic = ARIN; break; // LEGACY
        case 8: nic = ARIN; break; // LEGACY
        case 9: nic = ARIN; break; // LEGACY

        case 10: nic = OTHER; break; // IANA, RESERVED
        case 11: nic = OTHER; break; // DOD, LEGACY
        case 12: nic = OTHER; break; // Bell, LEGACY
        case 13: nic = ARIN; break; // LEGACY
        case 14: nic = APNIC; break;
        case 15: nic = ARIN; break; // LEGACY
        case 16: nic = ARIN; break; // LEGACY
        case 17: nic = OTHER; break; // Apple, LEGACY
        case 18: nic = ARIN; break; // LEGACY
        case 19: nic = OTHER; break; // Ford, LEGACY

        case 20: nic = ARIN; break; // LEGACY
        case 21: nic = OTHER; break; // DDN, LEGACY
        case 22: nic = ARIN; break; // DOD, LEGACY
        case 23: nic = ARIN; break;
        case 24: nic = ARIN; break;
        case 25: nic = RIPE_NCC; break; // LEGACY
        case 26: nic = OTHER; break; // US-DOD
        case 27: nic = APNIC; break;
        case 28: nic = OTHER; break; // DSI, LEGACY
        case 29: nic = OTHER; break; // DIS, LEGACY

        case 30: nic = OTHER; break; // DIS, LEGACY
        case 31: nic = RIPE_NCC; break;
        case 32: nic = ARIN; break; // LEGACY
        case 33: nic = OTHER; break; // DLA, LEGACY
        case 34: nic = ARIN; break; // LEGACY
        case 35: nic = ARIN; break; // LEGACY
        case 36: nic = APNIC; break;
        case 37: nic = RIPE_NCC; break;
        case 38: nic = OTHER; break; // PSINet, LEGACY
        case 39: nic = APNIC; break;

        case 40: nic = ARIN; break; // LEGACY
        case 41: nic = AFRINIC; break;
        case 42: nic = APNIC; break;
        case 43: nic = APNIC; break; // LEGACY
        case 44: nic = ARIN; break; // LEGACY
        case 45: nic = ARIN; break; // LEGACY
        case 46: nic = RIPE_NCC; break;
        case 47: nic = ARIN; break; // LEGACY
        case 48: nic = ARIN; break; // LEGACY
        case 49: nic = APNIC; break;

        case 50: nic = ARIN; break;
        case 51: nic = RIPE_NCC; break; // LEGACY
        case 52: nic = ARIN; break; // LEGACY
        case 53: nic = OTHER; break; // Daimler, LEGACY
        case 54: nic = ARIN; break; // LEGACY
        case 55: nic = OTHER; break; // US-DOD, LEGACY
        case 56: nic = ARIN; break; // LEGACY
        case 57: nic = RIPE_NCC; break; // LEGACY
        case 58: nic = APNIC; break;
        case 59: nic = APNIC; break;

        case 60: nic = APNIC; break;
        case 61: nic = APNIC; break;
        case 62: nic = RIPE_NCC; break;
        case 63: nic = ARIN; break;
        case 64: nic = ARIN; break;
        case 65: nic = ARIN; break;
        case 66: nic = ARIN; break;
        case 67: nic = ARIN; break;
        case 68: nic = ARIN; break;
        case 69: nic = ARIN; break;

        case 70: nic = ARIN; break;
        case 71: nic = ARIN; break;
        case 72: nic = ARIN; break;
        case 73: nic = ARIN; break;
        case 74: nic = ARIN; break;
        case 75: nic = ARIN; break;
        case 76: nic = ARIN; break;
        case 77: nic = RIPE_NCC; break;
        case 78: nic = RIPE_NCC; break;
        case 79: nic = RIPE_NCC; break;

        case 80: nic = RIPE_NCC; break;
        case 81: nic = RIPE_NCC; break;
        case 82: nic = RIPE_NCC; break;
        case 83: nic = RIPE_NCC; break;
        case 84: nic = RIPE_NCC; break;
        case 85: nic = RIPE_NCC; break;
        case 86: nic = RIPE_NCC; break;
        case 87: nic = RIPE_NCC; break;
        case 88: nic = RIPE_NCC; break;
        case 89: nic = RIPE_NCC; break;

        case 90: nic = RIPE_NCC; break;
        case 91: nic = RIPE_NCC; break;
        case 92: nic = RIPE_NCC; break;
        case 93: nic = RIPE_NCC; break;
        case 94: nic = RIPE_NCC; break;
        case 95: nic = RIPE_NCC; break;
        case 96: nic = ARIN; break;
        case 97: nic = ARIN; break;
        case 98: nic = ARIN; break;
        case 99: nic = ARIN; break;

        case 100: nic = ARIN; break;
        case 101: nic = APNIC; break;
        case 102: nic = AFRINIC; break;
        case 103: nic = APNIC; break;
        case 104: nic = ARIN; break;
        case 105: nic = AFRINIC; break;
        case 106: nic = APNIC; break;
        case 107: nic = ARIN; break;
        case 108: nic = ARIN; break;
        case 109: nic = RIPE_NCC; break;

        case 110: nic = APNIC; break;
        case 111: nic = APNIC; break;
        case 112: nic = APNIC; break;
        case 113: nic = APNIC; break;
        case 114: nic = APNIC; break;
        case 115: nic = APNIC; break;
        case 116: nic = APNIC; break;
        case 117: nic = APNIC; break;
        case 118: nic = APNIC; break;
        case 119: nic = APNIC; break;

        case 120: nic = APNIC; break;
        case 121: nic = APNIC; break;
        case 122: nic = APNIC; break;
        case 123: nic = APNIC; break;
        case 124: nic = APNIC; break;
        case 125: nic = APNIC; break;
        case 126: nic = APNIC; break;
        case 127: nic = OTHER; break; // IANA
        case 128: nic = ARIN; break; // LEGACY
        case 129: nic = ARIN; break; // LEGACY

        case 130: nic = ARIN; break; // LEGACY
        case 131: nic = ARIN; break; // LEGACY
        case 132: nic = ARIN; break; // LEGACY
        case 133: nic = APNIC; break; // LEGACY
        case 134: nic = ARIN; break; // LEGACY
        case 135: nic = ARIN; break; // LEGACY
        case 136: nic = ARIN; break; // LEGACY
        case 137: nic = ARIN; break; // LEGACY
        case 138: nic = ARIN; break; // LEGACY
        case 139: nic = ARIN; break; // LEGACY

        case 140: nic = ARIN; break; // LEGACY
        case 141: nic = RIPE_NCC; break; // LEGACY
        case 142: nic = ARIN; break; // LEGACY
        case 143: nic = ARIN; break; // LEGACY
        case 144: nic = ARIN; break; // LEGACY
        case 145: nic = RIPE_NCC; break; // LEGACY
        case 146: nic = ARIN; break; // LEGACY
        case 147: nic = ARIN; break; // LEGACY
        case 148: nic = ARIN; break; // LEGACY
        case 149: nic = ARIN; break; // LEGACY

        case 150: nic = APNIC; break; // LEGACY
        case 151: nic = RIPE_NCC; break; // LEGACY
        case 152: nic = ARIN; break; // LEGACY
        case 153: nic = APNIC; break; // LEGACY
        case 154: nic = AFRINIC; break; // LEGACY
        case 155: nic = ARIN; break; // LEGACY
        case 156: nic = ARIN; break; // LEGACY
        case 157: nic = ARIN; break; // LEGACY
        case 158: nic = ARIN; break; // LEGACY
        case 159: nic = ARIN; break; // LEGACY

        case 160: nic = ARIN; break; // LEGACY
        case 161: nic = ARIN; break; // LEGACY
        case 162: nic = ARIN; break; // LEGACY
        case 163: nic = APNIC; break; // LEGACY
        case 164: nic = ARIN; break; // LEGACY
        case 165: nic = ARIN; break; // LEGACY
        case 166: nic = ARIN; break; // LEGACY
        case 167: nic = ARIN; break; // LEGACY
        case 168: nic = ARIN; break; // LEGACY
        case 169: nic = ARIN; break; // LEGACY

        case 170: nic = ARIN; break; // LEGACY
        case 171: nic = APNIC; break; // LEGACY
        case 172: nic = ARIN; break; // LEGACY
        case 173: nic = ARIN; break;
        case 174: nic = ARIN; break;
        case 175: nic = APNIC; break;
        case 176: nic = RIPE_NCC; break;
        case 177: nic = LACNIC; break;
        case 178: nic = RIPE_NCC; break;
        case 179: nic = LACNIC; break;

        case 180: nic = APNIC; break;
        case 181: nic = LACNIC; break;
        case 182: nic = APNIC; break;
        case 183: nic = APNIC; break;
        case 184: nic = ARIN; break;
        case 185: nic = RIPE_NCC; break;
        case 186: nic = LACNIC; break;
        case 187: nic = LACNIC; break;
        case 188: nic = RIPE_NCC; break; // LEGACY
        case 189: nic = LACNIC; break;

        case 190: nic = LACNIC; break;
        case 191: nic = LACNIC; break; // LEGACY
        case 192: nic = ARIN; break; // LEGACY
        case 193: nic = RIPE_NCC; break;
        case 194: nic = RIPE_NCC; break;
        case 195: nic = RIPE_NCC; break;
        case 196: nic = AFRINIC; break; // LEGACY
        case 197: nic = AFRINIC; break;
        case 198: nic = ARIN; break; // LEGACY
        case 199: nic = ARIN; break;

        case 200: nic = LACNIC; break;
        case 201: nic = LACNIC; break;
        case 202: nic = APNIC; break;
        case 203: nic = APNIC; break;
        case 204: nic = ARIN; break;
        case 205: nic = ARIN; break;
        case 206: nic = ARIN; break;
        case 207: nic = ARIN; break;
        case 208: nic = ARIN; break;
        case 209: nic = ARIN; break;

        case 210: nic = APNIC; break;
        case 211: nic = APNIC; break;
        case 212: nic = RIPE_NCC; break;
        case 213: nic = RIPE_NCC; break;
        case 214: nic = OTHER; break; // US-DOD, LEGACY
        case 215: nic = OTHER; break; // US-DOD, LEGACY
        case 216: nic = ARIN; break;
        case 217: nic = RIPE_NCC; break;
        case 218: nic = APNIC; break;
        case 219: nic = APNIC; break;

        case 220: nic = APNIC; break;
        case 221: nic = APNIC; break;
        case 222: nic = APNIC; break;
        case 223: nic = APNIC; break;
        case 224: nic = OTHER; break; // RESERVED
        case 225: nic = OTHER; break; // RESERVED
        case 226: nic = OTHER; break; // RESERVED
        case 227: nic = OTHER; break; // RESERVED
        case 228: nic = OTHER; break; // RESERVED
        case 229: nic = OTHER; break; // RESERVED

        case 230: nic = OTHER; break; // RESERVED
        case 231: nic = OTHER; break; // RESERVED
        case 232: nic = OTHER; break; // RESERVED
        case 233: nic = OTHER; break; // RESERVED
        case 234: nic = OTHER; break; // RESERVED
        case 235: nic = OTHER; break; // RESERVED
        case 236: nic = OTHER; break; // RESERVED
        case 237: nic = OTHER; break; // RESERVED
        case 238: nic = OTHER; break; // RESERVED
        case 239: nic = OTHER; break; // RESERVED

        case 240: nic = OTHER; break; // RESERVED
        case 241: nic = OTHER; break; // RESERVED
        case 242: nic = OTHER; break; // RESERVED
        case 243: nic = OTHER; break; // RESERVED
        case 244: nic = OTHER; break; // RESERVED
        case 245: nic = OTHER; break; // RESERVED
        case 246: nic = OTHER; break; // RESERVED
        case 247: nic = OTHER; break; // RESERVED
        case 248: nic = OTHER; break; // RESERVED
        case 249: nic = OTHER; break; // RESERVED

        case 250: nic = OTHER; break; // RESERVED
        case 251: nic = OTHER; break; // RESERVED
        case 252: nic = OTHER; break; // RESERVED
        case 253: nic = OTHER; break; // RESERVED
        case 254: nic = OTHER; break; // RESERVED
        case 255: nic = OTHER; break; // RESERVED

        default: nic = OTHER;
    }
    return nic;
}

void setup_pins() {
    char all_pins[32];
    sprintf(all_pins, "%d,%d,%d,%d,%d,%d",
        pins[AFRINIC],
        pins[APNIC],
        pins[ARIN],
        pins[LACNIC],
        pins[RIPE_NCC],
        pins[HOME]);
    char cmd[64];
    sprintf(cmd, "pinctrl set %s op", all_pins); // output
    system(cmd);
    sprintf(cmd, "pinctrl set %s dl", all_pins); // low
    system(cmd);
}

void blink(int pin) {
    char cmd[64];
    sprintf(cmd, "pinctrl set %d dh", pin); // high
    system(cmd);
    int dt = 10; // ms
    struct timespec ts = { 0, (dt % 1000) * 1000000 };
    nanosleep(&ts, NULL);
    sprintf(cmd, "pinctrl set %d dl", pin); // low
    system(cmd);
}

int main(void) {
    printf("main\n");
    setup_pins();

    struct in_addr ip_addr;
    inet_pton(AF_INET, "0.0.0.0", &ip_addr.s_addr);

    struct sockaddr_in sv_addr;
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(8080); // 80, 22
    sv_addr.sin_addr = ip_addr;

    int sv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sv_fd == -1) { perror("socket"); exit(-1); }
    int res = bind(sv_fd, (struct sockaddr *) &sv_addr, sizeof(struct sockaddr_in));
    if (res == -1) { perror("bind"); exit(-1); }
    listen(sv_fd, 5);

    socklen_t cl_addr_len;
    struct sockaddr_in cl_addr;
    while (1) {
        int cl_fd = accept(sv_fd, (struct sockaddr *) &cl_addr, &cl_addr_len);
        if (cl_fd == -1) { perror("accept"); exit(-1); }
        printf("client addr AF = %d\n", cl_addr.sin_family);
        if (cl_addr.sin_family == AF_INET) {
            int b = cl_addr.sin_addr.s_addr % 256; // e.g. *127*.0.0.1
            int n = nic(b);
            int p = pins[n];
            printf("client addr %d, %d, %d\n", cl_addr.sin_addr.s_addr, b, n, p);
            if (p != OTHER) {
                blink(p);
            }
            //blink(pins[AFRINIC]);
            //blink(pins[APNIC]);
            //blink(pins[ARIN]);
            //blink(pins[LACNIC]);
            //blink(pins[RIPE_NCC]);
            blink(pins[HOME]);
        }
        close(cl_fd);
    }
}
