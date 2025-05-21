#pragma once

class RefreshHealth
{
public:
	static inline bool m_RefreshHealth = false;
	static inline float m_HealthThreshold = 0.85f;

public:
	static bool OnFrame(DMA* dma);
};