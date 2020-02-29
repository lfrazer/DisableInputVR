#include "config.h"

namespace DisableInputVR
{
	int leftHandedMode = 0;

	bool logging = false;

	float touchpadx = 0.60f;

	float touchpady = 0.60f;
		
	int useConfigValue = 1;

	int DisableTurning = 1;

	int PreventActivateMisclick = 1;
	int PreventActivateMisclickTimer = 1500;

	std::vector<int> disabledButtonsLeftController;
	std::vector<int> disabledButtonsRightController;

	//Config file is read in this function and variables are filled
	void ReadConfigFile()
	{
		std::string	runtimeDirectory = GetRuntimeDirectory();
		if (!runtimeDirectory.empty())
		{
			std::string configPath = runtimeDirectory + "Data\\SKSE\\Plugins\\disableinputvr_plugin.ini";

			std::ifstream file(configPath);
			std::string line;
			std::string currentSetting;
			while (std::getline(file, line))
			{
				trim(line);
				skipComments(line);
				trim(line);
				if (line.length() > 0)
				{
					if (line.substr(0, 1) == "[")
					{
						//newsetting
						currentSetting = line;
						std::transform(currentSetting.begin(), currentSetting.end(), currentSetting.begin(), ::tolower);
					}
					else
					{
						std::string variableName;
						std::string variableValue = GetConfigSettingsValue(line, variableName);
						
						if (variableName == "TouchPadX")
							touchpadx = ::atof(variableValue.c_str());
						else if (variableName == "TouchPadY")
							touchpady = ::atof(variableValue.c_str());
						else if (variableName == "LeftHandedMode")
							leftHandedMode = std::stoi(variableValue);
						else if (variableName == "UseConfigValueInstead")
							useConfigValue = std::stoi(variableValue);
						else if (variableName == "DisableTurning")
							DisableTurning = std::stoi(variableValue);		
						else if (variableName == "PreventActivateMisclick")
							PreventActivateMisclick = std::stoi(variableValue);		
						else if (variableName == "PreventActivateMisclickTimer")
							PreventActivateMisclickTimer = std::stoi(variableValue);
						else if (variableName == "Logging")
						{
							if (std::stoi(variableValue) > 0)
							{
								logging = true;
								Log("Logging is enabled.");
							}
						}
						else if (variableName == "DisableButtonsLeftController")
						{
							variableValue = removeWhiteSpaces(variableValue);

							std::vector<std::string> splittedByPlugin = split(variableValue, ',');

							for each(std::string var in splittedByPlugin)
							{
								int val = std::stoi(var);

								disabledButtonsLeftController.emplace_back(val);
							}
						}
						else if(variableName == "DisableButtonsRightController")
						{
							variableValue = removeWhiteSpaces(variableValue);

							std::vector<std::string> splittedByPlugin = split(variableValue, ',');

							for each(std::string var in splittedByPlugin)
							{
								int val = std::stoi(var);

								disabledButtonsRightController.emplace_back(val);
							}
						}
					}
				}
			}
			Log("Config read complete.");
		}
	}

	void Log(std::string message)
	{
		if (logging)
		{
			_MESSAGE(message.c_str());
		}
	}

	void Logf(std::string message, float f)
	{
		if (logging)
		{
			_MESSAGE(message.c_str(), f);
		}
	}

	void Logl(std::string message, long l)
	{
		if (logging)
		{
			_MESSAGE(message.c_str(), l);
		}
	}

	void Logi(std::string message, int i)
	{
		if (logging)
		{
			_MESSAGE(message.c_str(), i);
		}
	}

	void Logs(std::string message, std::string str)
	{
		if (logging)
		{
			_MESSAGE(message.c_str(), str.c_str());
		}
	}

	void Logsff(std::string message, std::string str, float f1, float f2)
	{
		if (logging)
		{
			_MESSAGE(message.c_str(), str.c_str(),f1,f2);
		}
	}

	void GetSettings()
	{
		Setting* setting = GetINISetting("fTouchpadCenterButtonPercent:VRInput");
		if (!setting || setting->GetType() != Setting::kType_Float)
			Log("Failed to get fTouchpadCenterButtonPercent from INI.");
		else
		{
			if (useConfigValue == 1)
			{
				touchpadx = setting->data.f32;
				touchpady = setting->data.f32;
			}
		}
			
	}
}