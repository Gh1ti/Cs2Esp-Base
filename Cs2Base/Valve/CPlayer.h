#pragma once
#include "Precompiled.h"

typedef uintptr_t uintptr;

class CPlayer {
public:
	CPlayer(int index, const Memory& mem) : mem(mem) {
		this->index = index;
		client = mem.GetModuleAddress("client.dll");
		uintptr_t entity_list = mem.Read<uintptr_t>(client + offsets::client::dwEntityList);
		list_entry = mem.Read<uintptr_t>(entity_list + (8 * (index & 0x7FFF) >> 9) + 16);
		player = mem.Read<uintptr_t>(list_entry + 120 * (index & 0x1FF));
		playerPawn = mem.Read<uintptr_t>(player + offsets::client::m_hPlayerPawn);
		list_entry2 = mem.Read<uintptr_t>(entity_list + (0x8 * (index & 0x7FFF) >> 9) + 16);
		pCSPlayerPawn = mem.Read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
	}

	int m_Index() {
		return this->index;
	}

	int m_iTeamNum() {
		return mem.Read<int>(player + offsets::client::m_iTeamNum);
	}
	int m_iHealth() {
		return mem.Read<int>(pCSPlayerPawn + offsets::client::m_iHealth);
	}
	bool Valid() {
		return player != 0 && playerPawn != 0 && pCSPlayerPawn != 0;
	}
	Vector_t m_VecOrigin() {
		return mem.Read<Vector_t>(pCSPlayerPawn + offsets::client::m_vOldOrigin);
	}
	Vector_t Bone_Origin(int index) {
		uintptr_t gamescene = mem.Read<uintptr_t>(pCSPlayerPawn + offsets::client::m_pGameSceneNode);
		uintptr_t bonearray = mem.Read<uintptr_t>(gamescene + offsets::client::m_modelState + offsets::client::m_boneArray);
		Vector_t position = mem.Read<Vector_t>(bonearray + index * 32);
		return position;
	}

private:
	Memory mem;
	uintptr_t list_entry;
	uintptr_t list_entry2;
	uintptr_t player;
	uintptr_t playerPawn;
	uintptr_t pCSPlayerPawn;
	uintptr_t client;
	int index;
};

class LocalPlayer {
public:
	LocalPlayer(Memory mem) : mem(mem) {
		client = mem.GetModuleAddress("client.dll");
		player = mem.Read<uintptr_t>(client + offsets::client::dwLocalPlayerController);
		playerPawn = mem.Read<uintptr_t>(player + offsets::client::m_hPlayerPawn);
		uintptr_t entity_list = mem.Read<uintptr_t>(client + offsets::client::dwEntityList);
		uintptr_t list_entry2 = mem.Read<uintptr_t>(entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		pCSPlayerPawn = mem.Read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
	}
	int m_iTeamNum() {
		return mem.Read<int>(player + offsets::client::m_iTeamNum);
	}
	int m_iHealth() {
	    return mem.Read<int>(pCSPlayerPawn + offsets::client::m_iHealth);
	}
private:
	Memory mem;
	uintptr_t client;
	uintptr_t player;
	uintptr_t playerPawn;
	uintptr_t pCSPlayerPawn;
};