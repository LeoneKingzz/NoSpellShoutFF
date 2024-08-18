#include "hook.h"

namespace hooks
{
    void on_animation_event::setglobals(){
        auto HdSingle = RE::TESDataHandler::GetSingleton();
        auto DS = GetSingleton();
        DS->NSSFFLK_Enable = skyrim_cast<RE::TESGlobal*>(HdSingle->LookupForm(0x800, "No Spell Shout FF.esp"));
        DS->NSSFFLK_Enable->value = 1.0f;
    }

    void util::install(){
        auto eventSink = OurEventSink::GetSingleton();

        // ScriptSource
        auto *eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESCombatEvent>(eventSink);
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
            auto DS = on_animation_event::GetSingleton();
            if (DS->NSSFFLK_Enable->value == 1.0){
                return RE::BSEventNotifyControl::kContinue;
            }
            decltype(auto) sourceName = event->actor;
            auto Protagonist = sourceName->As<RE::Actor>();

            if (Protagonist->IsPlayerRef()){
                return RE::BSEventNotifyControl::kContinue;
            }
            auto CombatTarget = Protagonist->GetActorRuntimeData().currentCombatTarget.get().get();

            if (!CombatTarget){
                return RE::BSEventNotifyControl::kContinue;
            }

            auto Playerhandle = RE::PlayerCharacter::GetSingleton();
            auto getcombatstate = event->newState.get();

            if (CombatTarget->IsPlayerRef() || CombatTarget->IsPlayerTeammate()){
                if (getcombatstate == RE::ACTOR_COMBAT_STATE::kCombat){
                    if (DS->NSSFFLK_Enable->value != 1.0){
                        DS->NSSFFLK_Enable->value = 1.0f;
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }

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

    void on_animation_event::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		if (!a_event->holder) {
			return;
		}
		std::string_view eventTag = a_event->tag.data();
		RE::Actor* actor = const_cast<RE::TESObjectREFR*>(a_event->holder)->As<RE::Actor>();
        if (!actor->IsPlayerRef()){
            return;
        }
        auto DS = GetSingleton();
        switch (hash(eventTag.data(), eventTag.size())){
        case "MLh_SpellFire_Event"_h:
        case "MRh_SpellFire_Event"_h:
        case "Voice_SpellFire_Event"_h:
            if (actor->IsSneaking() && !actor->IsInCombat()){
                if (DS->NSSFFLK_Enable->value != 0.0){
                    DS->NSSFFLK_Enable->value = 0.0f;
                }
            }else{
                if (DS->NSSFFLK_Enable->value != 1.0){
                    DS->NSSFFLK_Enable->value = 1.0f;
                }
            }
            break;
        }
    }

	EventResult on_animation_event::ProcessEvent_NPC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		ProcessEvent(a_sink, a_event, a_eventSource);
		return _ProcessEvent_NPC(a_sink, a_event, a_eventSource);
	}

    EventResult on_animation_event::ProcessEvent_PC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		ProcessEvent(a_sink, a_event, a_eventSource);
		return _ProcessEvent_PC(a_sink, a_event, a_eventSource);
	}

}