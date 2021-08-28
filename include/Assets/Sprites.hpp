#pragma once
#include <string>
#include "UnityEngine/Sprite.hpp"

namespace ServerBrowser::Assets {
    /// <summary>
    /// Helper code taken from BeatSaverDownloader
    /// Copyright (c) 2018 andruzzzhka (MIT Licensed)
    /// </summary>
    class Sprites {
        static const std::string BeatSaverB64;
        static const std::string PortalUserB64;
    
        static UnityEngine::Sprite* BeatSaverIcon;
        static UnityEngine::Sprite* PortalUser;
    public:
        static UnityEngine::Sprite* get_BeatSaverIcon();
        static UnityEngine::Sprite* get_PortalUser();

        //static void Initialize();
    };
}