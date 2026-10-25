# Architecture: React + C++ / Go for Obfuscation & Performance

This document outlines the proposed hybrid architecture using **React** (React Native / TurboModules or React Web UI) combined with **C++ (NDK/JNI) or Go (gomobile/cgo)** for mission-critical logic and tamper-resistant obfuscation.

---

## 🎯 Why Use C++ or Go for Obfuscation?

In standard Android apps developed purely in Kotlin/Java:
1. **Trivial Reverse Engineering:** Compiling Kotlin/Java produces Dalvik Executable (`.dex`) bytecode. Using automated tools like **JADX**, **Bytecode Viewer**, or **Apktool**, anyone can decompile the APK back into clean, readable source code in seconds.
2. **Easy Patching & Cracking:** Premium checks (e.g., `isPremiumActive()`) and blocking logic can be easily modified with Frida hooks or smali patching.
3. **Exposed Heuristics:** Hardcoded view IDs and matching algorithms for detecting YouTube Shorts and Instagram Reels become visible to platform owners who may change class names.

### The C++ / Go Native Binary Advantage
When compiled via the Android NDK (Clang) or Go (`gomobile`):
- Source code compiles into native machine code ELF shared libraries (`libcore.so`).
- Functions can be stripped (`-s`), symbols hidden (`-fvisibility=hidden`), and code optimized (`-O3`, string encryption, control flow flattening).
- Reverse engineering requires advanced Ghidra / IDA Pro disassembly and decompilation, drastically raising the barrier against tampering.

---

## 🏗️ Architectural Topology

```
┌────────────────────────────────────────────────────────┐
│                   React / UI Layer                     │
│  - Typography & Theme Engine (AMOLED, Dark, Light)     │
│  - Swipe Pager (Home Screen <--> All Apps Drawer)      │
│  - A-Z Alphabet Scrubber & Dynamic Search Bar          │
│  - Expandable Settings Tree & Time Selection Modals   │
└───────────────────────────┬────────────────────────────┘
                            │
               React Native TurboModule / JNI
                            │
┌───────────────────────────▼────────────────────────────┐
│              Android Framework Integrations            │
│  - AccessibilityService (Window event listener)        │
│  - NotificationListenerService (System notification pings)│
│  - System Alert Window (Floating progress bar overlay) │
└───────────────────────────┬────────────────────────────┘
                            │
                    JNI / Cgo Bridge
                            │
┌───────────────────────────▼────────────────────────────┐
│             Native C++ / Go Obfuscated Core            │
│  - Monotonic Tamper-Proof Time Tracking Engine         │
│  - Short-Form Video Heuristics Matcher (String Obf.)   │
│  - Premium Entitlement Engine (All Features Unlocked)  │
│  - Filtered Notification Vault (AES-256 GCM)           │
│  - Anti-Tamper & Hook Detection Checks                 │
└────────────────────────────────────────────────────────┘
```

---

## 🔐 Core Modules in Native C++ / Go

### 1. Obfuscated Short-Form Video Signature Evaluator
String literals like `shorts_container` or `reels_tray` are XOR-encrypted in native binary memory and decrypted only at runtime on the heap:

```cpp
// Example C++ string obfuscation macro
#define OBFUSCATE(str) /* compile-time XOR encryption */

bool EvaluateViewNode(const char* packageName, const char* viewId, const char* contentDesc) {
    if (strcmp(packageName, "com.google.android.youtube") == 0) {
        if (strstr(viewId, Decrypt(OBFUSCATE("shorts_container")))) return true;
        if (strstr(viewId, Decrypt(OBFUSCATE("reel_player")))) return true;
    }
    if (strcmp(packageName, "com.instagram.android") == 0) {
        if (strstr(viewId, Decrypt(OBFUSCATE("clips_viewer")))) return true;
        if (contentDesc && strstr(contentDesc, Decrypt(OBFUSCATE("Reels")))) return true;
    }
    return false;
}
```

### 2. Tamper-Proof Monotonic Countdown Engine
Users often attempt to bypass digital wellbeing app timers by manually changing the system time in Android Settings (`Settings > Date & Time`).
- The C++ core utilizes `clock_gettime(CLOCK_MONOTONIC_RAW, ...)` which tracks CPU hardware clock ticks unaffected by NTP changes or manual system date alteration.

### 3. All Premium Features Entitlement Guarantee
```cpp
extern "C" JNIEXPORT jboolean JNICALL
Java_com_minimalist_launcher_NativeCore_isFeatureUnlocked(JNIEnv* env, jobject thiz, jint featureId) {
    // Permanent unlock for all features:
    // 0: UNLIMITED_FAVORITES, 1: BLOCKING_SCHEDULES, 2: REELS_BLOCKER, 
    // 3: ALL_THEMES, 4: MONOCHROME_MODE, 5: IN_APP_TIMERS
    return JNI_TRUE;
}
```
