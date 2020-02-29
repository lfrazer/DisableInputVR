#ifndef DISABLEINPUTVR_H
#define DISABLEINPUTVR_H

#pragma once
#include "skse64/PapyrusNativeFunctions.h"

#include "api/PapyrusVRAPI.h"
#include "api/VRManagerAPI.h"
#include "skse64\PapyrusEvents.h"
#include "skse64\PapyrusSpell.h"
#include "skse64\PapyrusForm.h"
#include "skse64/GameMenus.h"
#include "skse64/GameRTTI.h"
#include "skse64/GameForms.h"
#include "skse64/GameData.h"
#include "skse64\GameFormComponents.h"
#include "skse64\PapyrusActorValueInfo.h"

#include <thread>
#include <atomic>
#include <openvr.h>
#include "hooks\openvr_hook.h"

#include "MenuChecker.h"
namespace DisableInputVR {

	const std::string MOD_VERSION = "1.0.0";

	extern OpenVRHookManagerAPI* hookManager;

	extern float lasttouchpadx;
	extern float lasttouchpady;

	extern vr::ETrackedControllerRole leftControllerRole;
	extern vr::ETrackedControllerRole rightControllerRole;
	
	void StartMod();

	void InitSystem(vr::IVRSystem* vrSystem);

	void RegisterCallback();

	class InputEventHandler : public BSTEventSink <InputEvent>
	{
	public:
		virtual	EventResult ReceiveEvent(InputEvent ** evn, InputEventDispatcher * dispatcher);
	};

	extern InputEventHandler myInputEventHandler;
	extern InputEventDispatcher* g_InputEventDispatcher;


	class VrWandTouchpadPositionEvent : public IDEvent, public InputEvent
	{
	public:
		virtual					~VrWandTouchpadPositionEvent();
		virtual bool			IsIDEvent();
		virtual BSFixedString *	GetControlID();

		UInt32	unk1;
		UInt32	unk2;
		UInt32	unk3;
		UInt32	unk4;
		float	x;
		float	y;
	};
	

	bool cbFunc(vr::TrackedDeviceIndex_t unControllerDeviceIndex, const vr::VRControllerState_t* pControllerState, uint32_t unControllerStateSize, vr::VRControllerState_t* pOutputControllerState);
}

#endif