& $PSScriptRoot/build.ps1
if ($?) {
    adb push build/libServerBrowser.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libServerBrowser.so
    if ($?) {
        & $PSScriptRoot/restart-game.ps1
        if ($args[0] -eq "--log") {
            & $PSScriptRoot/start-logging.ps1
        }
    }
}
