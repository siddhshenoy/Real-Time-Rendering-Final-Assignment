#include "Window.h"

Window::Window()
{
    mWindow = NULL;
    mShouldClose = false;
    mIsMouseLockedAtOrigin = false;
    mInitFunction = NULL;
}

Window::Window(const char* title, int x, int y, int width, int height, bool fullscreen)
{
    this->Create(title, x, y, width, height, fullscreen);

}

void Window::Create(const char* title, int x, int y, int width, int height, bool fullscreen)
{

    if (!glfwInit()) {
        std::cerr << __FUNCTION__ << ": GLFW initialization failed!" << std::endl;

    }
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if(!fullscreen)
        mWindow = glfwCreateWindow(mode->width, mode->height, title, NULL, NULL);
    else
        mWindow = glfwCreateWindow(mode->width, mode->height, title, glfwGetPrimaryMonitor(), NULL);
    glfwSetWindowPos(mWindow, x, y);
    glfwMakeContextCurrent(mWindow);


    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << __FUNCTION__ << ": Glew initialization failed with an error!" << std::endl;
        //glewGetErrorString(glewInitResult);
        //return 1;
    }
    currentFrameTime = (float)glfwGetTime();
    lastFrameTime = (float)glfwGetTime();
    frameCountLastTick = (float)glfwGetTime();

}

void Window::SetPosition(int x, int y)
{
    if (mWindow != NULL) {
        glfwSetWindowPos(mWindow, x, y);
    }
}

void Window::SetDimensions(int width, int height)
{
    if (mWindow != NULL) {
        glfwSetWindowSize(mWindow, width, height);
    }
}

void Window::GetPosition(int* x, int* y)
{
}

void Window::GetDimensions(int* width, int* height)
{
    if (mWindow != NULL)
        glfwGetWindowSize(mWindow, width, height);
}

int Window::GetScreenWidth()
{
    int width = -1, height = -1;
    if (mWindow != NULL)
        glfwGetWindowSize(mWindow, &width, &height);
    return width;
}

int Window::GetScreenHeight()
{
    int width = -1, height = -1;
    if (mWindow != NULL)
        glfwGetWindowSize(mWindow, &width, &height);
    return height;
}

void Window::SetTitle(const char* title)
{
}

const char* Window::GetTitle()
{
    return nullptr;
}

void Window::SetRenderingFunction(RenderFunction function)
{
    mRenderFunction = function;
}

void Window::RenderLoop()
{
    if (mInitFunction != NULL) {
        this->mInitFunction(this->mWindow);
    }
    while (!glfwWindowShouldClose(mWindow) && !mShouldClose) {
        if (mIsMouseLockedAtOrigin) {
            GetDimensions(&screenWidth, &screenHeight);

            glfwSetCursorPos(mWindow, (int)(screenWidth / 2), (int)(screenHeight / 2));
        }
        if (mRenderFunction != NULL) {
            if (((float)glfwGetTime() - frameCountLastTick) > 1.0f) {
                totalUpdatedFramesPerSecond = framesPerSecond;
                framesPerSecond = 0;
                frameCountLastTick = glfwGetTime();
            }

            currentFrameTime = glfwGetTime();
            deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            //We derive some frames and delta time over here..
            mRenderFunction(mWindow);
            framesPerSecond++;
        }
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}
void Window::SetInitFunction(InitFunction function)
{
    mInitFunction = function;
}
void Window::SetWindowResizeCallback(GLFWwindowsizefun function)
{
    glfwSetWindowSizeCallback(mWindow, function);
}

void Window::SetWindowKeyboardCallback(GLFWkeyfun keyfunc)
{
    glfwSetKeyCallback(mWindow, keyfunc);
}

void Window::SetWindowMouseButtonCallback(GLFWmousebuttonfun mousebuttonfunc)
{
    glfwSetMouseButtonCallback(mWindow, mousebuttonfunc);
}
void Window::SetWindowMouseCursorCallback(GLFWcursorposfun cursorposfunc)
{
    glfwSetCursorPosCallback(mWindow, cursorposfunc);
}
void Window::SetWindowMouseScrollCallback(GLFWscrollfun scrollfunc) {
    glfwSetScrollCallback(mWindow, scrollfunc);
}
void Window::GetMouseDelta(MouseData& mouse_data, int oldx, int oldy)
{
    //printf("(%d, %d)\n", oldx, oldy);
    mouse_data.mouseX = oldx - mMouseData.mouseX;
    mouse_data.mouseY = oldy - mMouseData.mouseY;
    //mouse_data.mouseX = (mouse_data.mouseX < 0) ? (-1 * mouse_data.mouseX) : (mouse_data.mouseX);
    //mouse_data.mouseY = (mouse_data.mouseY < 0) ? (-1 * mouse_data.mouseY) : (mouse_data.mouseY);
    mMouseData.mouseX = oldx;
    mMouseData.mouseY = oldy;
}
void Window::ShowWindow() {
    glfwShowWindow(mWindow);
    
}
void Window::CloseWindow() {
    mShouldClose = true;
}
void Window::HideCursor()
{
    mCursorHidden = true;
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::ShowCursor()
{
    mCursorHidden = false;
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
float Window::GetDeltaTime()
{
    return this->deltaTime;
}
int Window::GetFramesPerSecond()
{
    return this->totalUpdatedFramesPerSecond;
}
bool Window::IsCursorHidden()
{
    return mCursorHidden;
}
GLFWwindow* Window::GetWindow() {
    return mWindow;
}
void Window::GetFrameBufferSize(int* x, int* y) {
    glfwGetFramebufferSize(mWindow, x, y);
}
void Window::GetCursorPosition(double* x, double* y) {
    glfwGetCursorPos(mWindow, x, y);
}

void Window::SetWindowFavIcon(const std::string& image_path) {
    GLFWimage IconImages[1];
    int image_dim[2];
    IconImages[0].pixels = stbi_load(image_path.c_str(), &IconImages[0].width, &IconImages[0].height, 0, 4);
    glfwSetWindowIcon(mWindow, 1, IconImages);
    stbi_image_free(IconImages[0].pixels);
}
Window::~Window()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}
