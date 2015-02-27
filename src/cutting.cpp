#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include <stdio.h>
#include <math.h>

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "meshes/Mesh.h"

using namespace cut;

const char* windowTitle = "Mesh cutting example";

// Globals
HWND hWnd;
HDC hDC;
HGLRC hRC;
Mesh mesh;
Mesh left, right;

bool running = true;

int width, height;

// Function declarations
void render(double dt);
HWND createWindow(HINSTANCE hInstance, int cmdShow);
void resizeGL(GLsizei width, GLsizei height);
void initGL();
LRESULT CALLBACK messageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	GLuint textureId;
	int* texture;

	// FPS counter
	int fps = 0;
	int frames = 0;
	int lastFpsUpdate = GetTickCount();

	// Timing
	LARGE_INTEGER timerFreq;
	LARGE_INTEGER lastUpdate;

	// Initialise timing
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&lastUpdate);

	// Create window
	hWnd = createWindow(hInstance, nCmdShow);
	hDC = GetDC(hWnd);

	// Initialise OpenGL
	initGL();
	resizeGL(width, height);

	// Load mesh
	mesh.loadObj("teapot.obj");

	// Cut mesh
	Vector3 planePoint = { 0, 0, 0 };
	Vector4 planeNormal = { 1, 0, 0, 0 };

	// Listen for messages
	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Timing
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		unsigned int timePassed = (current.QuadPart - lastUpdate.QuadPart) * 1000 / timerFreq.QuadPart;
		float dt = (float)timePassed;
		lastUpdate = current;

		// Rotate normal
		Vector4 normal;

		static float rot = 0.0f;
		rot += 0.1f;

		float rotation[16];
		glLoadIdentity();
		glRotatef(rot, 1.0f, 1.0f, 1.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, rotation);

		multVector4((Matrix4*)rotation, &planeNormal, &normal);

		// Cut box
		mesh.cut(&left, &right, planePoint, *(Vector3*)&normal);

		// Render window
		render(0);
		SwapBuffers(hDC);

		// Update frame counter
		frames++;

		// If it's been a second, update window title and FPS
		if (GetTickCount() - lastFpsUpdate > 1000)
		{
			lastFpsUpdate = GetTickCount();

			fps = frames;
			frames = 0;

			const int bufferSize = 256;
			char title[bufferSize];
			sprintf_s(title, bufferSize, "%s [FPS: %d]", windowTitle, fps);
			SetWindowText(hWnd, title);
		}

	}

	return 0;
}

void render(double dt)
{
	static float rotation = 0.0f;
	rotation += 0.01f;

	// Clear screen
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable vertex and normal arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Set up transformation
	glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -15.0f);
    glScalef(0.025f, 0.025f, 0.025f);

	// Draw mesh
	glVertexPointer(3, GL_FLOAT, 0, mesh.vertices);
	glNormalPointer(GL_FLOAT, 0, mesh.vertexNormals);
	glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, mesh.indices);

	// Set up transformation
	glLoadIdentity();
    glTranslatef(5.0f, 0.0f, -15.0f);
    glScalef(0.025f, 0.025f, 0.025f);
	//glRotatef(rotation, 1, 1, 0);

	// Draw mesh
	glVertexPointer(3, GL_FLOAT, 0, left.vertices);
	glNormalPointer(GL_FLOAT, 0, left.vertexNormals);
	glDrawElements(GL_TRIANGLES, left.indexCount, GL_UNSIGNED_INT, left.indices);

	// Set up transformation
	glLoadIdentity();
    glTranslatef(-5.0f, 0.0f, -15.0f);
    glScalef(0.025f, 0.025f, 0.025f);

	// Draw mesh
	glVertexPointer(3, GL_FLOAT, 0, right.vertices);
	glNormalPointer(GL_FLOAT, 0, right.vertexNormals);
	glDrawElements(GL_TRIANGLES, right.indexCount, GL_UNSIGNED_INT, right.indices);
}

// Create Window
HWND createWindow(HINSTANCE hInstance, int cmdShow)
{
	// Return HWND value
	HWND hWnd;

	// Create window class
	const int width = 800;
	const int height = 600;
	const char* className = "cuttingDemoClass";

	WNDCLASS wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASS));
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = messageHandler;
	wndClass.lpszClassName = className;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// Register window class and create window
	RegisterClass(&wndClass);
	hWnd = CreateWindow(className,				// Window class
						windowTitle,			// Window title
						WS_OVERLAPPEDWINDOW,	// Window style (a preset)
						CW_USEDEFAULT,			// x position
						CW_USEDEFAULT,			// y position
						width, height,			// Window size
						NULL, NULL,				// Parent and menu (optional)
						hInstance,
						NULL);					// Optional parameter

	ShowWindow(hWnd, cmdShow);

	return hWnd;
}

void resizeGL(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
}

void initGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);

	// Create OpenGL context
	hRC = wglCreateContext(hDC);
	DWORD error = GetLastError();
	wglMakeCurrent(hDC, hRC);

	// Set up OpenGL state
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat lightpos[] = {1.0f, 0.0f, 1.0f, 1.0f};
	GLfloat ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

// Handle messages
LRESULT CALLBACK messageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paint;
	HDC hDC;

	switch(message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &paint);
		EndPaint(hWnd, &paint);
		break;
	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);

		resizeGL(width, height);
		break;
	case WM_DESTROY:
		running = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
