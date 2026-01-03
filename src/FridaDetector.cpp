#include <jni.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>
#include <thread>

#define LOG_TAG "WebUtils"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

void Monitor() {
    char p[] = {0x2f, 0x70, 0x72, 0x6f, 0x63, 0x2f, 0x73, 0x65, 0x6c, 0x66, 0x2f, 0x6d, 0x61, 0x70, 0x73, 0x00};
    
    struct sockaddr_in a;
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET;
    a.sin_port = htons(27042);
    a.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true) {
        FILE* fp = fopen(p, "r");
        if (fp) {
            char line[512];
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, "frida") || strstr(line, "gum-js") || strstr(line, "gadget")) {
                    // fake err if frida or related stuff is in memory
                    LOGD("SIGSEGV: invalid address alignment at %p", (void*)line);
                    fclose(fp);
                    exit(0);
                }
            }
            fclose(fp);
        }

        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s != -1) {
            int flags = fcntl(s, F_GETFL, 0);
            fcntl(s, F_SETFL, flags | O_NONBLOCK);
            
            if (connect(s, (struct sockaddr*)&a, sizeof(a)) == 0) {
                // fake error if frida port is open
                LOGD("Fatal Exception: Address already in use (Errno 98) in background_thread_pool");
                close(s);
                exit(0);
            }
            close(s);
        }

        sleep(5); 
    }
}

extern "C" {
    void NativeCheck() {
        std::thread(Monitor).detach();
        LOGD("Contacting PlayFab API");
    }

    JNIEXPORT void JNICALL
    Java_com_security_FridaDetector_check(JNIEnv* env, jobject thiz) {
        std::thread(Monitor).detach();
        LOGD("Contacting PlayFab API");
    }
}