# Minimalist Phone Launcher (React Native + C++ / Go Obfuscated Engine)

An open-source, privacy-first digital wellbeing Android launcher engineered to reclaim human attention, eliminate doom-scrolling, and transform smartphones into calm, intentional tools.

This repository hosts the **complete reverse-engineered architecture, live ADB device inspection, permission security guide, and comprehensive engineering specifications** replicating *Minimalist Phone* (`com.qqlabs.minimalistlauncher`), featuring **100% unlocked premium capabilities, pure React Native UI, and a C++ / Go obfuscated background engine**.

---

## 📚 Complete Technical Documentation

| Document | Focus & Contents |
|---|---|
| 📄 [**PERMISSIONS_AND_SECURITY.md**](docs/PERMISSIONS_AND_SECURITY.md) | In-depth breakdown of all 14 Android permissions (`BIND_ACCESSIBILITY_SERVICE`, `BIND_NOTIFICATION_LISTENER_SERVICE`, `SYSTEM_ALERT_WINDOW`, `PACKAGE_USAGE_STATS`, etc.) and why each is strictly required. |
| 📄 [**ARCHITECTURE_AND_INSPECTION.md**](docs/ARCHITECTURE_AND_INSPECTION.md) | Real-device ADB dumpsys findings from `com.qqlabs.minimalistlauncher`: Activity tree, ViewPager layout, battery arc canvas widget, and complete settings hierarchy. |
| 📄 [**INSTAGRAM_MODAL_INSPECTION.md**](docs/INSTAGRAM_MODAL_INSPECTION.md) | Live-captured inspection of tapping Instagram: *"How much time do you want to spend on Instagram now?"* (5m / 10m / 15m / custom time picker / floating timer HUD). |
| 📄 [**SHORT_FORM_VIDEO_BLOCKER_SPEC.md**](docs/SHORT_FORM_VIDEO_BLOCKER_SPEC.md) | Comprehensive heuristic specs for blocking **Instagram Reels**, **YouTube Shorts**, **Reddit Video swipe carousels**, and an optional **YouTube Educational-Only Mode** (channel whitelisting & entertainment filtering). |
| 📄 [**REACT_NATIVE_AND_CPP_ARCHITECTURE.md**](docs/REACT_NATIVE_AND_CPP_ARCHITECTURE.md) | Pure React Native UI architecture (Fabric/JSI) paired with a compiled, stripped C++ / Go NDK engine for tamper-resistant obfuscation without Kotlin UI code. |
| 📄 [**FEATURE_SPECIFICATION.md**](docs/FEATURE_SPECIFICATION.md) | Algorithmic state machines, data models, and workflow specifications for all core and premium features. |
| 📄 [**OBFUSCATION_AND_TECH_STACK.md**](docs/OBFUSCATION_AND_TECH_STACK.md) | Technical comparison of Dalvik bytecode vulnerability vs C++ ELF binary stripping, compile-time string encryption, and monotonic hardware clocks. |

---

## 🌟 Key Features & Unlocked Capabilities

- **Zero Dopamine UI:** Pure typography-driven home screen and drawer. No colorful app badges or notification dots.
- **Uncapped Favorites:** No 4-app limitation; add as many favorite apps as desired.
- **Circular Battery & Clock Widget:** Custom canvas drawing an active battery percentage arc around the digital clock.
- **Short-Form Video Filter:**
  - **Instagram Reels:** Blocks the vertical viewer or restricts it to custom daily budgets (e.g., 5 min/day), while leaving Direct Messages fully accessible.
  - **YouTube Shorts:** Intercepts the Shorts pivot tab and viewer feed.
  - **Reddit Video Swiper:** Neutralizes TikTok-style fullscreen vertical video feeds while preserving post discussions.
  - **YouTube Educational Mode:** Whitelist approved educational channels (e.g., MIT OCW, 3Blue1Brown, freeCodeCamp) and block entertainment/clickbait categories.
- **Mindful App Delays & Time Modals:** Configurable 15-second breathing pauses and on-launch time pickers (5 / 10 / 15 / custom minutes).
- **In-App Floating Progress Bar:** Real-time countdown timer floating over active target apps.
- **Custom Blocking Schedules:** Recurring focus routines (e.g. *Mindful Morning* 06:30–09:00, *No Screen Evening* 22:15–01:00).
- **Notification Filter & Digest Inbox:** Intercepts distracting notifications and collects them into a calm launcher digest.
- **Per-App Monochrome Mode:** Automatically converts distracting apps to grayscale via Android display Daltonizer.
- **Fast Alphabet Scrubber & Instant Fuzzy Search:** Seamlessly navigate all installed apps with haptic ticks.

---

## 🏗️ Technology Stack

- **UI Layer:** React Native (TypeScript, React Hooks, Skia Canvas for battery arc).
- **Native Interop:** React Native JSI (JavaScript Interface) — zero serialization overhead directly accessing C++ memory.
- **Core Engine & Obfuscation:** Modern C++ (C++20) compiled via Clang NDK with `-O3 -fvisibility=hidden -s` (or Go via `cgo`/`gomobile`).
- **Target OS:** Android 8.0+ (API level 26 to 34+).
