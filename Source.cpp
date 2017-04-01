#include "lib/playWave.hpp"
#include <tchar.h>
#include <strsafe.h>

struct thdata {
	miniPlayer::mini* myPlayer;
	int flag;
};

DWORD WINAPI threadPlay(LPVOID lpParam) {
	struct thdata* data = (struct thdata*)lpParam;
	if (data->flag == 0 || data->flag == 1) {
		// play thread
		data->myPlayer->miniPlay();
	}
	else if (data->flag == 2) {
		// restart thread
		data->myPlayer->mini::miniRestart();
	}
	else {
		Sleep(200);
		int choice;
		std::cout << "1. plause" << std::endl;
		std::cout << "2. stop" << std::endl;
		std::cin >> choice;
		if (choice == 1) {
			data->myPlayer->miniPlause();
		}
		else {
			data->myPlayer->miniStop();
		}
	}
	return 0;
}



int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage: player.exe filename" << std::endl;
		exit(-1);
	}
	char* filename = argv[1];

	miniPlayer::mini myPlayer(filename);
	// myPlayer.miniPlay();
	struct thdata* data = (struct thdata*)malloc(sizeof(struct thdata)*2);
	data[0].myPlayer = &myPlayer;
	data[1].myPlayer = &myPlayer;
	data[0].flag = 0;
	data[1].flag = 3;
	int count = 0;
	while (1) {

		DWORD dwThreadIdArray[2];
		HANDLE hThreadArray[2];
		if (count == 1) {
			std::cout << "1. continue" << std::endl;
			std::cout << "2. restart" << std::endl;
			std::cout << "3. stop" << std::endl;
			int choice;
			std::cin >> choice;
			if (choice == 1) {
				data[0].flag = 1;
				// create 2 threads
				for (int i = 0; i < 2; i++) {
					hThreadArray[i] = CreateThread(
						NULL,
						0,
						threadPlay,
						&data[i],
						0,
						&dwThreadIdArray[i]);
				}
				WaitForMultipleObjects(2, hThreadArray, TRUE, INFINITE);
			}
			else if (choice == 2) {
				data[0].flag = 2;
				// create 2 threads
				for (int i = 0; i < 2; i++) {
					hThreadArray[i] = CreateThread(
						NULL,
						0,
						threadPlay,
						&data[i],
						0,
						&dwThreadIdArray[i]);
				}
				WaitForMultipleObjects(2, hThreadArray, TRUE, INFINITE);
			}
			else {
				break;
			}
		}
		else {
			for (int i = 0; i < 2; i++) {
				hThreadArray[i] = CreateThread(
					NULL,
					0,
					threadPlay,
					&data[i],
					0,
					&dwThreadIdArray[i]);
			}
			WaitForMultipleObjects(2, hThreadArray, TRUE, INFINITE);
			count++;
		}
	}

}