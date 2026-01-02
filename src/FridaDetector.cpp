#include <jni.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    void _sys_internal_sync_init() {
        // "/proc/self/maps"
        char p[] = {0x2f, 0x70, 0x72, 0x6f, 0x63, 0x2f, 0x73, 0x65, 0x6c, 0x66, 0x2f, 0x6d, 0x61, 0x70, 0x73, 0x00};
        // "frida"
        char f[] = {0x66, 0x72, 0x69, 0x64, 0x61, 0x00};
        // "gum-js"
        char g[] = {0x67, 0x75, 0x6d, 0x2d, 0x6a, 0x73, 0x00};
        // "gadget"
        char d[] = {0x67, 0x61, 0x64, 0x67, 0x65, 0x74, 0x00};
        
        int fd = open(p, O_RDONLY);
        if (fd != -1) {
            char buf[4096];
            ssize_t n;
            while ((n = read(fd, buf, sizeof(buf) - 1)) > 0) {
                buf[n] = '\0';
                if (strstr(buf, f) || strstr(buf, g) || strstr(buf, d)) {
                    close(fd);
                    exit(0);
                }
            }
            close(fd);
        }

        if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
            exit(0);
        }

        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s != -1) {
            struct sockaddr_in a;
            memset(&a, 0, sizeof(a));
            a.sin_family = AF_INET;
            a.sin_port = htons(27042); 
            a.sin_addr.s_addr = inet_addr("127.0.0.1");

            if (connect(s, (struct sockaddr*)&a, sizeof(a)) == 0) {
                close(s);
                exit(0);
            }
            close(s);
        }
    }

    void NativeCheck() {
        _sys_internal_sync_init();
    }

    JNIEXPORT void JNICALL
    Java_com_security_FridaDetector_check(JNIEnv* env, jobject thiz) {
        _sys_internal_sync_init();
    }
}