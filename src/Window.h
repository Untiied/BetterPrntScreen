#pragma once
#include <ui.h>
#include <string>

class Window
{
public:
	~Window();
private:
	//Hidden to force singleton use.
	Window();
	static Window*	windowClass;
	static bool		windowClosingFlag;

	uiInitOptions	windowOptions;
	uiWindow*		window;
	std::string		m_windowName;
	int				m_windowWidth;
	int				m_windowHeight;
public:
	static int		onClosing(uiWindow *w, void *data);

	void			createWindow(std::string windowName, int width, int height, bool allowMenu);

	bool			shouldWindowClose();
public:
	static inline Window*	getWindow() {
		if (windowClass == nullptr) {
			windowClass = new Window();
		}
		return windowClass;
	};
	inline std::string		getWindowName() { return m_windowName; };
	inline int				getWindowWidth() { return m_windowWidth; };
	inline int				getWindowHeight() { return m_windowHeight; };
};

