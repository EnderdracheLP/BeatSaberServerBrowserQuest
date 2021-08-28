& $PSScriptRoot/build.ps1
if ($?) {
    adb push libs/arm64-v8a/libquestui.so /sdcard/Android/data/com.beatgames.beatsaber/files/libs/libquestui.so
    adb push libs/arm64-v8a/libServerBrowser.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libServerBrowser.so
    if ($?) {
        & $PSScriptRoot/restart-game.ps1
        if ($args[0] -eq "--log") {
            & $PSScriptRoot/start-logging.ps1
        }
    }
}
