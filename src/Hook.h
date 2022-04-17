#pragma once

namespace DualRingEquipping
{
	class InitWornArmorHook
	{
	public:
		static void InstallHook()
		{
#if ANNIVERSARY_EDITION

#else
			std::uint32_t base = 24232, offset = 0x302;
#endif
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> InitWornArmorAddonBase{ REL::ID(base) };
			_InitWornArmor = trampoline.write_call<5>(InitWornArmorAddonBase.address() + offset, InitWornArmor);
			logger::debug("Hook InitWornArmor!, Base:{}, Offset:{}", base, offset);
		}

	private:
		static void InitWornArmor(RE::TESObjectARMO* a_armor, RE::TESRace* a_race, RE::BSTSmartPointer<RE::BipedAnim>* a_biped, RE::SEX a_sex);

		static inline REL::Relocation<decltype(InitWornArmor)> _InitWornArmor;

		static void InitSingleRingAddon(RE::BSTSmartPointer<RE::BipedAnim>* a_biped, RE::Actor* a_actor, RE::SEX a_sex);

		static void InitDualRingAddon(RE::BSTSmartPointer<RE::BipedAnim>* a_biped, RE::Actor* a_actor, RE::SEX a_sex);
	};

	class UnequipWornArmorHook
	{
	public:
		static void InstallHook()
		{
#if ANNIVERSARY_EDITION

#else
			std::uint32_t base = 36979, offset = 0x1ce;
#endif
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> UnequipWornArmorBase{ REL::ID(base) };
			_UnequipWornArmor = trampoline.write_call<5>(UnequipWornArmorBase.address() + offset, UnequipWornArmor);
			logger::debug("Hook UnequipWornArmor(!, Base:{}, Offset:{}", base, offset);
		}

	private:
		static auto UnequipWornArmor(
			RE::ActorEquipManager* a_equipMgr, RE::Actor* a_actor, RE::TESBoundObject* a_object,
			RE::ExtraDataList* a_extraData, std::uint32_t a_count, const RE::BGSEquipSlot* a_slot,
			bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow, const RE::BGSEquipSlot* a_slotToReplace) -> bool;

		static inline REL::Relocation<decltype(UnequipWornArmor)> _UnequipWornArmor;
	};
}
