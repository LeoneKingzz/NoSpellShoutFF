#include "hook.h"

namespace hooks
{
    void animEventHandler::setglobals(){
        auto HdSingle = RE::TESDataHandler::GetSingleton();
        auto DS = GetSingleton();
        DS->NSSFFLK_Enable = skyrim_cast<RE::TESGlobal*>(HdSingle->LookupForm(0x800, "No Spell Shout FF.esp"));
        DS->NSSFFLK_Enable->value = 1.0f;
    }

    class OurEventSink : public RE::BSTEventSink<RE::TESCombatEvent>
    {
        OurEventSink() = default;
        OurEventSink(const OurEventSink &) = delete;
        OurEventSink(OurEventSink &&) = delete;
        OurEventSink &operator=(const OurEventSink &) = delete;
        OurEventSink &operator=(OurEventSink &&) = delete;

    public:
        static OurEventSink *GetSingleton()
        {
            static OurEventSink singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent *event, RE::BSTEventSource<RE::TESCombatEvent> *)
        {
            auto DS = animEventHandler::GetSingleton();
            if (DS->NSSFFLK_Enable->value == 1.0){
                return RE::BSEventNotifyControl::kContinue;
            }
            decltype(auto) sourceName = event->actor;
            auto Protagonist = sourceName->As<RE::Actor>();

            if (Protagonist->IsPlayerRef()){
                return RE::BSEventNotifyControl::kContinue;
            }
            auto CombatTarget = Protagonist->GetActorRuntimeData().currentCombatTarget.get().get();
            auto getcombatstate = event->newState.get();

            if (CombatTarget && CombatTarget->IsPlayerRef() || CombatTarget->IsPlayerTeammate()){
                if (getcombatstate == RE::ACTOR_COMBAT_STATE::kCombat){
                    if (DS->NSSFFLK_Enable->value != 1.0){
                        DS->NSSFFLK_Enable->value = 1.0f;
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
            auto Playerhandle = RE::PlayerCharacter::GetSingleton();
            auto combatGroup = Protagonist->GetCombatGroup();

            if (combatGroup){
                for (auto it = combatGroup->targets.begin(); it != combatGroup->targets.end(); ++it){
                    if (it->targetHandle && it->targetHandle.get().get() && it->targetHandle.get().get() == Playerhandle){
                        if (getcombatstate == RE::ACTOR_COMBAT_STATE::kCombat){
                            if (DS->NSSFFLK_Enable->value != 1.0){
                                DS->NSSFFLK_Enable->value = 1.0f;
                                return RE::BSEventNotifyControl::kContinue;
                            }
                        }
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }
    };

    void util::install(){
        auto eventSink = OurEventSink::GetSingleton();
        auto *eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESCombatEvent>(eventSink);
    }

	RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src)
	{
		FnProcessEvent fn = fnHash.at(*(uint64_t*)this);

		if (!a_event.holder) {
			return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
		}

		RE::Actor* actor = const_cast<RE::TESObjectREFR*>(a_event.holder)->As<RE::Actor>();
		if (!actor->IsPlayerRef()) {
			return;
		}
		auto DS = GetSingleton();
		switch (hash(a_event.tag.c_str(), a_event.tag.size())) {
		case "MLh_SpellFire_Event"_h:
		case "MRh_SpellFire_Event"_h:
		case "Voice_SpellFire_Event"_h:
			if (actor->IsSneaking() && !actor->IsInCombat()) {
				if (DS->NSSFFLK_Enable->value != 0.0) {
					DS->NSSFFLK_Enable->value = 0.0f;
				}
			} else {
				if (DS->NSSFFLK_Enable->value != 1.0) {
					DS->NSSFFLK_Enable->value = 1.0f;
				}
			}
			break;
		}

		return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
	}

	std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;

}