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
                static auto MagicDamageFire = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFire");
                static auto MagicDamageFrost = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFrost");
                static auto MagicDamageShock = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageShock");
                static auto MagicShout = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicShout");

                auto MagiceffectList = Yen->As<RE::SpellItem>()->effects;
                for (auto EachMagicEffect : MagiceffectList)
                {
                    if ((EachMagicEffect->baseEffect->data.flags.all(RE::EffectSetting::EffectSettingData::Flag::kHostile) || EachMagicEffect->baseEffect->HasKeyword(MagicDamageFire) || EachMagicEffect->baseEffect->HasKeyword(MagicDamageFrost) || EachMagicEffect->baseEffect->HasKeyword(MagicDamageShock) || EachMagicEffect->baseEffect->HasKeyword(MagicShout)) && (EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kValueModifier) || EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kPeakValueModifier) || EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kStagger) || EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kDemoralize) || EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kParalysis) || EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kAbsorb) || EachMagicEffect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kFrenzy)))
                    {
                        logger::info("Patching Conditions");
                        //logger::info("Protagnist {} ReflexScore {}"sv, a_actor->GetName(), dodge_chance);
                        auto formConditions = EachMagicEffect->baseEffect->conditions;
                        auto newNode = new RE::TESConditionItem;
                        newNode->next = nullptr;
                        newNode->data.object = RE::CONDITIONITEMOBJECT::kSelf;
                        newNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsHostileToActor;
                        newNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kEqualTo;
                        // newNode->data.flags.swapTarget = true;
                        newNode->data.comparisonValue.f = 1.0f;
                        // ConditionParam cond_param;
                        // cond_param.form = nullptr;
                        newNode->data.functionData.params[0] = nullptr;

                        if (formConditions.head == nullptr)
                        {
                            formConditions.head = newNode;
                        }
                        else
                        {
                            auto *current = formConditions.head;
                            while (current->next != nullptr)
                            {
                                current = current->next;
                            }
                            current->next = newNode;
                        }
                    }
                }
            }
        }
    }
}