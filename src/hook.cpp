#include "hook.h"

namespace hooks
{
    RE::TESForm;
    void on_animation_event::GetEquippedShout(RE::Actor *actor, bool SpellFire){
        auto limboshout = actor->GetActorRuntimeData().selectedPower;

        if (limboshout && limboshout->Is(RE::FormType::Shout)){
            auto data = RE::TESDataHandler::GetSingleton();
            auto Hen = limboshout->As<RE::TESShout>()->variations->spell->GetKeywords();

            for (RE::BGSKeyword* Yen : Hen){
                std::string_view Lsht = (clib_util::editorID::get_editorID(Yen)).data();
                switch (hash(Lsht.data(), Lsht.size())){
                case "DSV_UnrelentingForce"_h:
                    if (SpellFire) {
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x802, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x800, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_Dismay"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x805, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x804, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_IceForm"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x807, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x806, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_Disarm"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x809, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x808, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_FrostBreath"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x80B, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x80A, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_PhantomForm"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x80D, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x80C, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_ElementalFury"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x80F, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x80E, "DraugrShoutVoicing.esp")));
                    }
                    break;

                case "DSV_BecomeEthereal"_h:
                    if (SpellFire){
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x811, "DraugrShoutVoicing.esp")));
                    }else{
                        util::playSound(actor, (data->LookupForm<RE::BGSSoundDescriptorForm>(0x810, "DraugrShoutVoicing.esp")));
                    }
                    break;

                default:

                    break;
                }
            }
        }
    }


	void on_animation_event::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		if (!a_event->holder) {
			return;
		}
		std::string_view eventTag = a_event->tag.data();
		RE::Actor* actor = const_cast<RE::TESObjectREFR*>(a_event->holder)->As<RE::Actor>();
        switch (hash(eventTag.data(), eventTag.size())) {
        case "BeginCastVoice"_h:

            //logger::info("anim_event BeginCastVoice");

            GetEquippedShout(actor);

            break;

        case "Voice_SpellFire_Event"_h:

            //logger::info("anim_event Voice_SpellFire_Event");

            GetEquippedShout(actor, true);

            break;
		
		}
	}

	EventResult on_animation_event::ProcessEvent_NPC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		ProcessEvent(a_sink, a_event, a_eventSource);
		return _ProcessEvent_NPC(a_sink, a_event, a_eventSource);
	}

	// EventResult on_animation_event::ProcessEvent_PC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	// {
	// 	ProcessEvent(a_sink, a_event, a_eventSource);
	// 	return _ProcessEvent_PC(a_sink, a_event, a_eventSource);
	// }

}

// const auto caster = a_defender->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
// float a_reprisal = (EldenParry::GetSingleton()->AttackerBeatsParry(a_aggressor, a_defender));
// auto bHasEldenParryPerk2 = a_defender->HasPerk(RE::BGSPerk::LookupByEditorID("ORD_Bck20_TimedBlock_Perk_50_OrdASISExclude")->As<RE::BGSPerk>());
// auto bHasEldenParryPerk1 = a_defender->HasPerk(RE::BGSPerk::LookupByEditorID("ORD_Bck20_TimedBlock_Perk_20_OrdASISExclude")->As<RE::BGSPerk>());
// if (bHasEldenParryPerk2 || bHasEldenParryPerk1)
// {
//     RE::MagicItem *eldenArmorSpell = nullptr;
//     if (bHasEldenParryPerk2 == true)
//     {
//         eldenArmorSpell = RE::TESForm::LookupByEditorID<RE::MagicItem>("ORD_Bck_TimedBlock_Spell_Proc_2");
//     }
//     else if (bHasEldenParryPerk1)
//     {
//         eldenArmorSpell = RE::TESForm::LookupByEditorID<RE::MagicItem>("ORD_Bck_TimedBlock_Spell_Proc");
//     }
//     caster->CastSpellImmediate(eldenArmorSpell, true, a_defender, 1, false, 45, a_defender);
// }

// const auto ActorTypeDragon = RE::BGSKeyword::LookupByEditorID("ActorTypeDragon")->As<RE::BGSKeyword>();
// auto UDDragonAISpell = RE::TESForm::LookupByEditorID<RE::SpellItem>("aaaUDDragonAISpell");

// bool IsActorTypeDragon = Dragon->HasKeyword(ActorTypeDragon);
// bool HasUDDragonAISpell = Dragon->HasSpell(UDDragonAISpell);

// uti/l::playSound(actor, RE::TESForm::LookupByEditorID<RE::TESForm>

//Actors\\Character