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
                    auto MagiceffectList = Yen->As<RE::SpellItem>()->effects;
                    for (auto EachMagicEffect : MagiceffectList){
                        auto formConditions = EachMagicEffect->baseEffect->conditions;
                        if (const auto newNode = new RE::TESConditionItem)
                        {
                            newNode->next = nullptr;
                            //newNode->data = conditionData;
                            newNode->data.object = RE::CONDITIONITEMOBJECT::kSelf;
                            newNode->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsHostileToActor;
                            newNode->data.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kEqualTo;
                            newNode->data.flags.swapTarget = true;
                            newNode->data.comparisonValue.f = 1.0f;

                            if (formConditions.head == nullptr)
                            {
                                formConditions.head = newNode;
                            }
                            else{
                                auto *current = formConditions.head;
                                while (current->next != nullptr)
                                {
                                    current = current->next;
                                }
                                current->next = newNode;
                            }


                        }
                    }
                    // RE::CONDITION_ITEM_DATA condData;
                    // std::vector<RE::CONDITION_ITEM_DATA> dataVec{};
                }
            }
        }
    }
}