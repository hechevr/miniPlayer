#include "playWave.hpp"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage: player.exe filename" << std::endl;
		exit(-1);
	}
	char* filename = argv[1];

	miniPlayer::mini myPlayer(filename);
	myPlayer.miniPlay();
}