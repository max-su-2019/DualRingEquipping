#include "Settings.h"
#include "Hook.h"
#include "SimpleIni.h"

#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\DualRingEquipping.ini"

namespace DualRingEquipping
{
	INIData::INIData()
	{
		CSimpleIniA ini;
		ini.LoadFile(SETTINGFILE_PATH);
		leftRingSlot = RE::BipedObjectSlot(ini.GetDoubleValue("Main", "iLeftRingSlot", 0x00010000));  //Check the slot masks hex value here: https://www.creationkit.com/index.php?title=Slot_Masks_-_Armor
		 
		InitWornArmorHook::InstallHook();
		UnequipWornArmorHook::InstallHook();
	}
}
