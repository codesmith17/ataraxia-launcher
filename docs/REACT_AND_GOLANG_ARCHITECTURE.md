# Architecture: Pure React + Golang Native Engine
## (Zero Kotlin in UI — High-Performance Obfuscation & Security with Go)

This document establishes the architecture for developing the Minimalist Phone Launcher exclusively using **React / React Native for the user interface** and **Golang (Go 1.23+) for the native core engine, anti-tamper obfuscation, heuristics, and background services**.

---

## 🎯 Why React + Go (Golang)?

### 1. Zero Kotlin in the Presentation Layer
- Standard Android UI in Kotlin/Java suffers from verbose lifecycle management, fragmented styling, and Dalvik bytecode vulnerability.
- Using **React Native** provides:
  - Declarative, clean text-only typography styling with Flexbox.
  - Dynamic theme hot-swapping (OLED Black, Pure White, Monokai, Pastel).
  - High-performance gesture paging (`react-native-pager-view` or Skia gestures).
  - Smooth 120Hz alphabet scrubber interaction with haptic ticks.

### 2. Why Golang Over C++ for the Core Engine?
- **Superior Memory Safety:** Go eliminates pointer arithmetic corruption, use-after-free bugs, and buffer overflow vulnerabilities common in C++.
- **First-Class Concurrency:** Go’s `goroutines` and `channels` make coordinating background timers, notification listener queues, and real-time accessibility event streams straightforward:
  ```go
  go engine.WatchAccessibilityEvents(eventChan)
  go engine.RunMonotonicCountdown(timerChan)
  ```
- **Formidable Binary Obfuscation Out-of-the-Box:**
  - Compiling Go code with `gomobile bind` or `cgo` using `-ldflags="-s -w"` strips all debugging symbols, DWARF info, and symbol tables from the resulting `.so` ELF shared library.
  - Decompilers like JADX cannot touch Go binaries; reverse engineering requires disassembling raw machine code without symbol markers.
- **Cross-Platform Portability:** The Go engine logic (heuristics, schedulers, vault encryption, time tracking) compiles seamlessly for Android (`arm64-v8a`, `armeabi-v7a`), iOS, and desktop CLI.

---

## 🏗️ System Topology & Data Flow

```
┌──────────────────────────────────────────────────────────────────┐
│                   REACT NATIVE FRONTEND (UI)                     │
│                                                                  │
│  - HomeScreen.tsx            (Digital Clock + Battery Arc SVG)   │
│  - AppDrawer.tsx             (Fuzzy Search + A-Z Fast Scrubber)  │
│  - MindfulTimeModal.tsx      (5m / 10m / 15m / Custom Picker)    │
│  - SettingsAccordion.tsx     (All Settings & Schedule Toggles)   │
└─────────────────────────────────┬────────────────────────────────┘
                                  │
                 Gomobile / JNI High-Speed Bridge
                                  │
┌─────────────────────────────────▼────────────────────────────────┐
│               GOLANG NATIVE CORE ENGINE (libengine.so)           │
│                                                                  │
│   ┌──────────────────────────────────────────────────────────┐   │
│   │                      pkg/heuristics                      │   │
│   │  - ReelsMatcher.go     (Instagram Reels detection)       │   │
│   │  - ShortsMatcher.go    (YouTube Shorts detection)        │   │
│   │  - RedditMatcher.go    (Reddit video swipe feed blocker) │   │
│   │  - EduClassifier.go    (YouTube Educational filter)      │   │
│   └──────────────────────────────────────────────────────────┘   │
│   ┌──────────────────────────────────────────────────────────┐   │
│   │                        pkg/detox                         │   │
│   │  - MonotonicTimer.go   (Tamper-proof hardware timekeeper)│   │
│   │  - SessionManager.go   (Daily limits & in-app progress)  │   │
│   │  - ScheduleEngine.go   (Recurring blocking rules)        │   │
│   └──────────────────────────────────────────────────────────┘   │
│   ┌──────────────────────────────────────────────────────────┐   │
│   │                        pkg/vault                         │   │
│   │  - NotificationVault.go(AES-256 GCM encrypted digest)    │   │
│   │  - Preferences.go      (Permanent 100% Unlocked Pro)     │   │
│   └──────────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────┘
```

---

## 📂 Codebase Directory Organization

```
minimalist-phone-launcher/
├── package.json                   # React Native dependencies
├── src/                           # 100% React / TypeScript UI
│   ├── index.tsx                  # App entry point
│   ├── components/
│   │   ├── BatteryClockCanvas.tsx # Canvas battery arc and digital clock
│   │   ├── AlphabetScrubber.tsx   # Fast A-Z touch scrubber
│   │   ├── AppListItem.tsx        # Minimalist text app item
│   │   ├── MindfulTimeModal.tsx   # 5m/10m/15m launch modal
│   │   └── FloatingTimerBar.tsx   # In-app progress bar HUD
│   ├── screens/
│   │   ├── HomeScreen.tsx         # Main minimalist home
│   │   ├── AppDrawerScreen.tsx    # All apps with search
│   │   ├── SettingsScreen.tsx     # Expandable settings hub
│   │   └── SchedulesScreen.tsx    # Focus routine builder
│   └── bridge/
│       └── GoEngineBridge.ts      # TypeScript contract to Go engine
│
└── go/                            # 100% Golang Native Engine
    ├── go.mod
    ├── go.sum
    ├── cmd/
    │   └── engine/
    │       └── main.go            # Entry point for gomobile bind
    └── pkg/
        ├── heuristics/
        │   ├── reels.go           # Instagram Reels inspection
        │   ├── shorts.go          # YouTube Shorts inspection
        │   ├── reddit.go          # Reddit video swipe feed detection
        │   └── edu_filter.go      # Educational YouTube classifier
        ├── detox/
        │   ├── timer.go           # Monotonic clock counters
        │   └── scheduler.go       # Focus blocking schedules
        ├── vault/
        │   └── notifications.go   # Encrypted notification batch store
        └── entitlement/
            └── license.go         # 100% Permanently unlocked features
```

---

## 💻 Sample Golang Implementation Modules

### 1. Heuristics Matcher: Reels, Shorts & Reddit (`pkg/heuristics/matcher.go`)

```go
package heuristics

import (
	"strings"
	"sync"
)

type ContentType int

const (
	TypeNone ContentType = iota
	TypeInstagramReels
	TypeYouTubeShorts
	TypeRedditVideoSwipe
)

type HeuristicsEngine struct {
	mu           sync.RWMutex
	reelsBlocked bool
	shortsBlocked bool
	redditBlocked bool
}

func NewEngine() *HeuristicsEngine {
	return &HeuristicsEngine{
		reelsBlocked:  true,
		shortsBlocked: true,
		redditBlocked: true,
	}
}

// EvaluateViewNode checks incoming accessibility view nodes in real-time
func (e *HeuristicsEngine) EvaluateViewNode(packageName, viewID, contentDesc string) ContentType {
	e.mu.RLock()
	defer e.mu.RUnlock()

	switch packageName {
	case "com.instagram.android":
		if e.reelsBlocked {
			if strings.Contains(viewID, "clips_viewer") || 
			   strings.Contains(viewID, "reel_viewer") ||
			   strings.EqualFold(contentDesc, "Reels") {
				return TypeInstagramReels
			}
		}

	case "com.google.android.youtube":
		if e.shortsBlocked {
			if strings.Contains(viewID, "shorts_container") || 
			   strings.Contains(viewID, "reel_player") ||
			   strings.Contains(viewID, "pivot_bar_shorts") {
				return TypeYouTubeShorts
			}
		}

	case "com.reddit.frontpage":
		if e.redditBlocked {
			if strings.Contains(viewID, "media_viewer_pager") || 
			   strings.Contains(viewID, "video_feed_recycler_view") {
				return TypeRedditVideoSwipe
			}
		}
	}

	return TypeNone
}
```

---

### 2. YouTube Educational Video Classifier (`pkg/heuristics/edu_filter.go`)

```go
package heuristics

import (
	"strings"
)

type EduFilter struct {
	WhitelistedChannels []string
	BannedKeywords      []string
	ApprovedKeywords    []string
}

func NewEduFilter() *EduFilter {
	return &EduFilter{
		WhitelistedChannels: []string{
			"MIT OpenCourseWare", "freeCodeCamp.org", "3Blue1Brown",
			"Veritasium", "Lex Fridman", "Computerphile", "Khan Academy",
		},
		BannedKeywords: []string{
			"prank", "reaction", "vlog", "fortnite", "gta", "drama",
			"celebrity", "shocking", "tiktok", "challenge",
		},
		ApprovedKeywords: []string{
			"tutorial", "lecture", "course", "explained", "calculus",
			"architecture", "golang", "react", "physics", "system design",
		},
	}
}

// IsAllowedVideo evaluates if a YouTube video meets educational criteria
func (f *EduFilter) IsAllowedVideo(channelName, videoTitle string) (bool, string) {
	// Check Whitelist
	for _, ch := range f.WhitelistedChannels {
		if strings.EqualFold(ch, channelName) {
			return true, "Channel Whitelisted"
		}
	}

	lowerTitle := strings.ToLower(videoTitle)

	// Check Banned Keywords
	for _, ban := range f.BannedKeywords {
		if strings.Contains(lowerTitle, ban) {
			return false, "Entertainment / Clickbait Keyword Detected: " + ban
		}
	}

	// Check Approved Educational Keywords
	for _, app := range f.ApprovedKeywords {
		if strings.Contains(lowerTitle, app) {
			return true, "Educational Topic Approved: " + app
		}
	}

	// Default policy when educational mode is active
	return false, "Video not verified as educational content"
}
```

---

### 3. Tamper-Proof Monotonic Hardware Clock (`pkg/detox/timer.go`)

```go
package detox

import (
	"time"
)

type MonotonicTimer struct {
	targetDuration time.Duration
	startTicks     time.Time
}

func NewTimer(minutes int) *MonotonicTimer {
	return &MonotonicTimer{
		targetDuration: time.Duration(minutes) * time.Minute,
		startTicks:     time.Now(),
	}
}

// RemainingSeconds calculates remaining duration without relying on wall-clock time
func (m *MonotonicTimer) RemainingSeconds() int {
	elapsed := time.Since(m.startTicks)
	remaining := m.targetDuration - elapsed
	if remaining <= 0 {
		return 0
	}
	return int(remaining.Seconds())
}

func (m *MonotonicTimer) IsExpired() bool {
	return m.RemainingSeconds() <= 0
}
```

---

### 4. 100% Unlocked License Manager (`pkg/entitlement/license.go`)

```go
package entitlement

type FeatureCode int

const (
	FeatureUnlimitedFavorites FeatureCode = iota
	FeatureBlockingSchedules
	FeatureShortsAndReelsBlocker
	FeatureRedditSwiperBlocker
	FeatureYouTubeEduFilter
	FeatureCustomThemes
	FeatureMonochromeMode
	FeatureNotificationFilter
)

// IsUnlocked permanently validates all premium features as active
func IsUnlocked(feature FeatureCode) bool {
	return true // 100% Open-Source & Unlocked
}
```

---

## 🛠️ Build & Compilation Workflow

To compile the Go engine into a stripped Android native library:

```bash
# Install gomobile if not present
go install golang.org/x/mobile/cmd/gomobile@latest
gomobile init

# Compile Go package into Android .aar with stripped debug symbols
cd go
gomobile bind -target=android/arm64,android/arm -ldflags="-s -w" -o ../android/engine.aar ./cmd/engine
```

The resulting `engine.aar` contains:
- `libgojni.so` (stripped of all symbols with `-s -w`).
- Zero decompilable Java business logic — all heuristics, timers, and rules execute in native machine code.
- Direct invocation from React Native via standard React Native NativeModule wrapper.
