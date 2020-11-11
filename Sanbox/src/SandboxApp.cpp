#include <Workbench.h>
#include <Windows.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::cout << argv[i] << "\n";
	}
	
	int result;
	{
		Workbench::Engine::EngineProps engParams;
		Workbench::Window::WindowProps windowProps;

		windowProps.windowTitle = "Sandbox";
		//windowProps.isFullScreen = true;
		engParams.windowProps = windowProps;

		Workbench::Engine mEngine(engParams);
		
		result = mEngine.Run();
	}
#if defined(WB_DEBUG)
	system("pause");
#endif
	return result;
}