#pragma once

#include <iostream>
#include <glew/GL/glew.h>
#include <glfw/glfw3.h>

#include <stb/stb_image.h>

typedef struct MouseData {
	int mouseX;
	int mouseY;
}
MouseData;

typedef void (*RenderFunction)(GLFWwindow*);
typedef void (*InitFunction)(GLFWwindow*);


class Window
{
private:
	GLFWwindow* mWindow;
	RenderFunction mRenderFunction;
	InitFunction mInitFunction;
	bool mShouldClose;
	bool mCursorHidden;
	bool mIsMouseLockedAtOrigin;
	MouseData mMouseData;
	int screenWidth, screenHeight;
private:
	float deltaTime;
	float lastFrameTime;
	float currentFrameTime;
	int frameCountLastTick;
	int framesPerSecond;
	int totalUpdatedFramesPerSecond;
public:
	Window();
	Window(const char* title, int x, int y, int width, int height, bool fullscreen = false);
	void Create(const char* title, int x, int y, int width, int height, bool fullscreen = false);
	void ShowWindow();
	void SetPosition(int x, int y);
	void SetDimensions(int width, int height);
	void GetPosition(int* x, int* y);
	void GetDimensions(int* width, int* height);
	int GetScreenWidth();
	int GetScreenHeight();
	void SetTitle(const char* title);
	const char* GetTitle();
	void SetRenderingFunction(RenderFunction function);
	void SetInitFunction(InitFunction function);
	void RenderLoop();
	void SetWindowResizeCallback(GLFWwindowsizefun function);
	void SetWindowKeyboardCallback(GLFWkeyfun keyfunc);
	void SetWindowMouseButtonCallback(GLFWmousebuttonfun mousebuttonfunc);
	void SetWindowMouseCursorCallback(GLFWcursorposfun cursorposfunc);
	void SetWindowMouseScrollCallback(GLFWscrollfun scrollfunc);
	void GetMouseDelta(MouseData& mouse_data, int oldx, int oldy);
	void CloseWindow();
	void HideCursor();
	void ShowCursor();
	void SetWindowFavIcon(const std::string& image_path);
	void GetCursorPosition(double* x, double* y);
	float GetDeltaTime();
	int GetFramesPerSecond();
	void GetFrameBufferSize(int* x, int* y);
	void LockMouseAtOrigin() {
		mIsMouseLockedAtOrigin = true;
	}
	void UnlockMouseFromOrigin() {
		mIsMouseLockedAtOrigin = false;
	}
	bool IsMouseLockedAtOrigin() {
		return mIsMouseLockedAtOrigin;
	}
	bool IsCursorHidden();
	GLFWwindow* GetWindow();
	~Window();
};

