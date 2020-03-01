#include "DisableInputVR.h"
#include <iostream>
#include <string>
#include <fstream>

namespace DisableInputVR {
	//Openvr system variable
	vr::IVRSystem *ivrSystem = nullptr;

	//SkyrimVRTools hookManagerApi variable
	OpenVRHookManagerAPI* hookManager;
	
	float lasttouchpadx = 0;
	float lasttouchpady = 0;
	
	//Default left-right controller variables. For left handed support to be changed later if needed.
	vr::ETrackedControllerRole leftControllerRole = vr::ETrackedControllerRole::TrackedControllerRole_LeftHand;
	vr::ETrackedControllerRole rightControllerRole = vr::ETrackedControllerRole::TrackedControllerRole_RightHand;
	
	//This function is used to initialize the mod.
	void StartMod()
	{
		ReadConfigFile();

		GetSettings();
		
		if (leftHandedMode != 0) //This is the left handed mode check. This section reverses the default values of these variables.
		{
			leftControllerRole = vr::ETrackedControllerRole::TrackedControllerRole_RightHand;
			rightControllerRole = vr::ETrackedControllerRole::TrackedControllerRole_LeftHand;
		}

		RegisterCallback();	
	}

	//Initializes openvr system. Required for haptic triggers.
	void InitSystem(vr::IVRSystem* vrSystem)
	{
		if ((ivrSystem = vrSystem))
			Log("VR System is alive.");
		else
			Log("No VR System found.");
	}

	
	//This is the function to register SkyrimVRTools callback
	void RegisterCallback()
	{		
		if (hookManager->IsInitialized())
		{
			hookManager = RequestOpenVRHookManagerObject();
			if (hookManager)
			{
				Log("Successfully requested OpenVRHookManagerAPI.");
				InitSystem(hookManager->GetVRSystem());

				hookManager->RegisterControllerStateCB(cbFunc);
			}
		}
	}

	//SkyrimVRTools input blocking Callback function. Inside we disable the input we want. In this case input that turns you left or right.
	bool cbFunc(vr::TrackedDeviceIndex_t unControllerDeviceIndex, const vr::VRControllerState_t* pControllerState, uint32_t unControllerStateSize, vr::VRControllerState_t* pOutputControllerState)
	{
		const int JOYSTICK_AXIS = 0; // TODO: will be 3 for SKSEVR 2.00.11 in input actions mode

		if (pControllerState)
		{
			if (!isGameStopped())
			{			
				if (ivrSystem)
				{
					vr::TrackedDeviceIndex_t rightController = hookManager->GetVRSystem()->GetTrackedDeviceIndexForControllerRole(rightControllerRole);
					vr::TrackedDeviceIndex_t leftController = hookManager->GetVRSystem()->GetTrackedDeviceIndexForControllerRole(leftControllerRole);

					if (rightController == unControllerDeviceIndex)
					{
						if (DisableTurning)
						{
							// joystick turning disabled = force to 0
							pOutputControllerState->rAxis[JOYSTICK_AXIS].x = 0.0f;
						}

						// disable joystick jump if set
						if (DisableJoystickJump)
						{
							if (pControllerState->rAxis[JOYSTICK_AXIS].y > 0.0f)
							{
								pOutputControllerState->rAxis[JOYSTICK_AXIS].y = 0.0f;
							}
						}

						if (pControllerState->ulButtonPressed != 0)
						{
							Logsff("ulButtonPressed: %16x x: %g  y: %g", num2hex(pControllerState->ulButtonPressed), lasttouchpadx, lasttouchpady);

							if (DisableTurning)
							{
								if (lasttouchpadx < -1 * touchpadx || lasttouchpadx > touchpadx || lasttouchpady > touchpady || lasttouchpady < -1 * touchpady)
								{
									if (pControllerState->ulButtonPressed & 0x100000000)
										pOutputControllerState->ulButtonPressed &= ~0x100000000;
								}
							}
							
							if (PreventActivateMisclick && ActivateOpenWithCountDown)
							{
								if (lasttouchpadx >= -1 * touchpadx || lasttouchpadx <= touchpadx || lasttouchpady <= touchpady || lasttouchpady >= -1 * touchpady)
								{
									if (pControllerState->ulButtonPressed & 0x100000000)
										pOutputControllerState->ulButtonPressed &= ~0x100000000;
								}
							}
							
							//Here we disable other buttons set by user
							for (int i = 0; i < disabledButtonsRightController.size(); i++)
							{
								uint64_t buttonMask = GetButtonMaskFromId(disabledButtonsRightController[i]);
								if (pControllerState->ulButtonPressed & buttonMask)
									pOutputControllerState->ulButtonPressed &= ~buttonMask;
							}
						}
					}
					else if (leftController == unControllerDeviceIndex)
					{
						//Here we disable other buttons set by user
						for (int i = 0; i < disabledButtonsLeftController.size(); i++)
						{
							uint64_t buttonMask = GetButtonMaskFromId(disabledButtonsLeftController[i]);
							if (pControllerState->ulButtonPressed & buttonMask)
								pOutputControllerState->ulButtonPressed &= ~buttonMask;
						}
					}
				}
			}
		}

		return true;
	}

	InputEventHandler myInputEventHandler;
	InputEventDispatcher* g_InputEventDispatcher;
	
	EventResult InputEventHandler::ReceiveEvent(InputEvent ** evn, InputEventDispatcher * dispatcher)
	{
		//	00	ButtonEvent
		//	01	MouseMoveEvent
		//	02	CharEvent
		//	03	ThumbstickEvent
		//	04	DeviceConnectEvent
		//	05	KinectEvent
		//	06	VrWandTouchpadPositionEvent
		//	07	VrWandTouchpadSwipeEvent
		
		if (evn != nullptr)
		{
			if (evn[0] != nullptr)
			{
				if (evn[0]->IsIDEvent())
				{
					if (evn[0]->GetControlID())
					{						
						if (evn[0]->GetControlID()->data)
						{
							if (strlen(evn[0]->GetControlID()->data) == 0 && evn[0]->deviceType == 3 && evn[0]->eventType == 6)
							{
								VrWandTouchpadPositionEvent* ts = (VrWandTouchpadPositionEvent*)evn[0];

								if (ts)
								{
									lasttouchpadx = ts->x;
									lasttouchpady = ts->y;
								}
							}
						}
					}
				}
			}
		}

		return EventResult::kEvent_Continue;
	}
}
