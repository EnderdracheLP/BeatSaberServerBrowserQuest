#pragma once
#include "GlobalNamespace/BeatmapDifficultyMask.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"

namespace ServerBrowser::Models::Enums {
    enum BssbDifficulty {
        All = -1,
        Easy = 0,
        Normal = 1,
        Hard = 2,
        Expert = 3,
        ExpertPlus = 4
    };

    class DifficultyUtils {
        // TODO: Move function bodies into cpp!!!

        static BssbDifficulty ToBssbDifficulty(GlobalNamespace::BeatmapDifficultyMask mask) {
            switch (mask) {
                case GlobalNamespace::BeatmapDifficultyMask::All:
                    return BssbDifficulty::All;
                case GlobalNamespace::BeatmapDifficultyMask::Easy:
                    return BssbDifficulty::Easy;
                case GlobalNamespace::BeatmapDifficultyMask::Normal:
                    return BssbDifficulty::Normal;
                case GlobalNamespace::BeatmapDifficultyMask::Hard:
                    return BssbDifficulty::Hard;
                case GlobalNamespace::BeatmapDifficultyMask::Expert:
                    return BssbDifficulty::Expert;
                case GlobalNamespace::BeatmapDifficultyMask::ExpertPlus:
                    return BssbDifficulty::ExpertPlus;
                default:
                    return BssbDifficulty::All;
            }
        }

        static BssbDifficulty ToBssbDifficulty(GlobalNamespace::BeatmapDifficulty mask) {
            switch (mask) {
                case GlobalNamespace::BeatmapDifficulty::Easy:
                    return BssbDifficulty::Easy;
                case GlobalNamespace::BeatmapDifficulty::Normal:
                    return BssbDifficulty::Normal;
                case GlobalNamespace::BeatmapDifficulty::Hard:
                    return BssbDifficulty::Hard;
                case GlobalNamespace::BeatmapDifficulty::Expert:
                    return BssbDifficulty::Expert;
                case GlobalNamespace::BeatmapDifficulty::ExpertPlus:
                    return BssbDifficulty::ExpertPlus;
                default:
                    return BssbDifficulty::All;
            }
        }

        static StringW ToFormattedText(BssbDifficulty difficulty) {
            switch (difficulty) {
                case BssbDifficulty::All: 
                    return "<color=#dbbb48>All</color>";
                case BssbDifficulty::Easy:
                    return "<color=#3cb371>Easy</color>";
                case BssbDifficulty::Normal: 
                    return "<color=#59b0f4>Normal</color>";
                case BssbDifficulty::Hard: 
                    return "<color=#ff6347>Hard</color>";
                case BssbDifficulty::Expert: 
                    return "<color=#bf2a42>Expert</color>";
                case BssbDifficulty::ExpertPlus: 
                    return "<color=#8f48db>Expert+</color>";
                default:
                    return "<color=#bcbdc2>Unknown</color>";
            }
        }
    }
}
