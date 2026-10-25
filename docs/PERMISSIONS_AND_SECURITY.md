# Permissions & Security Architecture Deep-Dive

This document details every single Android permission requested by the Ataraxia Launcher (`com.qqlabs.minimalistlauncher`), explaining **why it is required**, the **underlying Android framework APIs**, and how each permission is used.

---

## 📋 Comprehensive Permission Matrix

| # | Android Permission Identifier | Permission Level | Used By Component / Feature | Why It Is Strictly Needed |
|---|---|---|---|---|
| 1 | `android.permission.BIND_ACCESSIBILITY_SERVICE` | Special Access (User Setting) | `MinimalistPhoneAccessibilityService` | Real-time short-form video blocking (YouTube Shorts, Reels), precision foreground app detection, and double-tap home screen to lock. |
| 2 | `android.service.notification.NotificationListenerService` | Special Access (Notification Access) | `NotificationManagerService` | Notification Filter: Intercepting incoming push notifications from designated apps and batching them into a calm launcher inbox. |
| 3 | `android.permission.SYSTEM_ALERT_WINDOW` | Special Access (Overlay / Draw Over Apps) | In-App Time Reminder Engine | Displays the floating progress bar / countdown timer over active apps and renders the 15-second mindful breathing delay before launching distracting apps. |
| 4 | `android.permission.PACKAGE_USAGE_STATS` | Special Access (Usage Data Access) | `UsageStatsManager` | Tracking exact daily screen time, in-app usage durations, and enforcing blocking schedules and app time limits. |
| 5 | `android.permission.QUERY_ALL_PACKAGES` | Manifest / Play Policy Declaration | `LauncherApps` / `PackageManager` | Essential for any custom launcher to discover, list, search, and launch all installed apps on the device. |
| 6 | `android.permission.EXPAND_STATUS_BAR` | Normal (Granted at Install) | Gestures Engine | Allows the launcher to programmatically pull down the system notification shade or quick settings when the user swipes down on the home screen. |
| 7 | `android.permission.SET_WALLPAPER` | Normal (Granted at Install) | Display Settings | Automatically sets a matching pure pitch-black AMOLED wallpaper for both the home screen and lock screen. |
| 8 | `android.permission.WRITE_SECURE_SETTINGS` | Privileged (Granted via ADB) | Monochrome Mode Engine | Toggles Android's hardware color inversion / display daltonizer matrix to render distracting apps in grayscale. |
| 9 | `android.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS` | Normal (User Prompted) | Foreground Service & Alarms | Prevents aggressive OEM battery management (OnePlus Athena, Xiaomi MIUI cleaner, Samsung sleeping apps) from terminating the launcher's background timers and accessibility services. |
| 10 | `android.app.action.DEVICE_ADMIN_ENABLED` | Special Access (Device Admin) | `MDeviceAdminReceiver` | Allows instant screen locking on double-tap when the device does not support or grant the accessibility global lock action. |
| 11 | `android.permission.REQUEST_DELETE_PACKAGES` | Normal (Granted at Install) | App Drawer Context Menu | Enables initiating app uninstalls directly from the launcher long-press menu without forcing the user into system settings. |
| 12 | `android.permission.POST_NOTIFICATIONS` | Runtime (Android 13+ / Tiramisu) | Reminder Engine | Sends scheduled reminders, time-limit expiration warnings, and blocking schedule alerts. |
| 13 | `android.permission.VIBRATE` | Normal (Granted at Install) | UI Haptics | Provides crisp haptic feedback when scrolling through the alphabet scrubber, long-pressing apps, or when a mindful timer ticks down. |
| 14 | `android.permission.WAKE_LOCK` | Normal (Granted at Install) | WorkManager / AlarmManager | Ensures exact alarms for blocking schedules (e.g., locking distracting apps at 22:15) fire precisely on time even while the device is sleeping. |

---

## 🔍 In-Depth Technical Justifications

### 1. `BIND_ACCESSIBILITY_SERVICE` (Accessibility Service)
- **Why It Is Needed:**
  1. **Short Video Feeds Blocker:** Android provides no public intent or callback when a user taps the "Shorts" tab in YouTube or the "Reels" tab in Instagram. An `AccessibilityService` listens to `TYPE_WINDOW_CONTENT_CHANGED` and `TYPE_VIEW_CLICKED` events. When view resource IDs matching `shorts_container`, `reels_tray`, or `reel_viewer` are detected, the service immediately calls `performGlobalAction(GLOBAL_ACTION_BACK)` or launches the home screen to break the doom-loop.
  2. **Precision Foreground Detection:** On modern Android versions, polling `UsageStatsManager` can have a 5–15 second latency. The accessibility service receives immediate `TYPE_WINDOW_STATE_CHANGED` events the millisecond a target package becomes active.
  3. **Double-Tap to Lock:** Allows invoking `performGlobalAction(GLOBAL_ACTION_LOCK_SCREEN)` without needing device administrator rights on Android 9+.

---

### 2. `BIND_NOTIFICATION_LISTENER_SERVICE` (Notification Listener)
- **Why It Is Needed:**
  - Modern app notifications are intentionally engineered to trigger dopamine pings. Minimalist Phone features a **Notification Filter**.
  - When an app (e.g. Instagram, WhatsApp, Twitter) sends a push notification, the `NotificationManagerService` intercepts the `StatusBarNotification` via `onNotificationPosted()`.
  - If the originating package is configured as "Filtered", the service calls `cancelNotification(sbn.key)` to remove the banner and sound from the system tray, and saves the message locally in an internal SQLite/Room database.
  - The user can then open **Filtered Notifications** inside the launcher at designated mindful times to read them in batch.

---

### 3. `SYSTEM_ALERT_WINDOW` (Display Over Other Apps)
- **Why It Is Needed:**
  - **In-App Floating Timer:** When using a time-restricted app, a non-intrusive horizontal progress bar and countdown indicator is drawn in the top corner of the screen using `WindowManager.addView()` with `LayoutParams.TYPE_APPLICATION_OVERLAY`.
  - **Mindful Launch Delay Modal:** When tapping an app subject to launch delay, a 15-second breathing countdown screen is displayed before passing the launch intent to the target app.
  - **Lockout Screen:** When an app's allotted time expires, a full-screen blocking overlay is drawn over the app with options to "Exit App" or "Take a Mindful Extension (wait 30s)".

---

### 4. `PACKAGE_USAGE_STATS` (Usage Access)
- **Why It Is Needed:**
  - Querying `UsageStatsManager.queryUsageStats()` to calculate total daily screen time.
  - Calculating exact minutes spent inside specific apps over rolling 24-hour periods.
  - Displaying educational statistics (e.g., "Every day you lose up to 8 hours by being on your phone").

---

### 5. `WRITE_SECURE_SETTINGS` (Monochrome Mode)
- **Why It Is Needed:**
  - To turn specific distracting apps into black-and-white (grayscale).
  - Colorful interfaces keep users engaged 40% longer. By modifying the system settings:
    ```bash
    Settings.Secure.putInt(contentResolver, "accessibility_display_daltonizer_enabled", 1);
    Settings.Secure.putInt(contentResolver, "accessibility_display_daltonizer", 0); # 0 = Grayscale (Monochrome)
    ```
  - When the user exits the app back to the launcher or switches to a utility app (e.g. Maps/Camera), the daltonizer is turned back off (`0`).
  - *Granting via ADB:*
    ```bash
    adb shell pm grant com.ataraxia.launcher android.permission.WRITE_SECURE_SETTINGS
    ```

---

### 6. `QUERY_ALL_PACKAGES`
- **Why It Is Needed:**
  - Starting in Android 11 (API 30), apps cannot see other installed applications by default due to package visibility filtering.
  - A Home Launcher (`CATEGORY_HOME`) is explicitly recognized by Google Play as an exempted core use case for `QUERY_ALL_PACKAGES` because it cannot function without indexing all installed launcher activities.

---

### 7. `EXPAND_STATUS_BAR`
- **Why It Is Needed:**
  - Provides natural single-hand ergonomics. Swiping down anywhere on the minimalist home screen triggers:
    ```kotlin
    val statusBarService = context.getSystemService("statusbar")
    val statusBarManager = Class.forName("android.app.StatusBarManager")
    val expandMethod = statusBarManager.getMethod("expandNotificationsPanel")
    expandMethod.invoke(statusBarService)
    ```

---

### 8. `SET_WALLPAPER`
- **Why It Is Needed:**
  - Minimalist design depends on pure pitch-black AMOLED background (`#000000`).
  - Automatically updates both `FLAG_SYSTEM` (Home screen) and `FLAG_LOCK` (Lock screen) wallpapers using `WallpaperManager.getInstance(context).setBitmap(blackBitmap)`.

---

### 9. `REQUEST_IGNORE_BATTERY_OPTIMIZATIONS`
- **Why It Is Needed:**
  - Modern OEM Android skins (OnePlus OxygenOS, Xiaomi MIUI/HyperOS, Oppo ColorOS, Vivo FuntouchOS) aggressively kill background processes to claim higher battery benchmarks.
  - If the accessibility service or alarm triggers are killed, short video blocking and time limits fail to trigger. Whitelisting the launcher prevents unprompted process termination.

---

## 🛡️ Privacy & Local Execution Guarantee

In our open-source replication:
1. **No External Network Calls:** The app requires no analytics endpoints (`INTERNET` permission is completely optional or omitted).
2. **Local Database Only:** All notification logs, usage statistics, and custom configurations remain strictly inside private app storage (`/data/data/com.ataraxia.launcher/databases/`).
3. **No Advertising IDs:** Unlike commercial builds that include `com.google.android.gms.permission.AD_ID` and install referrer SDKs, this implementation contains **zero tracking SDKs**.
