#pragma once
#include "Precompiled.hpp"

namespace offsets {
	namespace client {
		constexpr std::ptrdiff_t dwEntityList = 0x17CE6A0;
		constexpr std::ptrdiff_t dwLocalPlayerController = 0x181DC98;
        constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x16D4F48;
		constexpr std::ptrdiff_t dwViewMatrix = 0x182CEA0;
		constexpr std::ptrdiff_t m_entitySpottedState = 0x1638;
		constexpr std::ptrdiff_t m_bSpottedByMask = 0xC;
		constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;
		constexpr std::ptrdiff_t m_iHealth = 0x32C;
		constexpr std::ptrdiff_t m_iTeamNum = 0x3BF;
		constexpr std::ptrdiff_t m_vOldOrigin = 0x1224;
		constexpr std::ptrdiff_t m_pGameSceneNode = 0x310;
		constexpr std::ptrdiff_t m_modelState = 0x160;
		constexpr std::ptrdiff_t m_boneArray = 0x80;
		constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x750;
	}
}