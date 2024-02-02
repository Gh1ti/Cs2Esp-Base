#pragma once
#include "Precompiled.hpp"

class CPlayer {
public:
	CPlayer(int index){
		this->index = index;
		list_entry = interfaces::mem.Read<uintptr_t>(interfaces::entity_list + (8 * (index & 0x7FFF) >> 9) + 16);
		player = interfaces::mem.Read<uintptr_t>(list_entry + 120 * (index & 0x1FF));
		playerPawn = interfaces::mem.Read<uintptr_t>(player + offsets::client::m_hPlayerPawn);
		list_entry2 = interfaces::mem.Read<uintptr_t>(interfaces::entity_list + (0x8 * (index & 0x7FFF) >> 9) + 16);
		pCSPlayerPawn = interfaces::mem.Read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
	}
	int GetIndex() {
		return this->index;
	}
	int GetTeam() {
		return interfaces::mem.Read<int>(player + offsets::client::m_iTeamNum);
	}
	int GetHealth() {
		return interfaces::mem.Read<int>(pCSPlayerPawn + offsets::client::m_iHealth);
	}
	bool IsValid() {
		return player != 0 && playerPawn != 0 && pCSPlayerPawn != 0;
	}
	Vector_t GetOrigin() {
		return interfaces::mem.Read<Vector_t>(pCSPlayerPawn + offsets::client::m_vOldOrigin);
	}
	Vector_t BoneOrigin(int index) {
		uintptr_t gamescene = interfaces::mem.Read<uintptr_t>(pCSPlayerPawn + offsets::client::m_pGameSceneNode);
		uintptr_t bonearray = interfaces::mem.Read<uintptr_t>(gamescene + offsets::client::m_modelState + offsets::client::m_boneArray);
		Vector_t position = interfaces::mem.Read<Vector_t>(bonearray + index * 32);
		return position;
	}
	uintptr_t Entity() {
		return this->player;
	}
	uintptr_t SpotedState() {
		return interfaces::mem.Read<uintptr_t>(pCSPlayerPawn + offsets::client::m_entitySpottedState + offsets::client::m_bSpottedByMask);
	}
private:
	uintptr_t list_entry;
	uintptr_t list_entry2;
	uintptr_t player;
	uintptr_t playerPawn;
	uintptr_t pCSPlayerPawn;
	int index;
};

class LocalPlayer {
public:
	LocalPlayer(){
		player = interfaces::mem.Read<uintptr_t>(interfaces::client + offsets::client::dwLocalPlayerController);
		playerPawn = interfaces::mem.Read<uintptr_t>(player + offsets::client::m_hPlayerPawn);
		uintptr_t list_entry2 = interfaces::mem.Read<uintptr_t>(interfaces::entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		pCSPlayerPawn = interfaces::mem.Read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
	}
	int GetTeam() {
		return interfaces::mem.Read<int>(player + offsets::client::m_iTeamNum);
	}
	int GetHealth() {
	    return interfaces::mem.Read<int>(pCSPlayerPawn + offsets::client::m_iHealth);
	}

	uintptr_t Entity() {
		return this->player;
	}
	uintptr_t SpotedState() {
		return interfaces::mem.Read<uintptr_t>(pCSPlayerPawn + offsets::client::m_entitySpottedState + offsets::client::m_bSpottedByMask);
	}
private:
	uintptr_t player;
	uintptr_t playerPawn;
	uintptr_t pCSPlayerPawn;
};