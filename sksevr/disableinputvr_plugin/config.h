#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <fstream>
#include <sstream>
#include "skse64\GameSettings.h"


#include "Utility.hpp"

namespace DisableInputVR
{
	extern int leftHandedMode;

	extern float touchpadx;

	extern float touchpady;

	extern bool logging;

	extern int DisableTurning;
	extern int DisableJoystickJump;

	extern int PreventActivateMisclick;
	extern int PreventActivateMisclickTimer;

	extern std::vector<int> disabledButtonsLeftController;
	extern std::vector<int> disabledButtonsRightController;

	extern int useConfigValue;

	void ReadConfigFile();

	void GetSettings();

	void Log(std::string message);
	void Logf(std::string message, float f);
	void Logl(std::string message, long l);
	void Logi(std::string message, int i);
	void Logs(std::string message, std::string str);
	void Logsff(std::string message, std::string str, float f1, float f2);
}

#endif
////