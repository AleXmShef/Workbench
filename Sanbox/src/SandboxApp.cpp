#include <Workbench.h>
#include <Windows.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::cout << argv[i] << "\n";
	}
	
	int result;
	{
		auto pEngParams = new Workbench::Engine::EngineProps;
		pEngParams->windowTitle = "Sandbox";
		pEngParams->isFullScreen = true;
		Workbench::Engine mEngine(pEngParams);
		result = mEngine.Run();
	}
#if defined(WB_DEBUG)
	system("pause");
#endif
	return result;
}