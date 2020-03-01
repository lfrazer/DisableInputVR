#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include <shlobj.h>
#include <intrin.h>
#include <string>
#include "skse64/PluginAPI.h"
#include "api/PapyrusVRAPI.h"
#include "api/VRManagerAPI.h"

#include "DisableInputVR.h"

static SKSEMessagingInterface		* g_messaging = NULL;
static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;
static SKSEObjectInterface         * g_object = NULL;

#pragma comment(lib, "Ws2_32.lib")
PapyrusVRAPI* g_papyrusvr;

//In this function we add event sinks for InputEventHandler and MenuEvent
void SetupReceptors()
{
	_MESSAGE("Building Event Sinks...");

	RelocPtr<InputEventDispatcher*> g_sinputEventDispatcher(0x02FEA520);

	DisableInputVR::g_InputEventDispatcher = *(g_sinputEventDispatcher.GetPtr());
	DisableInputVR::g_InputEventDispatcher = InputEventDispatcher::GetSingleton();

	//Add event sinks
	DisableInputVR::g_InputEventDispatcher->AddEventSink(&DisableInputVR::myInputEventHandler);

	MenuManager * menuManager = MenuManager::GetSingleton();
	if (menuManager)
		menuManager->MenuOpenCloseEventDispatcher()->AddEventSink(&DisableInputVR::menuEvent);
}

extern "C" {

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info) {	// Called by SKSE to learn about this plugin and check that it's safe to load it
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim VR\\SKSE\\DisableInputVRPlugin.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
		//gLog.SetLogLevel(IDebugLog::kLevel_FatalError); //Setting this to remove logging. Logging causes performance drops.

		std::string logMsg("DisableInputVR: ");
		logMsg.append(DisableInputVR::MOD_VERSION);
		_MESSAGE(logMsg.c_str());

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "DisableInputVRPlugin";
		info->version = 11000; // 1.1.0

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();
		
		std::string skseVers = "SKSE Version: ";
		skseVers += std::to_string(skse->runtimeVersion);
		_MESSAGE(skseVers.c_str());

		if (skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if (skse->runtimeVersion < CURRENT_RELEASE_RUNTIME)
		{
			_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

			return false;
		}
			

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	//Listener for PapyrusVR Messages
	void OnPapyrusVRMessage(SKSEMessagingInterface::Message* msg)
	{
		if (msg)
		{
			if (msg->type == kPapyrusVR_Message_Init && msg->data)
			{
				_MESSAGE("SkyrimVRTools Init Message recived with valid data, registering for callback");
				g_papyrusvr = (PapyrusVRAPI*)msg->data;

				DisableInputVR::hookManager = g_papyrusvr->GetOpenVRHook();
			}
		}
	}

	//Listener for SKSE Messages
	void OnSKSEMessage(SKSEMessagingInterface::Message* msg)
	{
		if (msg)
		{
			if (msg->type == SKSEMessagingInterface::kMessage_PostLoad)
			{
				_MESSAGE("SKSE PostLoad received, registering for SkyrimVRTools messages");
				//SkyrimVRTools registered here
				g_messaging->RegisterListener(g_pluginHandle, "SkyrimVRTools", OnPapyrusVRMessage);
			}
			else if (msg->type == SKSEMessagingInterface::kMessage_InputLoaded)
				SetupReceptors();
			else if (msg->type == SKSEMessagingInterface::kMessage_DataLoaded)
			{
				DisableInputVR::StartMod();
			}
		}
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse) {	// Called by SKSE to load this plugin
		_MESSAGE("DisableInputVRPluginScript loaded");		
		
		
		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);

		g_messaging = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);
		g_messaging->RegisterListener(g_pluginHandle, "SKSE", OnSKSEMessage);		

		return true;
	}

};