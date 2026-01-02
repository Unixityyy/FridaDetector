#include <jni.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern "C" {
    JNIEXPORT jboolean JNICALL
    Java_com_security_FridaDetector_check(JNIEnv* env, jobject thiz) {
        char line[512];
        
        int fd = open("/proc/self/maps", O_RDONLY);
        if (fd != -1) {
            while (read(fd, line, sizeof(line)) > 0) {
                if (strstr(line, "frida") || strstr(line, "gadget") || 
                    strstr(line, "gum-js") || strstr(line, "patcher")) {
                    close(fd);
                    return JNI_TRUE;
                }
            }
            close(fd);
        }

        if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
            return JNI_TRUE;
        }

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(27042); 
        inet_aton("127.0.0.1", &sa.sin_addr);

        if (connect(sock, (struct sockaddr*)&sa, sizeof(sa)) == 0) {
            close(sock);
            return JNI_TRUE;
        }
        close(sock);

        return JNI_FALSE;
    }

    bool IsFridaDetected() {
        return Java_com_security_FridaDetector_check(nullptr, nullptr);
    }
}