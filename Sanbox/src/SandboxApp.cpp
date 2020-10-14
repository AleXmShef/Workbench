#include <Workbench.h>
#include <Windows.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::cout << argv[i] << "\n";
	}

	auto pEngParams = new Workbench::Engine::EngineProps;
	Workbench::Engine mEngine(pEngParams);
	mEngine.Run();

	return 0;
}