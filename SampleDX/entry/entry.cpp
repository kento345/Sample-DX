#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include<Windows.h>

#include"Application/Application.h"

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR , int)
{
	Application app;
	if (!app.initialize(hInstance))
	{
		return -1;
	}

	app.loop();

	return 0;
}	