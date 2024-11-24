#include "Application.h"
#include <crtdbg.h>
int main() {
	{
	Application app;
	app.Run();
	}
	_CrtDumpMemoryLeaks();
}