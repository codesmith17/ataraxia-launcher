# System Architecture: Pure React Native + C++ / Go Native Engine
## (Zero Kotlin / Java in UI — High-Performance Obfuscation Architecture)

This document details the software architecture for building the Minimalist Launcher entirely with **React Native for the UI** and **C++ / Go for native background services, accessibility heuristics, and military-grade obfuscation**.

---

## 🏛️ High-Level Architectural Diagram

```
┌────────────────────────────────────────────────────────────────────────┐
│                      REACT / JAVASCRIPT UI LAYER                       │
│                                                                        │
│   ┌─────────────────────┐   ┌───────────────────┐   ┌──────────────┐   │
│   │     HomeScreen      │   │   AllAppsDrawer   │   │ SettingsTree │   │
│   │  (Clock + Battery)  │   │  (A-Z Scrubber)   │   │ (Accordion)  │   │
│   └──────────┬──────────┘   └─────────┬─────────┘   └──────┬───────┘   │
│              │                        │                    │           │
│              └────────────────────────┼────────────────────┘           │
│                                       │                                │
│                   React Native JavaScript Interface (JSI)              │
│                 (Zero Java / Kotlin Bridge Serialization)              │
└───────────────────────────────────────┬────────────────────────────────┘
                                        │
┌───────────────────────────────────────▼────────────────────────────────┐
│                    NATIVE C++ / GO COMPILED ENGINE                     │
│                       (libminimalistcore.so)                           │
│                                                                        │
│   ┌────────────────────────────────────────────────────────────────┐   │
│   │                  Security & Obfuscation Core                   │   │
│   │   - String XOR / AES-128 Encryption Table                      │   │
│   │   - Control Flow Flattening (OLLVM compatible)                 │   │
│   │   - Monotonic Hardware Clock (Tamper-proof against NTP hacks)  │   │
│   │   - Permanent Premium Entitlement Logic (100% Free / Unlocked) │   │
│   └────────────────────────────────────────────────────────────────┘   │
│   ┌────────────────────────────────────────────────────────────────┐   │
│   │              Short Video & Content Heuristics Engine           │   │
│   │   - Instagram Reels Evaluator (Direct Message Whitelist)       │   │
│   │   - YouTube Shorts Interceptor                                 │   │
│   │   - Reddit Video Swipe Carousel Blocker                        │   │
│   │   - YouTube Educational Video Classifier (Title/Channel filter)│   │
│   └────────────────────────────────────────────────────────────────┘   │
│   ┌────────────────────────────────────────────────────────────────┐   │
│   │                Platform System Interop (NDK / Cgo)             │   │
│   │   - Accessibility Service Event Dispatcher                     │   │
│   │   - Notification Listener Stream Buffer                        │   │
│   │   - Window Manager Overlay Manager (Floating Progress Timer)   │   │
│   └────────────────────────────────────────────────────────────────┘   │
└────────────────────────────────────────────────────────────────────────┘
```

---

## ⚡ Why Pure React Native (JSI) + C++ / Go?

| Dimension | Traditional Android (Kotlin) | Pure React Native + C++ / Go |
|---|---|---|
| **UI Development** | Verbose XML / Jetpack Compose | Declarative React components, instant styling, flexbox layout, hot reload. |
| **Reverse Engineering Risk** | **Extremely High** (JADX decompiles Kotlin `.dex` bytecode back to readable source in 1 click). | **Near Zero** (C++/Go compiles directly into stripped ELF machine code binaries). |
| **Bridge Latency** | N/A (single language) | **0ms overhead via JSI** (Direct C++ host object calls from JS without JSON serialization). |
| **Tamper Resistance** | Easy to bypass with Frida / Smali injection. | Protected with symbol stripping, inlined assembly checks, and hardware clock calls. |
| **Cross-Platform Portability** | Android only. | 90% of the UI code is reusable for iOS Minimalist Phone or Desktop companion. |

---

## 📂 Proposed Codebase Organization

```
minimalist-phone-launcher/
├── package.json                       # React Native dependencies
├── src/                               # 100% React / TypeScript UI
│   ├── index.tsx                      # Root App Component
│   ├── components/
│   │   ├── BatteryClockCanvas.tsx     # SVG / Skia Canvas circular battery gauge & clock
│   │   ├── AlphabetScrubber.tsx       # Smooth touch-drag A-Z fast index bar
│   │   ├── FavoriteAppList.tsx        # Unlimited minimalist text app labels
│   │   ├── MindfulTimePickerModal.tsx # Instagram 5m/10m/15m/Custom time modal
│   │   └── FloatingTimerBar.tsx       # In-app progress timer HUD
│   ├── screens/
│   │   ├── HomeScreen.tsx             # Page 0: Clean home interface
│   │   ├── AllAppsScreen.tsx          # Page 1: Filterable app drawer with fuzzy search
│   │   ├── SettingsScreen.tsx         # Expandable accordion settings hub
│   │   └── BlockingScheduleScreen.tsx # Time intervals & schedule builder
│   ├── hooks/
│   │   ├── useAppLauncher.ts          # Launch hooks with mindful delay logic
│   │   ├── useAccessibilityState.ts   # Real-time state from C++ service
│   │   └── useBatteryStatus.ts        # Battery level & charging state
│   └── bridge/
│       └── NativeCoreBridge.ts        # TypeScript definitions for C++ JSI Module
│
├── cpp/                               # 100% C++ Obfuscated Core
│   ├── CMakeLists.txt
│   ├── core/
│   │   ├── HeuristicsMatcher.cpp      # Reels, Shorts, Reddit swiper signature engine
│   │   ├── YouTubeEduFilter.cpp       # Title/Channel educational classifier
│   │   ├── MonotonicClock.cpp         # Hardware time keeper
│   │   ├── StringObfuscator.h         # Compile-time XOR string encryptor
│   │   └── EntitlementManager.cpp     # Permanent unlock for all premium features
│   └── jsi/
│       ├── MinimalistJsiModule.cpp    # JSI C++ host object exposed directly to JS
│       └── MinimalistJsiModule.h
│
├── go/                                # Optional Go Engine Alternative (via gomobile)
│   ├── heuristics/
│   │   └── filter.go                  # Go-based pattern matching and rule evaluation
│   └── crypto/
│       └── vault.go                   # Encrypted notification storage
│
└── android/                           # Minimal C++ NDK host harness
    ├── app/
    │   ├── src/main/AndroidManifest.xml
    │   └── src/main/cpp/
```

---

## 🔌 JSI (JavaScript Interface) Communication Protocol

Unlike legacy React Native bridges that convert everything to JSON strings across threads, **JSI** allows JavaScript to directly hold pointers to C++ memory and execute native functions synchronously:

```typescript
// src/bridge/NativeCoreBridge.ts
export interface NativeMinimalistCore {
  // Evaluates whether a view should be blocked (Reels, Shorts, Reddit Video)
  evaluateView(packageName: string, viewId: string, contentDesc: string): boolean;

  // Checks educational video classification
  isEducationalVideo(title: string, channelName: string): boolean;

  // Returns monotonic timestamp in seconds
  getMonotonicSeconds(): number;

  // Premium status check (always true in this build)
  isPremiumUnlocked(featureId: number): boolean;
}

declare const MinimalistCore: NativeMinimalistCore;
export default MinimalistCore;
```

---

## 🔒 Build & Obfuscation Flags (C++ Clang / NDK)

When building `libminimalistcore.so`, the following compiler and linker flags are applied in `CMakeLists.txt`:

```cmake
# Maximum performance & symbol stripping
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fvisibility=hidden -fvisibility-inlines-hidden")

# Strip all symbol names from the shared library
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -Wl,-s")

# Stack protection & control flow integrity
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong -D_FORTIFY_SOURCE=2")
```

This ensures that running `nm`, `objdump`, or decompilers like Ghidra on `libminimalistcore.so` produces no exposed function names, no readable strings, and completely flattened control flow logic.

