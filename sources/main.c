/* 
 * Example of a Windows OpenGL program.  
 * The OpenGL code is the same as that used in  
 * the X Window System sample 
 */ 
#include "tinytl/platform_selector.h"
#include "tinytl/memory/allocator/Arena.h"
 
/* Windows globals, defines, and prototypes */ 
#define APPNAME "Win OpenGL"
HWND  ghWnd; 
HDC   ghDC; 
HGLRC ghRC; 
 
#define SWAPBUFFERS SwapBuffers(ghDC) 
#define BLACK_INDEX     0 
#define RED_INDEX       13 
#define GREEN_INDEX     14 
#define BLUE_INDEX      16 
#define WIDTH           300 
#define HEIGHT          200 
 
LONG WINAPI MainWndProc (HWND, UINT, WPARAM, LPARAM); 
BOOL bSetupPixelFormat(HDC); 
 
/* OpenGL globals, defines, and prototypes */ 
GLfloat latitude, longitude, latinc, longinc; 
GLdouble radius; 
 
#define GLOBE    1 
#define CYLINDER 2 
#define CONE     3 
 
GLvoid resize(GLsizei, GLsizei); 
GLvoid initializeGL(GLsizei, GLsizei); 
GLvoid drawScene(GLvoid); 
void polarView( GLdouble, GLdouble, GLdouble, GLdouble); 
 
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 

 
    /* make sure window was created */ 
    if (!ghWnd) 
        return FALSE; 
 
    /* show and update main window */ 
    ShowWindow (ghWnd, nCmdShow); 
 
    UpdateWindow (ghWnd); 
 
    /* animation loop */ 
    while (1) { 
 
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
        { 
            if (!GetMessage(&msg, NULL, 0, 0) ) 
            { 
                break;
            } 
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        } 
        drawScene(); 
    } 
    return 0;
}

 
