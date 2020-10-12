#include <Workbench.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::cout << argv[i] << "\n";
	}
	Workbench::Engine mEngine;
	mEngine.Run();
	return 0;
}