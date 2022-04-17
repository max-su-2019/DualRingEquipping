#include "Hook.h"

namespace DualRingEquipping
{
	static std::int32_t GetWornRingCount(RE::Actor* a_actor)
	{
		std::int32_t count = 0;

		if (!a_actor)
			return count;

		for (auto& pair : a_actor->GetInventory()) {
			if (pair.first && pair.first->As<RE::TESObjectARMO>() && pair.first->As<RE::TESObjectARMO>()->GetSlotMask().any(RE::BipedObjectSlot::kRing) && pair.second.second->IsWorn())
				count += 1;
		}

		return count;
	}

	void InitWornArmorHook::InitWornArmor(RE::TESObjectARMO* a_armor, RE::TESRace* a_race, RE::BSTSmartPointer<RE::BipedAnim>* a_biped, RE::SEX a_sex)
	{
		logger::debug("Init Worn Armor Hook Start!");

		if (a_biped && a_armor && a_armor->GetSlotMask().any(RE::BipedObjectSlot::kRing)) {
			auto actor = a_biped->get()->actorRef.get() ? a_biped->get()->actorRef.get()->As<RE::Actor>() : nullptr;
			auto wornRingCount = GetWornRingCount(actor);
			switch (wornRingCount) {
			case 1:
				InitSingleRingAddon(a_biped, actor, a_sex);
				break;

			case 2:
				InitDualRingAddon(a_biped, actor, a_sex);
				break;

			default:
				break;
			}

			return;
		}

		return _InitWornArmor(a_armor, a_race, a_biped, a_sex);
	}

	void InitWornArmorHook::InitSingleRingAddon(RE::BSTSmartPointer<RE::BipedAnim>* a_biped, RE::Actor* a_actor, RE::SEX a_sex)
	{
		if (!a_actor || !a_biped)
			return;

		auto currentRing = a_actor->GetInventoryChanges()->GetArmorInSlot(RE::InventoryChanges::ArmorSlot::kRing);
		if (currentRing) {
			for (auto& pair : a_actor->GetInventory()) {
				if (pair.first && pair.first->As<RE::TESObjectARMO>() && pair.first->As<RE::TESObjectARMO>()->GetSlotMask().any(RE::BipedObjectSlot::kRing) && pair.second.second->IsWorn()) {
					auto addon = pair.first->As<RE::TESObjectARMO>()->GetArmorAddonInSlot(RE::BipedObjectSlot::kRing);
					if (addon)
						addon->InitWornArmorAddon(currentRing, a_biped, a_sex);
				}
			}
		}
	}

	void InitWornArmorHook::InitDualRingAddon(RE::BSTSmartPointer<RE::BipedAnim>* a_biped, RE::Actor* a_actor, RE::SEX a_sex)
	{
		if (!a_actor || !a_biped)
			return;

		auto currentRing = a_actor->GetInventoryChanges()->GetArmorInSlot(RE::InventoryChanges::ArmorSlot::kRing);
		if (currentRing) {
			for (auto& pair : a_actor->GetInventory()) {
				if (pair.first && pair.first->As<RE::TESObjectARMO>() && pair.first->As<RE::TESObjectARMO>()->GetSlotMask().any(RE::BipedObjectSlot::kRing) && pair.second.second->IsWorn()) {
					RE::BipedObjectSlot slot = pair.second.second->IsWornLeft() ? RE::BipedObjectSlot::kRing : RE::BipedObjectSlot::kFX01;
					auto addon = pair.first->As<RE::TESObjectARMO>()->GetArmorAddonInSlot(slot);
					if (addon) {
						addon->InitWornArmorAddon(currentRing, a_biped, a_sex);
						logger::debug("Init  {}, in Slot {}", pair.first->As<RE::TESObjectARMO>()->GetName(), slot);
					}
				}
			}
		}
	}

	bool UnequipWornArmorHook::UnequipWornArmor(
		RE::ActorEquipManager* a_equipMgr, RE::Actor* a_actor, RE::TESBoundObject* a_object,
		RE::ExtraDataList* a_extraData, std::uint32_t a_count, const RE::BGSEquipSlot* a_slot,
		bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow, const RE::BGSEquipSlot* a_slotToReplace)
	{
		logger::debug("UnequipWornArmor Hook Start!");

		logger::debug("Object Name is {}, ID is {:x}", a_object->GetName(), a_object->GetFormID());

		if (a_object->IsArmor() && a_object->As<RE::TESObjectARMO>() && a_object->As<RE::TESObjectARMO>()->GetSlotMask().any(RE::BipedObjectSlot::kRing)) {
			auto wornRingCount = GetWornRingCount(a_actor);
			logger::debug("Current Equipped Ring Count is {}", wornRingCount);
			if (wornRingCount == 1 && a_extraData && !a_extraData->GetByType(RE::ExtraDataType::kWornLeft)) {
				a_extraData->RemoveByType(RE::ExtraDataType::kWorn);
				a_extraData->Add(new RE::ExtraWornLeft());
				return false;
			}
		}

		return _UnequipWornArmor(a_equipMgr, a_actor, a_object, a_extraData, a_count, a_slot, a_queueEquip, a_forceEquip, a_playSounds, a_applyNow, a_slotToReplace);
	}
}
