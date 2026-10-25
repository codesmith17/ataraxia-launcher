package com.ataraxia.launcher.data.model

data class AppItem(
    val packageName: String,
    val activityName: String,
    val label: String,
    val customLabel: String? = null,
    val isFavorite: Boolean = false,
    val isHidden: Boolean = false,
    val isMonochrome: Boolean = false,
    val hasTimeReminder: Boolean = false,
    val targetTimeMinutes: Int = 15
) {
    val displayName: String
        get() = customLabel ?: label
}

data class BlockingSchedule(
    val id: String,
    val name: String,
    val startHour: Int,
    val startMinute: Int,
    val endHour: Int,
    val endMinute: Int,
    val daysOfWeek: List<Int>, // 1 = Monday ... 7 = Sunday
    val blockedPackages: Set<String>,
    val isEnabled: Boolean = true
) {
    val timeLabel: String
        get() = String.format("%02d:%02d - %02d:%02d", startHour, startMinute, endHour, endMinute)
}

data class FilteredNotification(
    val id: Long = 0,
    val packageName: String,
    val appName: String,
    val title: String?,
    val text: String?,
    val timestamp: Long,
    val isRead: Boolean = false
)

enum class ColorTheme {
    DARK_AMOLED,
    PURE_WHITE,
    MONOCHROME_GRAY,
    MONOKAI_ACCENT,
    PASTEL_CALM
}

enum class FontSize {
    SMALL,
    MEDIUM,
    LARGE,
    EXTRA_LARGE
}
