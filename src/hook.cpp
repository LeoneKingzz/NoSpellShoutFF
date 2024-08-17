#include "hook.h"

namespace hooks
{
    void hooks::on_animation_event::setglobals(){
        auto HdSingle = RE::TESDataHandler::GetSingleton();
        auto DS = GetSingleton();
        DS->NSSFFLK_Enable = skyrim_cast<RE::TESGlobal*>(HdSingle->LookupForm(0x800, "No Spell Shout FF.esp"));
    }

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
        case "SneakStart"_h:
            
            DS->NSSFFLK_Enable->value = 0.0f;
            break;

        case "SneakStop"_h:

            DS->NSSFFLK_Enable->value = 1.0f;
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