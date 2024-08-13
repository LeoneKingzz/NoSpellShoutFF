#include "hook.h"

namespace hooks
{

    void util::install()
    {
        //const auto dataHandler = RE::TESDataHandler::GetSingleton();
        auto MagiceffectList = get_all<RE::EffectSetting>();

        static auto MagicDamageFire = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFire");
        static auto MagicDamageFrost = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFrost");
        static auto MagicDamageShock = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageShock");
        static auto MagicShout = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicShout");

        for (auto EachMagicEffect : MagiceffectList)
        {
            if (!((EachMagicEffect->data.flags.all(RE::EffectSetting::EffectSettingData::Flag::kHostile) || EachMagicEffect->HasKeyword(MagicDamageFire) || EachMagicEffect->HasKeyword(MagicDamageFrost) || EachMagicEffect->HasKeyword(MagicDamageShock) || EachMagicEffect->HasKeyword(MagicShout)) && (EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kValueModifier) || EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kPeakValueModifier) || EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kStagger) || EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kDemoralize) || EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kParalysis) || EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kAbsorb) || EachMagicEffect->HasArchetype(RE::EffectSetting::Archetype::kFrenzy))))
            {
                logger::info("Skipping Invalid Magic Effect");
                continue;
            }
            logger::info("Patching Conditions");
            // logger::info("Patching Conditions");
            // logger::info("Protagnist {} ReflexScore {}"sv, a_actor->GetName(), dodge_chance); auto HdSingle = RE::TESDataHandler::GetSingleton();
            auto formConditions = EachMagicEffect->conditions;
            auto newNode = new RE::TESConditionItem;
            newNode->next = nullptr;
            auto player = RE::PlayerCharacter::GetSingleton();
            RE::CONDITION_ITEM_DATA condData;
            condData.object = RE::CONDITIONITEMOBJECT::kTarget;
            condData.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsHostileToActor;
            condData.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kEqualTo;
            // newNode->data.flags.swapTarget = true;
            condData.comparisonValue.f = 1.0f;
            ConditionParam cond_param;
            cond_param.form = const_cast<RE::TESObjectREFR*>(player->As<RE::TESObjectREFR>());
            newNode->data.functionData.params[0] = std::bit_cast<void *>(cond_param);
            newNode->data = condData;

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
            logger::info("Parsing Next Conditions");
        }
        logger::info("Patching Complete");
    }
}