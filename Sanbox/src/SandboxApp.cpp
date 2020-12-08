#include <Workbench.h>
#include "ParabolicTrajectory.h"
//#include "RotatingCubesDemo.h"

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
  }

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::cout << argv[i] << "\n";
	}
	
	int result;
	{
		Workbench::Engine::EngineProps engParams = {};
		Workbench::Window::WindowProps windowProps = {};
		//windowProps.isVsync = true;

		windowProps.windowTitle = "Sandbox";
		//windowProps.isFullScreen = true;
		engParams.windowProps = windowProps;

		Workbench::Engine mEngine(engParams);
		
		mEngine.PushLayer(std::make_shared<ParabolicTrajectory>(&mEngine));
		//mEngine.PushLayer(std::make_shared<RotatinCubesDemo>(&mEngine));

		result = mEngine.Run();
	}
#if defined(WB_DEBUG)
	system("pause");
#endif
	return result;
}