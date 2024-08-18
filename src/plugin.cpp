#include "log.h"
#include "hook.h"

void OnDataLoaded()
{
   
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
	    hooks::on_animation_event::install();
		hooks::util::install();
		break;
	case SKSE::MessagingInterface::kPostLoad:
		
		break;
	case SKSE::MessagingInterface::kPostPostLoad:
		
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
        hooks::on_animation_event::setglobals();
		break;
	case SKSE::MessagingInterface::kNewGame:
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
	SetupLog();


    auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	
    return true;
}