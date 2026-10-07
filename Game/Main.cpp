#include "LaunchApplication.h"

#if _DEBUG
int main()
{
	return LaunchApplication();
}
#else
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	return LaunchApplication();
}
#endif