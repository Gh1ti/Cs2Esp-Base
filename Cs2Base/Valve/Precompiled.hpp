#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string_view>

#include "Offsets.hpp"
#include "Vector.hpp"
#include "Memory.hpp"

namespace interfaces {
	Memory mem = Memory("cs2.exe");
	uintptr_t client = mem.GetModuleAddress("client.dll");
	uintptr_t entity_list = mem.Read<uintptr_t>(client + offsets::client::dwEntityList);
	ViewMatrix_t vm = mem.Read<ViewMatrix_t>(client + offsets::client::dwViewMatrix);
}


#include "CPlayer.hpp"
