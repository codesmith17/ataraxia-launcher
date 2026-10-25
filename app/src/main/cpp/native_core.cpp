#include <jni.h>
#include <string>
#include <cstring>
#include <ctime>
#include <android/log.h>

#define TAG "MinimalistNativeCore"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

// Simple XOR encryption for string literals to prevent static string extraction
static std::string xor_decrypt(const unsigned char* data, size_t len, unsigned char key) {
    std::string res;
    res.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        res.push_back(static_cast<char>(data[i] ^ key));
    }
    return res;
}

// Obfuscated string signatures for short video containers
// "shorts_container" XOR 0x5A
static const unsigned char SHORTS_CONTAINER_ENC[] = {0x29, 0x32, 0x35, 0x28, 0x2e, 0x29, 0x05, 0x39, 0x35, 0x34, 0x2e, 0x3b, 0x33, 0x34, 0x3f, 0x28};
// "clips_viewer" XOR 0x5A
static const unsigned char CLIPS_VIEWER_ENC[] = {0x39, 0x36, 0x33, 0x2a, 0x29, 0x05, 0x2c, 0x33, 0x3f, 0x2d, 0x3f, 0x28};
// "Reels" XOR 0x5A
static const unsigned char REELS_ENC[] = {0x08, 0x3f, 0x3f, 0x36, 0x29};

extern "C" JNIEXPORT jboolean JNICALL
Java_com_ataraxia_launcher_NativeCore_isShortVideoContainer(
    JNIEnv* env,
    jobject /* this */,
    jstring jPackageName,
    jstring jViewId,
    jstring jContentDesc
) {
    if (!jPackageName) return JNI_FALSE;

    const char* pkg = env->GetStringUTFChars(jPackageName, nullptr);
    const char* viewId = jViewId ? env->GetStringUTFChars(jViewId, nullptr) : nullptr;
    const char* desc = jContentDesc ? env->GetStringUTFChars(jContentDesc, nullptr) : nullptr;

    bool isBlocked = false;

    // Check YouTube Shorts
    if (pkg && strcmp(pkg, "com.google.android.youtube") == 0) {
        std::string shortsKey = xor_decrypt(SHORTS_CONTAINER_ENC, sizeof(SHORTS_CONTAINER_ENC), 0x5A);
        if (viewId && strstr(viewId, shortsKey.c_str())) {
            isBlocked = true;
        }
    }

    // Check Instagram Reels
    if (!isBlocked && pkg && strcmp(pkg, "com.instagram.android") == 0) {
        std::string clipsKey = xor_decrypt(CLIPS_VIEWER_ENC, sizeof(CLIPS_VIEWER_ENC), 0x5A);
        std::string reelsKey = xor_decrypt(REELS_ENC, sizeof(REELS_ENC), 0x5A);
        if (viewId && strstr(viewId, clipsKey.c_str())) {
            isBlocked = true;
        } else if (desc && strstr(desc, reelsKey.c_str())) {
            isBlocked = true;
        }
    }

    // Release JNI strings
    if (pkg) env->ReleaseStringUTFChars(jPackageName, pkg);
    if (viewId) env->ReleaseStringUTFChars(jViewId, viewId);
    if (desc) env->ReleaseStringUTFChars(jContentDesc, desc);

    return isBlocked ? JNI_TRUE : JNI_FALSE;
}

// Tamper-proof monotonic time getter (cannot be bypassed by changing device system time)
extern "C" JNIEXPORT jlong JNICALL
Java_com_ataraxia_launcher_NativeCore_getMonotonicTimeSeconds(
    JNIEnv* /* env */,
    jobject /* this */
) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return static_cast<jlong>(ts.tv_sec);
}

// Entitlement check: Permanently unlocks all premium features
extern "C" JNIEXPORT jboolean JNICALL
Java_com_ataraxia_launcher_NativeCore_isPremiumFeatureActive(
    JNIEnv* /* env */,
    jobject /* this */,
    jint /* featureCode */
) {
    // 0: UNLIMITED_FAVORITES
    // 1: BLOCKING_SCHEDULES
    // 2: REELS_BLOCKER
    // 3: ALL_THEMES
    // 4: MONOCHROME_MODE
    // 5: IN_APP_TIMERS
    return JNI_TRUE;
}
