#ifndef MENUCHECKER_H
#define MENUCHECKER_H

#pragma once
#include "skse64/GameReferences.h"

#include "skse64/PapyrusVM.h"
#include <unordered_map>
#include <thread>

#include "config.h"

namespace DisableInputVR
{
	extern std::vector<std::string> gameStoppingMenus;

	extern std::unordered_map<std::string, bool> menuTypes;

	bool isGameStopped();

	bool isActivateOpen();
	
	void ActivateMenuAction();

	void ActivateMenuCountdown();

	extern bool ActivateOpenWithCountDown;

	class AllMenuEventHandler : public BSTEventSink <MenuOpenCloseEvent> {
	public:
		virtual EventResult	ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent> * dispatcher);
	};

	extern AllMenuEventHandler menuEvent;
}

#endif