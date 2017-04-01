#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <fstream>
#pragma comment(lib, "winmm.lib")

namespace miniPlayer
{
	class mini
	{
	public:
		// constructor
		mini(char* filename, unsigned int BUFFERSIZE = 8000 * 60);

		// public function

		// for continue
		void miniPlay();
		// for stop
		void miniStop();
		// for restart
		void miniRestart();
		// plause
		void miniPlause();

		// public attributes
		char* data;
		unsigned int dataSize;

	private:
		// private attributes
		char* filename;

		unsigned int BUFFERSIZE;
		unsigned int bufferCount;
		unsigned int state;
		unsigned int plause;

		// hmmio
		HMMIO hmmioIn;
		// info
		MMCKINFO RIFFInfo;
		MMCKINFO FMTChunkInfo;
		WAVEFORMATEX waveFmt;
		MMCKINFO DATAChunkInfo;

		// device
		HWAVEOUT WaveHandle;

		// buffer
		WAVEHDR* waveBuffer;

		// initialize
		void initialize();
		void miniOpen();
		void readRIFF();
		void readFmt();
		void readData();
		void setBuffer();

		// query a device
		void setDevice();

	};
}
