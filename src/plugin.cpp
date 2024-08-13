#include "log.h"
#include "hook.h"

void OnDataLoaded()
{
   
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		break;
	case SKSE::MessagingInterface::kPostLoad:
		//hooks::util::install();
		break;
	case SKSE::MessagingInterface::kPostPostLoad:
		//hooks::util::install();
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		hooks::util::install();
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