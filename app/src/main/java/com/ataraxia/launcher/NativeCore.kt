package com.ataraxia.launcher

object NativeCore {
    init {
        try {
            System.loadLibrary("ataraxiacore")
        } catch (e: UnsatisfiedLinkError) {
            // Fallback if native library is compiled separately
        }
    }

    const val FEATURE_UNLIMITED_FAVORITES = 0
    const val FEATURE_BLOCKING_SCHEDULES = 1
    const val FEATURE_REELS_BLOCKER = 2
    const val FEATURE_ALL_THEMES = 3
    const val FEATURE_MONOCHROME_MODE = 4
    const val FEATURE_IN_APP_TIMERS = 5

    external fun isShortVideoContainer(
        packageName: String?,
        viewId: String?,
        contentDesc: String?
    ): Boolean

    external fun getMonotonicTimeSeconds(): Long

    external fun isPremiumFeatureActive(featureCode: Int): Boolean

    // Fallback implementations if native library is not yet loaded
    fun isShortVideoFallback(packageName: String?, viewId: String?, contentDesc: String?): Boolean {
        if (packageName == "com.google.android.youtube") {
            if (viewId?.contains("shorts_container") == true || viewId?.contains("reel_player") == true) {
                return true
            }
        }
        if (packageName == "com.instagram.android") {
            if (viewId?.contains("clips_viewer") == true || contentDesc?.contains("Reels") == true) {
                return true
            }
        }
        return false
    }
}
