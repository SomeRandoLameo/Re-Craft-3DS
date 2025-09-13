#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <stdarg.h>

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

static u32 *SOC_buffer = NULL;
int sock = -1;

__attribute__((format(printf, 1, 2)))
void failExit(const char *fmt, ...);

void socShutdown();
void initSOC();
void createSocket();
bool connectToServer(const char *ip, u16 port);
void sendMinecraftPing();
void receiveServerResponse();
void cleanupSocket();

void getServerAddress(char *out, size_t size) {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);  // type, num buttons, text length
    swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY, 0, 0); 
    swkbdSetHintText(&swkbd, "Enter Server IP");

    // limit buffer length to size-1 (null terminator)
    swkbdInputText(&swkbd, out, size);
}

int main(int argc, char **argv) {
    // Initialize graphics and console
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    atexit(gfxExit);

    // Initialize SOC and create socket
    initSOC();
    createSocket();

   // buffer for typed IP
    char ip[256] = {0};
    getServerAddress(ip, sizeof(ip));

    // fallback if user cancels
    if (ip[0] == '\0') {
        strcpy(ip, "192.168.2.101"); // default/fallback IP
    }

    printf("Connecting to %s...\n", ip);

    if (!connectToServer(ip, 25565)) {
        failExit("Failed to connect to server");
    }

    // Send Minecraft ping
    sendMinecraftPing();

    // Receive server response
    receiveServerResponse();

    cleanupSocket();

    printf("Finished.\n");
    while (aptMainLoop()) {
        gspWaitForVBlank();
    }

    return 0;
}

// Graceful SOC shutdown
void socShutdown() {
    printf("Shutting down SOC...\n");
    socExit();
}

// Initialize SOC subsystem
void initSOC() {
    SOC_buffer = (u32 *)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
    if (!SOC_buffer) failExit("Failed to allocate SOC buffer");

    int ret = socInit(SOC_buffer, SOC_BUFFERSIZE);
    if (ret != 0) failExit("socInit failed: 0x%08X", ret);

    atexit(socShutdown);
}

// Create TCP socket
void createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) failExit("socket failed: %d %s", errno, strerror(errno));

    // Set non-blocking
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
}

// Connect to server (non-blocking)
bool connectToServer(const char *ip, u16 port) {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);

    int ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0 && errno != EINPROGRESS) {
        return false;
    }

    printf("Connecting to server...\n");

    bool connected = false;
    while (aptMainLoop() && !connected) {
        gspWaitForVBlank();
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;

        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sock, &wfds);

        struct timeval tv = {0, 10000}; // 10 ms
        ret = select(sock + 1, NULL, &wfds, NULL, &tv);
        if (ret > 0 && FD_ISSET(sock, &wfds)) {
            connected = true;
        }
    }

    if (connected) printf("Connected!\n");
    return connected;
}

// Send Minecraft handshake and status request
void sendMinecraftPing() {
    uint8_t handshake = 254;
    send(sock, &handshake, sizeof(handshake), 0);

    u8 status_req[] = { 0x00 }; // Status request packet
    send(sock, status_req, sizeof(status_req), 0);
}


int isAllowedChar(unsigned char c) {
    // Simplified allowed character check (printable ASCII)
    return (c >= 32 && c <= 126);
}

// UTF-16BE → UTF-8 decoder
int utf16be_to_utf8(const uint8_t *in, int inLen, char *out, int outSize) {
    int i = 0, o = 0;
    while (i + 1 < inLen && o + 4 < outSize) {
        uint16_t code = (in[i] << 8) | in[i+1];
        i += 2;

        if (code < 0x80) {
            out[o++] = (char)code;
        } else if (code < 0x800) {
            out[o++] = 0xC0 | (code >> 6);
            out[o++] = 0x80 | (code & 0x3F);
        } else {
            out[o++] = 0xE0 | (code >> 12);
            out[o++] = 0x80 | ((code >> 6) & 0x3F);
            out[o++] = 0x80 | (code & 0x3F);
        }
    }
    out[o] = '\0';
    return o;
}

void parseServerResponse(uint8_t *buffer, int len) {
    if (len < 3) { // must have at least ID + length
        printf("Response too short\n");
        return;
    }

    // First byte is packet ID (should be 0xFF for legacy ping)
    if (buffer[0] != 0xFF) {
        printf("Unexpected packet ID: %02X\n", buffer[0]);
        return;
    }

    // Next 2 bytes: string length (big endian, count of UTF-16 characters)
    int strLen = (buffer[1] << 8) | buffer[2];
    if (strLen * 2 > len - 3) {
        printf("Invalid length field\n");
        return;
    }

    // Decode UTF-16BE string into UTF-8
    char utf8buf[1024];
    int utf8len = utf16be_to_utf8(buffer + 3, strLen * 2, utf8buf, sizeof(utf8buf));

    // Split on '§'
    char *hostParts[3] = {NULL, NULL, NULL};
    int partIndex = 0;
    char *token = strtok(utf8buf, "§");
    while (token && partIndex < 3) {
        hostParts[partIndex++] = token;
        token = strtok(NULL, "§");
    }

    const char *motd = hostParts[0] ? hostParts[0] : "";
    int onlinePlayers = -1, maxPlayers = -1;

    if (hostParts[1]) onlinePlayers = atoi(hostParts[1]);
    if (hostParts[2]) maxPlayers = atoi(hostParts[2]);

    printf("MOTD: %s\n", motd);
    if (onlinePlayers >= 0 && maxPlayers > 0) {
        printf("Players: %d/%d\n", onlinePlayers, maxPlayers);
    } else {
        printf("Players: ???\n");
    }
}



// Receive server response
void receiveServerResponse() {
    printf("Waiting for server response...\n");
    u8 buffer[1024];
    int ret;

    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;

        ret = recv(sock, buffer, sizeof(buffer)-1, 0); // leave room for null
		if (ret > 0) {
			printf("Received %d bytes from server:\n", ret);
			for (int i = 0; i < ret; i++) {
				printf("%02X ", buffer[i]);
			}
			printf("\n");

			parseServerResponse(buffer, ret);
		} else if (ret < 0 && errno != EAGAIN) {
            failExit("recv failed: %d %s", errno, strerror(errno));
        }
    }
}

// Close socket
void cleanupSocket() {
    if (sock > 0) close(sock);
    sock = -1;
}

// Fail and exit with message
void failExit(const char *fmt, ...) {
    cleanupSocket();

    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    printf("\nPress B to exit\n");
    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        if (hidKeysDown() & KEY_B) exit(0);
    }
}
