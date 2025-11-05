#pragma once
#include <Siv3D.hpp>
#include "TrainingScene.h"
class CharacterStatusChangeTable
{
public:
	CharacterStatusChangeTable(String fileExtension,int size,uint64 hash):
		m_fileExtension(fileExtension),
		m_size(size),
		m_hash(hash)
	{
	}
private:
	String m_fileExtension;
	int m_size;
	uint64 m_hash;
public:
	Array<SystemStatusAddData> statusTable();
};

