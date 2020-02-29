#include "MenuChecker.h"

namespace DisableInputVR
{
	bool ActivateOpenWithCountDown = false;

	std::vector<std::string> activateMenu{
		"WSActivateRollover"
	};

	std::vector<std::string> gameStoppingMenus {
		"BarterMenu",
		"Book Menu",
		"Console",
		"Native UI Menu",
		"ContainerMenu",
		"Dialogue Menu",
		"Crafting Menu",
		"Credits Menu",
		"Cursor Menu",
		"Debug Text Menu",
		"FavoritesMenu",
		"GiftMenu",
		"InventoryMenu",
		"Journal Menu",
		"Kinect Menu",
		"Loading Menu",
		"Lockpicking Menu",
		"MagicMenu",
		"Main Menu",
		"MapMarkerText3D",
		"MapMenu",
		"MessageBoxMenu",
		"Mist Menu",
		"Quantity Menu",
		"RaceSex Menu",
		"Sleep/Wait Menu",
		"StatsMenuSkillRing",
		"StatsMenuPerks",
		"Training Menu",
		"Tutorial Menu",
		"TweenMenu"
	};

	std::unordered_map<std::string, bool> menuTypes({
		{ "BarterMenu", false },
		{ "Book Menu", false },
		{ "Console", false },
		{ "Native UI Menu", false },
		{ "ContainerMenu", false },
		{ "Dialogue Menu", false },
		{ "Crafting Menu", false },
		{ "Credits Menu", false },
		{ "Cursor Menu", false },
		{ "Debug Text Menu", false },
		{ "Fader Menu", false },
		{ "FavoritesMenu", false },
		{ "GiftMenu", false },
		{ "HUD Menu", false },
		{ "InventoryMenu", false },
		{ "Journal Menu", false },
		{ "Kinect Menu", false },
		{ "Loading Menu", false },
		{ "Lockpicking Menu", false },
		{ "MagicMenu", false },
		{ "Main Menu", false },
		{ "MapMarkerText3D", false },
		{ "MapMenu", false },
		{ "MessageBoxMenu", false },
		{ "Mist Menu", false },
		{ "Overlay Interaction Menu", false },
		{ "Overlay Menu", false },
		{ "Quantity Menu", false },
		{ "RaceSex Menu", false },
		{ "Sleep/Wait Menu", false },
		{ "StatsMenu", false },
		{ "StatsMenuPerks", false },
		{ "StatsMenuSkillRing", false },
		{ "TitleSequence Menu", false },
		{ "Top Menu", false },
		{ "Training Menu", false },
		{ "Tutorial Menu", false },
		{ "TweenMenu", false },
		{ "WSEnemyMeters", false },
		{ "WSDebugOverlay", false },
		{ "WSActivateRollover", false },
		{ "LoadWaitSpinner", false }
	});

	//Menu open event functions
	AllMenuEventHandler menuEvent;

	EventResult AllMenuEventHandler::ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent> * dispatcher)
	{
		const char * menuName = evn->menuName.data;

		if (evn->opening) //Menu opened
		{			
			if (menuTypes.find(menuName) != menuTypes.end())
			{
				menuTypes[menuName] = true;
			}
			else
			{
				Logs("Opened Menu doesn't exist in our list: %s", menuName);
			}			
		}
		else  //Menu closed
		{
			if (menuTypes.find(menuName) != menuTypes.end())
			{
				menuTypes[menuName] = false;
			}
			else
			{
				Logs("Closed Menu doesn't exist in our list: %s", menuName);
			}
		}

		//Prevent Activate Misclick
		if (!isGameStopped())
		{
			if (PreventActivateMisclick)
			{
				if (strcmp(menuName, "WSActivateRollover") == 0)
				{
					ActivateMenuAction();
				}
			}
		}
		return EventResult::kEvent_Continue;
	}

	bool isGameStopped()
	{
		for (int i = 0; i < gameStoppingMenus.size(); i++) 
		{
			if (menuTypes[gameStoppingMenus[i]] == true)
				return true;
		}
		return false;
	}

	bool isActivateOpen()
	{
		for (int i = 0; i < activateMenu.size(); i++)
		{
			if (menuTypes[activateMenu[i]] == true)
				return true;
		}
		return false;
	}

	void ActivateMenuAction()
	{
		if (!isActivateOpen()) //Activate Popup disappeared
		{
			ActivateOpenWithCountDown = true;
			std::thread t2(ActivateMenuCountdown);
			t2.detach();
		}
		else
		{
			ActivateOpenWithCountDown = false;	
		}
	}

	void ActivateMenuCountdown()
	{
		Sleep(PreventActivateMisclickTimer);

		ActivateOpenWithCountDown = false;
	}

}

