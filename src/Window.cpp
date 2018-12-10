#include "Window.h"
#include "Logger.h"

Window* Window::windowClass = 0;
bool Window::windowClosingFlag = false;

Window::Window() {

}

int Window::onClosing(uiWindow *w, void *data){
	Window::windowClosingFlag = true;
	uiQuit();
	return 1;
}

void Window::createWindow(std::string windowName, int width, int height, bool allowMenu) {
	memset(&windowOptions, 0, sizeof(uiInitOptions));
	if (uiInit(&windowOptions) != NULL) {
		WindowLog("Couldn't create a window! Issue: %s", "Window options was not set.")
		abort();
	}

	window = uiNewWindow(windowName.c_str(), width, height, (int)allowMenu);

	uiWindowOnClosing(window, Window::onClosing, NULL);
	uiControlShow(uiControl(window));
	uiMain();
}
bool Window::shouldWindowClose()
{
	return Window::window;
};


Window::~Window() {

	uiQuit();
}

