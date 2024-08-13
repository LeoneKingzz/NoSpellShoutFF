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
                if (Yen->As<RE::SpellItem>()->avEffectSetting){
                    auto Ciri = Yen->As<RE::SpellItem>()->avEffectSetting;
                    static auto MagicDamageFire = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFire");
                    static auto MagicDamageFrost = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFrost");
                    static auto MagicDamageShock = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageShock");
                    static auto MagicShout = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicShout");

                    if ((Ciri->data.flags.all(RE::EffectSetting::EffectSettingData::Flag::kHostile) || Ciri->HasKeyword(MagicDamageFire) || Ciri->HasKeyword(MagicDamageFrost) || Ciri->HasKeyword(MagicDamageShock) || Ciri->HasKeyword(MagicShout)) 
                    && (Ciri->HasArchetype(RE::EffectSetting::Archetype::kValueModifier) || Ciri->HasArchetype(RE::EffectSetting::Archetype::kPeakValueModifier) || Ciri->HasArchetype(RE::EffectSetting::Archetype::kStagger) 
                    || Ciri->HasArchetype(RE::EffectSetting::Archetype::kDemoralize) || Ciri->HasArchetype(RE::EffectSetting::Archetype::kParalysis) || Ciri->HasArchetype(RE::EffectSetting::Archetype::kAbsorb) || Ciri->HasArchetype(RE::EffectSetting::Archetype::kFrenzy)))
                    {
                        auto MagiceffectList = Yen->As<RE::SpellItem>()->effects;
                        for (auto EachMagicEffect : MagiceffectList)
                        {
                            auto formConditions = EachMagicEffect->baseEffect->conditions;
                            const auto newNode = new RE::TESConditionItem;
                            if (newNode)
                            {
                                newNode->next = nullptr;
                                newNode->data.object = RE::CONDITIONITEMOBJECT::kSelf;
                                newNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsHostileToActor;
                                newNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kEqualTo;
                                newNode->data.flags.swapTarget = true;
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
                        // newNode->data = conditionData;
                        //  RE::CONDITION_ITEM_DATA condData;
                        //  std::vector<RE::CONDITION_ITEM_DATA> dataVec{};
                    }
                }
            }
        }
    }
}