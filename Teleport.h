#pragma once

class Teleport
{
public:
	static inline bool m_Teleport = false;
	static inline bool m_RequestedTeleport = false;
	static inline Vector3 m_RequestedLocation = { 0.0f,200.0f,0.0f };

private:
	static inline Vector3 m_StartingLocation = { 0.0f,0.0f,0.0f };

public:
	static bool OnFrame(DMA* dma);
	static bool SetPlayerLocation(DMA* dma, Vector3& Location);
};