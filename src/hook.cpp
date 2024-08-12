#include "hook.h"

namespace hooks
{
    void util::install()
    {
        auto Triss = get_all<RE::SpellItem>();

        for (RE::SpellItem *Yen: Triss)
        {
            if (Yen && Yen->Is(RE::FormType::Spell))
            {
                if (Yen->As<RE::SpellItem>()->avEffectSetting->data.flags.all(RE::EffectSetting::EffectSettingData::Flag::kHostile))
                {
                    return;
                }
            }
        }
    }
}