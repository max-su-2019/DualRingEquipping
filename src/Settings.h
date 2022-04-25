#pragma once

namespace DualRingEquipping
{
	class INIData
	{
		INIData();

		INIData(const INIData&) = delete;

		INIData(INIData&&) = delete;

		INIData& operator=(const INIData&) = delete;

		INIData& operator=(INIData&&) = delete;

		RE::BipedObjectSlot leftRingSlot;  //Check the slot masks hex value here: https://www.creationkit.com/index.php?title=Slot_Masks_-_Armor

	public:
		static INIData* GetSingleton()
		{
			static INIData singleton;
			return std::addressof(singleton);
		}

		RE::BipedObjectSlot GetLeftRingSlot() const { return leftRingSlot; };
	};
}
