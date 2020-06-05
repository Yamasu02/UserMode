#pragma once
#include "Structs.h"
#include "Offsets.h"
#include "Memory.h"
#include "MiscVisuals.h"
#include "TraceRay.hpp"
#include "BSPFile.hpp"
#include "BSPParser.hpp"
#include "BSPStructure.hpp"



using namespace std;

DWORD procId = GetProcID(L"csgo.exe");
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
DWORD modulePanorama = GetModuleBA(procId, L"client_panorama.dll");
DWORD moduleEngine = GetModuleBA(procId, L"engine.dll");
DWORD BoneAddress;


float GlowTerroristRed = 1.f;
float GlowTerroristGreen = 0.f;
float GlowTerroristBlue = 0.f;
float GlowTerroristAlpha = 1.f;

float GlowCounterTerroristRed = 0.f;
float GlowCounterTerroristGreen = 0.f;
float GlowCounterTerroristBlue = 1.f;
float GlowCounterTerroristAlpha = 1.f;






DWORD GetPlayerAddress(int index) 
{
	return Read<DWORD>(hProcess, modulePanorama + EntityListOffset + 0X10 * index);
}

bool GetDormant(int i)
{
	return Read<bool>(hProcess, GetPlayerAddress(i) + DormantOffset);
}

uint32_t GetIndex()
{
	return (Read<uint32_t>(hProcess, (Read<DWORD>(hProcess, moduleEngine + ClientStateOffset)) + LocalIndexOffset) + 1);
}

int GetPlayerHealth(int index) 
{
	return Read<int>(hProcess, GetPlayerAddress(index) + HealthOffset);
}

int GetPlayerTeam(int index)
{
	return Read<int>(hProcess, GetPlayerAddress(index) + TeamOffset);
}

bool scope()
{
	return Read<bool>(hProcess, GetPlayerAddress(GetIndex()) + isScoped);
}

vector3 GetPlayerOrigin(int index)
{ 
	return Read <vector3>(hProcess, GetPlayerAddress(index) + VecOriginOffset);
}

vector3 GetPlayerView(int index)
{
	return Read <vector3>(hProcess, GetPlayerAddress(index) + ViewOffset);
}

vector3 GetBonePosEnemy(int index, int boneid)
{
	DWORD addr = Read<DWORD>(hProcess, GetPlayerAddress(index) + BoneMatrixOffset);
	return { Read<float>(hProcess,addr + 0x30 * boneid + 0x0C) , Read<float>(hProcess,addr + 0x30 * boneid + 0x1C) , Read<float>(hProcess,addr + 0x30 * boneid + 0x2C) };
}

bool IsSpotted(int index)
{
	return Read<bool>(hProcess, GetPlayerAddress(index) + SpottedOffset);
}

float GetDistance(vector3* Enemy, vector3* Local)
{
	vector3 DeltaVec = vector3(Enemy->x - Local->x, Enemy->y - Local->y, Enemy->z - Local->z);
	return sqrt(DeltaVec.x * DeltaVec.x + DeltaVec.y * DeltaVec.y + DeltaVec.z * DeltaVec.z);
}

int GetClosestEnemy()
{
	int oof = GetIndex();
	int localteam = GetPlayerTeam(oof);
	float closestDistance = 999999999;
	int closestDistanceIndex;

	for (int i = 0; i < 40; i++)
	{
		bool b1 = GetDormant(i);
		DWORD currentPlayer = GetPlayerAddress(i);
		int currentHelath = GetPlayerHealth(i);


		if (currentPlayer == GetPlayerAddress(oof))
		{
			continue;
		}

		if (!currentPlayer)
		{
			continue;
		}

		if (GetPlayerTeam(i) == localteam)
		{
			continue;
		}

		if (currentHelath < 1 | currentHelath >100)
		{
			continue;
		}


		float currentDistance = GetDistance(&GetPlayerOrigin(i), &GetPlayerOrigin(oof));

		if (currentDistance < 0)
		{
			continue;
		}


		if (currentDistance <= closestDistance)
		{
			closestDistance = currentDistance;
			closestDistanceIndex = i;
		}

	}
	return closestDistanceIndex;
}

void AimAt(vector3* EnemyPositionHead)
{
	int oof = GetIndex();
	DWORD pitchaddr = Read<DWORD>(hProcess, moduleEngine + ClientStateOffset);
	vector3 OriginLocal = GetPlayerOrigin(oof);
	vector3 ViewOffsetLocal = GetPlayerView(oof);
	vector3* PosLocal = &(OriginLocal + ViewOffsetLocal);
	vector3 DeltaVector = { EnemyPositionHead->x - PosLocal->x, EnemyPositionHead->y - PosLocal->y, EnemyPositionHead->z - PosLocal->z };
	float DeltaVectorLength = sqrt(DeltaVector.x * DeltaVector.x + DeltaVector.y * DeltaVector.y + DeltaVector.z * DeltaVector.z);
	float pitch = -asin(DeltaVector.z / DeltaVectorLength) * (180 / PI);
	float yaw = atan2(DeltaVector.y, DeltaVector.x) * (180 / PI);
	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		Write<float>(hProcess, pitchaddr + PitchOffset, pitch);
		Write<float>(hProcess, pitchaddr + YawOffset, yaw);
	}
}

DWORD aimbotloop()
{
	while (1)
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			int xt = GetClosestEnemy();
			vector3 uh = GetBonePosEnemy(xt, 8);
			AimAt(&uh);
		}
	Sleep(3);
}

void aa(vector3* EnemyPositionHead)
{
	int oof = GetIndex();
	DWORD pitchaddr;
	ReadProcessMemory(hProcess, (LPVOID)(moduleEngine + ClientStateOffset), &pitchaddr, sizeof(pitchaddr), NULL);
	vector3 OriginLocal = GetPlayerOrigin(oof);
	vector3 ViewOffsetLocal = GetPlayerView(oof);
	vector3* PosLocal = &(OriginLocal + ViewOffsetLocal);
	vector3 DeltaVector = { EnemyPositionHead->x - PosLocal->x, EnemyPositionHead->y - PosLocal->y, EnemyPositionHead->z - PosLocal->z };
	float DeltaVectorLength = sqrt(DeltaVector.x * DeltaVector.x + DeltaVector.y * DeltaVector.y + DeltaVector.z * DeltaVector.z);
	float pitch = -asin(DeltaVector.z / DeltaVectorLength) * (180 / PI);
	float yaw = atan2(DeltaVector.y, DeltaVector.x) * (180 / PI);
	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		yaw += 179;
		pitch = 89;
		BYTE bytes_temp[4];
		BYTE bytes[4];
		ULONG i1;
		ULONG i2;
		std::memcpy((void*)bytes_temp, (void*)&pitch, 4);
		std::memcpy((void*)bytes, (void*)&yaw, 4);
		std::memcpy((void*)&i1, (void*)bytes_temp, 4);
		std::memcpy((void*)&i2, (void*)bytes, 4);
		WriteProcessMemory(hProcess, (LPVOID)(pitchaddr + PitchOffset), &i1, sizeof(pitch), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(pitchaddr + YawOffset), &i2, sizeof(yaw), NULL);
	}
}

DWORD SpinbotLoop()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_LMENU))
		{
			int x2 = GetIndex();
			{

				for (int x = 0; x <= MAXPLAYER; x++)
				{
					if (!GetPlayerAddress(x))
					{
						continue;
					}
					if (x == x2)
					{
						continue;
					}
					if (GetPlayerTeam(x) == GetPlayerTeam(x2))
					{
						continue;
					}
					else
					{
						vector3 uh = GetBonePosEnemy(x, 8);
						AimAt(&uh);
						Sleep(2);
						//handleTBot();
					}
				}

			}
		}
		Sleep(2);
	}
}

void GlowLoop()
{
	while (1)
	{
		for (int i = 0; i <= MAXPLAYER; i++)
		{
			DWORD glow_Pointer = Read<DWORD>(hProcess, modulePanorama + GlowObjectManagerOffset);
			int glow_currentPlayerGlowIndex = Read<int>(hProcess, GetPlayerAddress(i) + GlowIndexOffset);
			int health = GetPlayerHealth(i);
			int team = GetPlayerTeam(i);
			bool b1 = GetDormant(i);
			if (health > 0 && health < 101 && b1 == false)
			{
				switch (team)
				{
				case 2:
				{
					bool b2 = true;
					bool b3 = false;
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)), &GlowTerroristRed, sizeof(GlowTerroristRed), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)), &GlowTerroristGreen, sizeof(GlowTerroristGreen), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)), &GlowTerroristBlue, sizeof(GlowTerroristBlue), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)), &GlowTerroristAlpha, sizeof(GlowTerroristAlpha), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)), &b2, sizeof(b2), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)), &b3, sizeof(b3), NULL);
					break;
				}
				case 3:
					bool b2 = true;
					bool b3 = false;
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)), &GlowCounterTerroristRed, sizeof(GlowCounterTerroristRed), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)), &GlowCounterTerroristGreen, sizeof(GlowCounterTerroristGreen), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)), &GlowCounterTerroristBlue, sizeof(GlowCounterTerroristBlue), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)), &GlowCounterTerroristAlpha, sizeof(GlowCounterTerroristAlpha), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)), &b2, sizeof(b2), NULL);
					WriteProcessMemory(hProcess, (LPVOID)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)), &b3, sizeof(b3), NULL);
					break;
				}
			}
		}
	}
}








void shoot()
{
	auto of = 0x5;
	auto of2 = 0x4;
	WriteProcessMemory(hProcess, (LPVOID)(modulePanorama + dwForceAttack), &of, sizeof(of), 0);
	Sleep(20);
	WriteProcessMemory(hProcess, (LPVOID)(modulePanorama + dwForceAttack), &of2, sizeof(of2), 0);
	Sleep(500);
}


bool CheckCrosshair()
{
	int crosshair = Read <int>(hProcess, GetPlayerAddress(GetIndex()) + CrosshairOffset);

	if (crosshair != GetIndex() && crosshair < MAXPLAYER)
	{
		//DWORD entity = GetPlayerAddress(crosshair - 1); // Read <DWORD>(hProcess, modulePanorama + EntityListOffset + ((crosshair - 1) * 0x10));
		//int team, health;
		//ReadProcessMemory(hProcess, (LPCVOID)(modulePanorama + EntityListOffset + ((crosshair - 1) * 0x10)), &entity, sizeof(entity), 0);
		//ReadProcessMemory(hProcess, (LPCVOID)(entity + TeamOffset), &team, sizeof(team), 0);
		//ReadProcessMemory(hProcess, (LPCVOID)(entity + HealthOffset), &health, sizeof(health), 0);
		if (GetPlayerTeam(crosshair - 1) != GetPlayerTeam(GetIndex()) && GetPlayerHealth(crosshair - 1) > 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

void handleTBot()
{
	if (CheckCrosshair())
		shoot();
}



void BunnyHop()
{
	auto v1 = 0x5;
	auto v2 = 0x4;

	while (1)
	{
		DWORD InGround = Read <DWORD>(hProcess, GetPlayerAddress(GetIndex()) + fFlagsOffset);

		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (InGround & 1 == 1))
		{
			WriteProcessMemory(hProcess, (LPVOID)(modulePanorama + ForceJumpOffset), &v1, sizeof(v1), NULL);
			Sleep(50);
			WriteProcessMemory(hProcess, (LPVOID)(modulePanorama + ForceJumpOffset), &v2, sizeof(v2), NULL);
		}
	}
}

