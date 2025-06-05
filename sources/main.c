/* 
 * Example of a Windows OpenGL program.  
 * The OpenGL code is the same as that used in  
 * the X Window System sample 
 */ 
#include "tinytl/renderer/renderer_platform_selector.h"
 
/* Windows globals, defines, and prototypes */ 
CHAR szAppName[]="Win OpenGL"; 
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
    MSG        msg; 
    WNDCLASS   wndclass; 
 
    /* Register the frame class */ 
    wndclass.style         = 0; 
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc; 
    wndclass.cbClsExtra    = 0; 
    wndclass.cbWndExtra    = 0; 
    wndclass.hInstance     = hInstance; 
    wndclass.hIcon         = LoadIcon (hInstance, szAppName); 
    wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW); 
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
    wndclass.lpszMenuName  = szAppName; 
    wndclass.lpszClassName = szAppName; 
 
    if (!RegisterClass (&wndclass) ) 
        return FALSE; 
 
    /* Create the frame */ 
    ghWnd = CreateWindow (szAppName, 
             TEXT("Generic OpenGL Sample"), 
         WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
             CW_USEDEFAULT, 
             CW_USEDEFAULT, 
             WIDTH, 
             HEIGHT, 
             NULL, 
             NULL, 
             hInstance, 
             NULL); 
 
    /* make sure window was created */ 
    if (!ghWnd) 
        return FALSE; 
 
    /* show and update main window */ 
    ShowWindow (ghWnd, nCmdShow); 
 
    UpdateWindow (ghWnd); 
 
    /* animation loop */ 
    while (1) { 
        /* 
         *  Process all pending messages 
         */ 
 
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
        { 
            if (GetMessage(&msg, NULL, 0, 0) ) 
            { 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            } else { 
                return TRUE; 
            } 
        } 
        drawScene(); 
    } 
} 
 
/* main window procedure */ 
LONG WINAPI MainWndProc ( 
    HWND    hWnd, 
    UINT    uMsg, 
    WPARAM  wParam, 
    LPARAM  lParam) 
{ 
    LONG    lRet = 1; 
    PAINTSTRUCT    ps; 
    RECT rect; 
 
    switch (uMsg) { 
 
    case WM_CREATE: 
        ghDC = GetDC(hWnd); 
        if (!bSetupPixelFormat(ghDC)) 
            PostQuitMessage (0); 
 
        ghRC = wglCreateContext(ghDC); 
        wglMakeCurrent(ghDC, ghRC); 
        GetClientRect(hWnd, &rect); 
        initializeGL(rect.right, rect.bottom); 
        break; 
 
    case WM_PAINT: 
        BeginPaint(hWnd, &ps); 
        EndPaint(hWnd, &ps); 
        break; 
 
    case WM_SIZE: 
        GetClientRect(hWnd, &rect); 
        resize(rect.right, rect.bottom); 
        break; 
 
    case WM_CLOSE: 
        if (ghRC) 
            wglDeleteContext(ghRC); 
        if (ghDC) 
            ReleaseDC(hWnd, ghDC); 
        ghRC = 0; 
        ghDC = 0; 
 
        DestroyWindow (hWnd); 
        break; 
 
    case WM_DESTROY: 
        if (ghRC) 
            wglDeleteContext(ghRC); 
        if (ghDC) 
            ReleaseDC(hWnd, ghDC); 
 
        PostQuitMessage (0); 
        break; 
     
    case WM_KEYDOWN: 
        switch (wParam) { 
        case VK_LEFT: 
            longinc += 0.5F; 
            break; 
        case VK_RIGHT: 
            longinc -= 0.5F; 
            break; 
        case VK_UP: 
            latinc += 0.5F; 
            break; 
        case VK_DOWN: 
            latinc -= 0.5F; 
            break; 
        } 
 
    default: 
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet; 
} 
 
BOOL bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd, *ppfd; 
    int pixelformat; 
 
    ppfd = &pfd; 
 
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    ppfd->nVersion = 1; 
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  
                        PFD_DOUBLEBUFFER; 
    ppfd->dwLayerMask = PFD_MAIN_PLANE; 
    ppfd->iPixelType = PFD_TYPE_COLORINDEX; 
    ppfd->cColorBits = 8; 
    ppfd->cDepthBits = 16; 
    ppfd->cAccumBits = 0; 
    ppfd->cStencilBits = 0; 
 
    pixelformat = ChoosePixelFormat(hdc, ppfd); 
 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        MessageBox(NULL, TEXT("ChoosePixelFormat failed"), TEXT("Error"), MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        MessageBox(NULL, TEXT("SetPixelFormat failed"), TEXT("Error"), MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 
} 
 
/* OpenGL code */ 
 
GLvoid resize( GLsizei width, GLsizei height ) 
{ 
    GLfloat aspect; 
 
    glViewport( 0, 0, width, height ); 
 
    aspect = (GLfloat) width / height; 
}     
 
GLvoid createObjects() 
{  
} 
 
GLvoid initializeGL(GLsizei width, GLsizei height) 
{ 
    GLfloat     maxObjectSize, aspect; 
    GLdouble    near_plane, far_plane; 
 
    glClearIndex( (GLfloat)BLACK_INDEX); 
    glClearDepth( 1.0 ); 
 
    glEnable(GL_DEPTH_TEST); 
 
    glMatrixMode( GL_PROJECTION ); 
    aspect = (GLfloat) width / height; 
    glMatrixMode( GL_MODELVIEW ); 
 
    near_plane = 3.0; 
    far_plane = 7.0; 
    maxObjectSize = 3.0F; 
    radius = near_plane + maxObjectSize/2.0; 
 
    latitude = 0.0F; 
    longitude = 0.0F; 
    latinc = 6.0F; 
    longinc = 2.5F; 
 
    createObjects(); 
} 
 
void polarView(GLdouble radius, GLdouble twist, GLdouble latitude, 
           GLdouble longitude) 
{ 
    glTranslated(0.0, 0.0, -radius); 
    glRotated(-twist, 0.0, 0.0, 1.0); 
    glRotated(-latitude, 1.0, 0.0, 0.0); 
    glRotated(longitude, 0.0, 0.0, 1.0);      
 
} 
 
GLvoid drawScene(GLvoid) 
{ 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
 
    glPushMatrix(); 
 
        latitude += latinc; 
        longitude += longinc; 
 
        polarView( radius, 0, latitude, longitude ); 
 
        glIndexi(RED_INDEX); 
        glCallList(CONE); 
 
        glIndexi(BLUE_INDEX); 
        glCallList(GLOBE); 
 
    glIndexi(GREEN_INDEX); 
        glPushMatrix(); 
            glTranslatef(0.8F, -0.65F, 0.0F); 
            glRotatef(30.0F, 1.0F, 0.5F, 1.0F); 
            glCallList(CYLINDER); 
        glPopMatrix(); 
 
    glPopMatrix(); 
 
    SWAPBUFFERS; 
}