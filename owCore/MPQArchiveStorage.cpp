#include "stdafx.h"

// General
#include "MPQArchiveStorage.h"

const char* MPQArchiveStorage::archives = "D:/_games/World of Warcraft 1.12.1/Data/";
vector<mpq_archive_s*> MPQArchiveStorage::m_OpenArchives;

void MPQArchiveStorage::AddArchive(string filename)
{
	mpq_archive_s* mpq_a;
	int result = libmpq__archive_open(&mpq_a, (MPQArchiveStorage::archives + filename).c_str(), -1);
	Log::Info("Opening %s", filename.c_str());
	if (result)
	{
		switch (result)
		{
			case LIBMPQ_ERROR_OPEN:
            Log::Error("Error opening archive [%s]: Does file really exist?", filename.c_str());
			break;

			case LIBMPQ_ERROR_FORMAT:            /* bad file format */
			Log::Error("Error opening archive [%s]: Bad file format", filename.c_str());
			break;

			case LIBMPQ_ERROR_SEEK:         /* seeking in file failed */
			Log::Error("Error opening archive [%s]: Seeking in file failed", filename.c_str());
			break;

			case LIBMPQ_ERROR_READ:              /* Read error in archive */
			Log::Error("Error opening archive [%s]: Read error in archive", filename.c_str());
			break;

			case LIBMPQ_ERROR_MALLOC:               /* maybe not enough memory? :) */
			Log::Error("Error opening archive [%s]: Maybe not enough memory", filename.c_str());
			break;

			default:
			Log::Error("Error opening archive [%s]: Unknown error\n", filename.c_str());
			break;
		}

		return;
	}

	m_OpenArchives.push_back(mpq_a);
	Log::Green("MPQFile[%s]: Added!", filename.c_str());
}

MPQFileLocation MPQArchiveStorage::GetFileLocation(cstring filename)
{
	for (auto i = m_OpenArchives.rbegin(); i != m_OpenArchives.rend(); ++i)
	{
		mpq_archive_s* mpq_a = *i;

		uint32 filenum;
		if (libmpq__file_number(mpq_a, filename.c_str(), &filenum) == LIBMPQ_ERROR_EXIST)
		{
			continue;
		}

		return MPQFileLocation(mpq_a, filenum);
	}

	return MPQFileLocation();
}

void MPQArchiveStorage::ClearArchives()
{
	for (auto it : m_OpenArchives)
	{
		libmpq__archive_close(it);
	}
}

void MPQArchiveStorage::InitCommonArchives()
{
	// Files
	AddArchive(string("backup.MPQ"));
	AddArchive(string("base.MPQ"));
	AddArchive(string("dbc.MPQ"));
	AddArchive(string("fonts.MPQ"));
	AddArchive(string("interface.MPQ"));
	AddArchive(string("misc.MPQ"));
	AddArchive(string("model.MPQ"));
	AddArchive(string("patch.MPQ"));
	AddArchive(string("patch-2.MPQ"));
	AddArchive(string("patch-3.MPQ"));
	AddArchive(string("sound.MPQ"));
	AddArchive(string("speech.MPQ"));
	AddArchive(string("terrain.MPQ"));
	AddArchive(string("texture.MPQ"));
	AddArchive(string("wmo.MPQ"));

	//AddArchive(string("ruRU/patch-1.MPQ"));
	//AddArchive(string("ruRU/patch-2.MPQ"));
	//AddArchive(string("ruRU/patch-3.MPQ"));
}
