#ifndef GLWINDOW_H
#define GLWINDOW_H

#define MAX_LOADSTRING 100

// static opengl window class
class GLwindow
{
private:
	float m_width;
	float m_height;
	static GLwindow * m_instance;

	HWND hWnd;
	PIXELFORMATDESCRIPTOR * m_pfd;
	HDC * m_hdc; // a pointer to the hdc
	void CreatePixelFormatDescriptor();
public:

	HINSTANCE hInst;								// current instance
	TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

	GLuint	base;							// Base Display List For The Font Set

	GLwindow(void);
	~GLwindow(void);
	int Initialise(HINSTANCE hInstance, int nCmdShow, HDC hDC);
	int DrawScene(GLvoid);
	void GLResize(int width,int height);
	static GLwindow * GetInstance();
	PIXELFORMATDESCRIPTOR GetPFD();
	GLvoid BuildFont(GLvoid);					// build character font List
	GLvoid KillFont(GLvoid);						// Delete The Font List
	GLvoid Print(const char * fmt, ...);				// Custom GL "Print" Routine
	inline HWND & GetHwndRef()
	{
		return hWnd;
	}
	inline void SetTitle(char * text)
	{
		SetWindowText(hWnd, (LPCSTR)text);
	}
	inline void SetTitle(char * textOut, float value)
	{
		char text[100];
		sprintf(text,textOut,value);

		SetWindowText(hWnd, (LPCSTR)text);
	}
};

#endif