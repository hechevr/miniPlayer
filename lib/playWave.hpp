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
		
		void miniPlay();

		void miniPlause();

		// public attributes
		char* data;
		unsigned int dataSize;

	private:
		// private attributes
		char* filename;

		unsigned int BUFFERSIZE;
		unsigned int bufferCount;

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

	mini::mini(char* filename, unsigned int BUFFERSIZE){
		this->BUFFERSIZE = BUFFERSIZE;
		this->filename = new char[strlen(filename)];
		strcpy(this->filename, filename);
		initialize();
	}

	void mini::initialize() {
		miniOpen();
		readRIFF();
		readFmt();
		readData();
	}
	void mini::miniOpen() {
		std::cout << "Try to open " << filename << std::endl;
		hmmioIn = mmioOpen(filename, NULL, MMIO_READ);
		if (hmmioIn == NULL) {
			std::cerr << "Error: cannot open " << filename << std::endl;
			exit(-1);
		}
		std::cout << "Open file successfully" << std::endl;
	}
	void mini::readRIFF() {
		std::cout << "Read RIFF info" << std::endl;
		memset(&RIFFInfo, 0, sizeof(MMCKINFO));
		RIFFInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		int res = mmioDescend(hmmioIn, &RIFFInfo, NULL, MMIO_FINDRIFF);
		if (res) {
			std::cerr << "Error: cannot find WAVE." << std::endl;
			mmioClose(hmmioIn, 0);
			exit(-1);
		}
		std::cout << "Read RIFF info successuflly" << std::endl;
	}
	void mini::readFmt() {
		std::cout << "Read FMT" << std::endl;
		FMTChunkInfo.ckid = mmioStringToFOURCC("fmt", 0);
		int res = mmioDescend(hmmioIn, &FMTChunkInfo, &RIFFInfo, MMIO_FINDCHUNK);
		if (res){
			std::cerr << "Error: cannot read fmt." << std::endl;
			mmioClose(hmmioIn, 0);
			exit(-1);
		}

		res = mmioRead(hmmioIn, (char*)&waveFmt, FMTChunkInfo.cksize);
		if (res != FMTChunkInfo.cksize){
			std::cerr << "Error: cannot read waveFmt." << std::endl;
			std::cerr << res << std::endl;
			mmioClose(hmmioIn, 0);
			exit(-1);
		}
		std::cout << "Read FMT sucessfully" << std::endl;
	}
	void mini::readData() {
		std::cout << "Read DATA" << std::endl;
		int res;
		res = mmioAscend(hmmioIn, &FMTChunkInfo, 0);
		if (res){
			std::cerr << "Can not ascend" << std::endl;
			mmioClose(hmmioIn, 0);
			exit(-1);
		}
		DATAChunkInfo.ckid = mmioStringToFOURCC("data", 0);
		res = mmioDescend(hmmioIn, &DATAChunkInfo, &RIFFInfo, MMIO_FINDCHUNK);
		if (res){
			std::cerr << "Cannot Descend to data chunk" << std::endl;
			mmioClose(hmmioIn, 0);
			exit(-1);
		}

		dataSize = DATAChunkInfo.cksize;
		data = new char[dataSize];
		res = mmioRead(hmmioIn, data, dataSize);
		if (res != dataSize) {
			std::cerr << "Not enough data" << std::endl;
			mmioClose(hmmioIn, 0);
			exit(-1);
		}
		mmioClose(hmmioIn, 0);
		std::cout << "Read data successfully" << std::endl;
	}
	void mini::setDevice() {
		std::cout << "Set Device" << std::endl;
		// query device
		int res = waveOutOpen(&WaveHandle, WAVE_MAPPER, &waveFmt, 0, 0, WAVE_FORMAT_QUERY);
		if (res) {
			std::cout << "Error: device doesn't support the format." << std::endl;
			exit(-1);
		}

		// open device
		res = waveOutOpen(&WaveHandle, WAVE_MAPPER, &waveFmt, 0, 0, CALLBACK_FUNCTION);
		if (res){
			std::cout << "Error: something wrong!" << std::endl;
			exit(-1);
		}
		std::cout << "Set device successfully" << std::endl;
	}

	void mini::setBuffer() {
		std::cout << "Set Buffer" << std::endl;
		bufferCount = dataSize / BUFFERSIZE + 1;
		std::cout << bufferCount << " " << dataSize << " " << BUFFERSIZE << std::endl;
		waveBuffer = new WAVEHDR[bufferCount];
		int i = 0;
		for (i = 0; i < bufferCount; i++) {
			WAVEHDR &wd = waveBuffer[i];
			wd.lpData = data + i*BUFFERSIZE;
			wd.dwFlags = 0;
			wd.dwBufferLength = BUFFERSIZE;
		}
		std::cout << "Set Buffer successfully" << std::endl;
	}

	void mini::miniPlay() {
		setDevice();
		setBuffer();
		waveOutSetVolume(WaveHandle, 0x80008000);
		std::cout << "Start to play" << std::endl;
		int i = 0;
		for (i = 0; i < bufferCount; i++){
			WAVEHDR &wd = waveBuffer[i];
			waveOutPrepareHeader(WaveHandle, &wd, sizeof(WAVEHDR));
			waveOutWrite(WaveHandle, &wd, sizeof(WAVEHDR));
			std::cout << i << std::endl;
			Sleep(1000);
		}
		Sleep(50000);
		
	}
}