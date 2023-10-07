#pragma once
#include "stdafx.h"

class Memory {
public:

	~Memory() {
		if (m_pMemory != nullptr)
			free(m_pMemory);
	}

	void SetSize(unsigned long size) {
		if (m_pMemory == nullptr || (size != m_Size)) {
			if (m_pMemory != nullptr)
				free(m_pMemory);
			m_pMemory = (unsigned char*)malloc(size);
			RtlSecureZeroMemory(m_pMemory, size);
			m_Size = size;
		}
	}

	unsigned char* Data() { return m_pMemory; };
	unsigned long DataSize() const { return m_Size; }

private:
	// ÄÚ´æÊý¾Ý
	unsigned char* m_pMemory{ nullptr };
	unsigned long m_Size;
};