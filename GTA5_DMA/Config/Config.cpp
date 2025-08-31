#include "../pch.h"

#include "Config.h"

#include "json.hpp"

#include <fstream>

#include "Features/Features.h"

bool Config::SaveConfig()
{
	std::ofstream ConfigFile("config.json", std::ios_base::out);

	if (!ConfigFile.is_open())
	{
		std::println("[!] Failed To Open Config File For Saving!");
		return 0;
	}

	using json = nlohmann::json;

	json Config;

	Config["Unsorted"] = { {"m_BlipInspector", BlipInspector::m_BlipInspector} };

	Config["Weapon"] = { {"m_WeaponEditor", WeaponEditor::m_WeaponEditor} };
	Config["Weapon"] += {"m_WeaponInspector", WeaponInspector::m_WeaponInspector};
	Config["Weapon"] += {"m_InfiniteAmmo", InfiniteAmmo::m_InfiniteAmmo};

	Config["Self"] = { {"m_GodMode",GodMode::m_GodMode} };
	Config["Self"] += {"m_NeverWanted", NeverWanted::m_NeverWanted};
	Config["Self"] += {"m_RefreshHealth", RefreshHealth::m_RefreshHealth};
	Config["Self"] += {"m_HealthThreshold", RefreshHealth::m_HealthThreshold};

	Config["Vehicle"] = { {"m_VehicleEditor", VehicleEditor::m_VehicleEditor} };
	Config["Vehicle"] += {"m_VehicleInspector", VehicleInspector::m_VehicleInspector};
	Config["Vehicle"] += { "m_VehicleGodMode", VehicleGodMode::m_VehicleGodMode };
	Config["Vehicle"] += { "m_RainbowCar", RainbowCar::m_RainbowCar };

	Config["Fuser"] = { {"m_Fuser",Fuser::m_Fuser} };
	Config["Fuser"] += {"m_Width", Fuser::m_Width };
	Config["Fuser"] += {"m_Height", Fuser::m_Height };
	Config["Fuser"] += {"m_MonitorIndex", Fuser::m_MonitorIndex };

	ConfigFile << Config.dump(2);

	ConfigFile.close();

	return 1;
}

bool Config::LoadConfig()
{
	std::ifstream ConfigFile("config.json", std::ios_base::in | std::ios_base::ate);

	if (!ConfigFile.is_open())
	{
		std::println("[!] Failed To Open Config File For Loading!");
		return 0;
	}

	std::streamsize ConfigFileSize = ConfigFile.tellg();

	ConfigFile.seekg(std::ios::beg);

	auto ConfigFileBuffer = std::make_unique<char[]>(ConfigFileSize + 0x1);

	ConfigFile.read(ConfigFileBuffer.get(), ConfigFileSize);

	using json = nlohmann::json;

	try
	{
		json Config = json::parse(ConfigFileBuffer.get());

		/* Unsorted */
		if (Config["Unsorted"].count("m_BlipInspector"))
			BlipInspector::m_BlipInspector = Config["Unsorted"]["m_BlipInspector"].get<bool>();

		/* Weapon */
		if (Config["Weapon"].count("m_WeaponEditor"))
			WeaponEditor::m_WeaponEditor = Config["Weapon"]["m_WeaponEditor"].get<bool>();

		if (Config["Weapon"].count("m_WeaponInspector"))
			WeaponInspector::m_WeaponInspector = Config["Weapon"]["m_WeaponInspector"].get<bool>();

		if (Config["Weapon"].count("m_InfiniteAmmo"))
			InfiniteAmmo::m_InfiniteAmmo = Config["Weapon"]["m_InfiniteAmmo"].get<bool>();

		/* Self */
		if (Config["Self"].count("m_GodMode"))
			GodMode::m_GodMode = Config["Self"]["m_GodMode"].get<bool>();

		if (Config["Self"].count("m_NeverWanted"))
			NeverWanted::m_NeverWanted = Config["Self"]["m_NeverWanted"].get<bool>();

		if (Config["Self"].count("m_RefreshHealth"))
			RefreshHealth::m_RefreshHealth = Config["Self"]["m_RefreshHealth"].get<bool>();

		if (Config["Self"].count("m_HealthThreshold"))
			RefreshHealth::m_HealthThreshold = Config["Self"]["m_HealthThreshold"].get<float>();

		/* Vehicle */
		if (Config["Vehicle"].count("m_VehicleGodMode"))
			VehicleGodMode::m_VehicleGodMode = Config["Vehicle"]["m_VehicleGodMode"].get<bool>();

		if (Config["Vehicle"].count("m_RainbowCar"))
			RainbowCar::m_RainbowCar = Config["Vehicle"]["m_RainbowCar"].get<bool>();

		if (Config["Vehicle"].count("m_VehicleEditor"))
			VehicleEditor::m_VehicleEditor = Config["Vehicle"]["m_VehicleEditor"].get<bool>();

		if (Config["Vehicle"].count("m_VehicleInspector"))
			VehicleInspector::m_VehicleInspector = Config["Vehicle"]["m_VehicleInspector"].get<bool>();

		/* Fuser */
		if (Config["Fuser"].count("m_Fuser"))
			Fuser::m_Fuser = Config["Fuser"]["m_Fuser"].get<bool>();

		if (Config["Fuser"].count("m_Width"))
			Fuser::m_Width = Config["Fuser"]["m_Width"].get<int>();

		if (Config["Fuser"].count("m_Height"))
			Fuser::m_Height = Config["Fuser"]["m_Height"].get<int>();

		if (Config["Fuser"].count("m_MonitorIndex"))
			Fuser::m_MonitorIndex = Config["Fuser"]["m_MonitorIndex"].get<int>();
	}
	catch (const json::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 1;
}