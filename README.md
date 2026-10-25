# Minimalist Phone Launcher (React + Golang Architecture)

An open-source, **100% on-device (zero external server)**, privacy-first digital wellbeing Android launcher engineered to eliminate doom-scrolling, neutralize addictive short-form video loops, and transform smartphones into calm, intentional tools.

This repository contains the **complete reverse-engineered architecture, live ADB device inspection, competitive market research, and full engineering specifications** replicating *Minimalist Phone* (`com.qqlabs.minimalistlauncher`), featuring **100% unlocked premium capabilities, pure React Native for the presentation layer, and a compiled, obfuscated Golang native engine running completely locally on-device**.

---

## 🔒 100% On-Device & Zero-Server Guarantee

- **No External Servers:** No remote backend, no cloud proxies, and no external AI/NLP APIs. Everything runs in-process on your phone's CPU.
- **Zero Internet Permission Required:** The application operates completely air-gapped without requiring network access.
- **Instant Offline Execution:** All heuristics (Instagram Reels, YouTube Shorts, Reddit video swiper, YouTube Educational title classifier) execute locally in < 1.5ms.

---

## 📚 Complete Technical Documentation

| Document | Focus & Contents |
|---|---|
| 📄 [**ON_DEVICE_ZERO_SERVER_ARCHITECTURE.md**](docs/ON_DEVICE_ZERO_SERVER_ARCHITECTURE.md) | **100% On-Device / Zero-Server Guarantee**: How the app runs completely offline in Airplane Mode with zero network calls, local trie-based NLP classification, and in-process Go engine execution. |
| 📄 [**PREMIUM_FEATURES_AND_COMPETITORS_RESEARCH.md**](docs/PREMIUM_FEATURES_AND_COMPETITORS_RESEARCH.md) | **In-depth market research**: Minimalist Phone's pricing models vs. competitors (**Niagara Pro**, **Before Launcher Pro**, **Indistractable Pro**, **Olauncher**, **One Sec**, **Opal**), feature comparison matrix, and our 100% free unlocked strategy. |
| 📄 [**REACT_AND_GOLANG_ARCHITECTURE.md**](docs/REACT_AND_GOLANG_ARCHITECTURE.md) | **Pure React + Golang architecture**: Zero Kotlin in UI, Go 1.23+ native engine compiled via `gomobile` (`-ldflags="-s -w"`), high-concurrency event streams, and anti-tamper security. |
| 📄 [**SHORT_FORM_VIDEO_BLOCKER_SPEC.md**](docs/SHORT_FORM_VIDEO_BLOCKER_SPEC.md) | Heuristic specs for blocking **Instagram Reels**, **YouTube Shorts**, **Reddit Video swipe carousels**, and an optional **YouTube Educational-Only Mode** (channel whitelist & clickbait category filter). |
| 📄 [**INSTAGRAM_MODAL_INSPECTION.md**](docs/INSTAGRAM_MODAL_INSPECTION.md) | Live-captured inspection of tapping Instagram: *"How much time do you want to spend on Instagram now?"* (5m / 10m / 15m / custom time picker / floating timer HUD). |
| 📄 [**PERMISSIONS_AND_SECURITY.md**](docs/PERMISSIONS_AND_SECURITY.md) | In-depth breakdown of all 14 Android permissions (`BIND_ACCESSIBILITY_SERVICE`, `BIND_NOTIFICATION_LISTENER_SERVICE`, `SYSTEM_ALERT_WINDOW`, `PACKAGE_USAGE_STATS`, etc.) and why each is required. |
| 📄 [**ARCHITECTURE_AND_INSPECTION.md**](docs/ARCHITECTURE_AND_INSPECTION.md) | Real-device ADB dumpsys findings from `com.qqlabs.minimalistlauncher`: Activity tree, ViewPager layout, battery arc canvas widget, and complete settings hierarchy. |
| 📄 [**FEATURE_SPECIFICATION.md**](docs/FEATURE_SPECIFICATION.md) | Algorithmic state machines, data models, and workflow specifications for all core and premium features. |
| 📄 [**OBFUSCATION_AND_TECH_STACK.md**](docs/OBFUSCATION_AND_TECH_STACK.md) | Technical comparison of Dalvik bytecode vulnerability vs stripped native machine code binaries. |

---

## 🌟 Key Features & Unlocked Capabilities

- **Zero Dopamine UI:** Pure typography-driven home screen and drawer. No colorful app badges or notification dots.
- **Uncapped Favorites:** No 4-app limitation; add as many favorite apps as desired.
- **Circular Battery & Clock Widget:** Custom canvas drawing an active battery percentage arc around the digital clock.
- **Short-Form Video Filter:**
  - **Instagram Reels:** Blocks the vertical viewer or restricts it to custom daily budgets (e.g., 5 min/day), while leaving Direct Messages fully accessible.
  - **YouTube Shorts:** Intercepts the Shorts pivot tab and viewer feed.
  - **Reddit Video Swiper:** Neutralizes TikTok-style fullscreen vertical video feeds while preserving post discussions.
  - **YouTube Educational Mode:** Whitelist approved educational channels (e.g., MIT OCW, 3Blue1Brown, freeCodeCamp) and block entertainment/clickbait categories on-device.
- **Mindful App Delays & Time Modals:** Configurable 15-second breathing pauses and on-launch time pickers (5 / 10 / 15 / custom minutes).
- **In-App Floating Progress Bar:** Real-time countdown timer floating over active target apps.
- **Custom Blocking Schedules:** Recurring focus routines (e.g. *Mindful Morning* 06:30–09:00, *No Screen Evening* 22:15–01:00).
- **Notification Filter & Digest Inbox:** Intercepts distracting notifications and collects them into a calm launcher digest.
- **Per-App Monochrome Mode:** Automatically converts distracting apps to grayscale via Android display Daltonizer.
- **Fast Alphabet Scrubber & Instant Fuzzy Search:** Seamlessly navigate all installed apps with haptic ticks.

---

## 🏗️ Technology Stack

- **UI Layer:** React Native (TypeScript, Flexbox, Skia/SVG Canvas for battery arc).
- **Core Engine & Anti-Tamper:** Golang 1.23+ compiled via `gomobile` into stripped ELF shared libraries (`-ldflags="-s -w"`).
- **Target OS:** Android 8.0+ (API level 26 to 34+).
- **Network / Cloud Footprint:** **Zero (100% on-device execution).**
