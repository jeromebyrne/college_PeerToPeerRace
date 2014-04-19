#include "Precompiled.h"
#include "GLwindow.h"

GLwindow * GLwindow::m_instance = 0;

GLwindow * GLwindow::GetInstance()
{
	if(m_instance == 0)
	{
		m_instance = new GLwindow();
	}
	return m_instance;
}

GLwindow::GLwindow(void) : m_width(800), m_height(600)
{
	CreatePixelFormatDescriptor();
}

GLwindow::~GLwindow(void)
{
	KillFont(); // delete the font list
}
GLvoid GLwindow :: BuildFont(GLvoid)
{

	HFONT	font;						// Windows Font ID
	HFONT	oldfont;					// Used For Good House Keeping

	base = glGenLists(96);					// Storage For 96 Characters ( NEW )

	font = CreateFont(	-24,				// Height Of Font ( NEW )
						  0,				// Width Of Font
						  0,				// Angle Of Escapement
						  0,				// Orientation Angle
						  FW_BOLD,			// Font Weight
						  FALSE,			// Italic
						  FALSE,			// Underline
				          FALSE,			// Strikeout
						  ANSI_CHARSET,		// Character Set Identifier
						  OUT_TT_PRECIS,	// Output Precision
						  CLIP_DEFAULT_PRECIS,		// Clipping Precision
						  ANTIALIASED_QUALITY,		// Output Quality
						  FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
						  "Poor Richard"			// Font Name
					);

	oldfont = (HFONT)SelectObject(*m_hdc, font);		// Selects The Font We Want
	wglUseFontBitmaps(*m_hdc, 32, 96, base);			// Builds 96 Characters Starting At Character 32
	SelectObject(*m_hdc, oldfont);				// Selects The Font We Want
	DeleteObject(font);					// Delete The Font

}

GLvoid GLwindow :: KillFont(GLvoid)						// Delete The Font List
{
 	glDeleteLists(base, 96);				// Delete All 96 Characters ( NEW )
}

GLvoid GLwindow ::Print(const char * fmt, ...)
{
	glPushMatrix();
	// ====== Position the text ==================================================================
	glTranslatef(0.0f,0.0f,-10.0f);				// Move One Unit Into The Screen
	
	// Position The Text On The Screen
	glRasterPos2f(-4.5f, 3.5f);
	// ============================================================================================

	char		text[256];				// Holds Our String
	va_list		ap;					    // Pointer To List Of Argument

	if (fmt == NULL)					// If There's No Text
		return;						    // Do Nothing

	va_start(ap, fmt);					// Parses The String For Variables
	    vsprintf(text, fmt, ap);		// And Converts Symbols To Actual Numbers
	va_end(ap);						    // Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);				// Pushes The Display List Bits		( NEW )
	glListBase(base - 32);					// Sets The Base Character to 32	( NEW )

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text	( NEW )
	glPopAttrib();

	glPopMatrix();
}
int GLwindow::Initialise(HINSTANCE hInstance, int nCmdShow, HDC hdc)
{
   m_hdc = &hdc;
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, m_width, m_height, NULL, NULL, hInstance, NULL);


   if (!hWnd)
   {
      return FALSE;
   }
	

   int PixelFormat=ChoosePixelFormat(GetDC(hWnd),m_pfd);
	SetPixelFormat(GetDC(hWnd),PixelFormat,m_pfd);
    HGLRC glrc = wglCreateContext( GetDC(hWnd) );
    wglMakeCurrent(GetDC(hWnd),glrc);

   ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);



	glShadeModel(GL_SMOOTH);		// Enable Smooth Shading
    glClearColor(0.2f, 0.4f, 1.0f, 0.0f);	
    glClearDepth(1.0f);			// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Testing To Do

    // Really Nice Perspective Calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
    glLoadIdentity();		// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)m_width/(GLfloat)m_height,0.1f,1000.0f);

    glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
    glLoadIdentity();		// Reset The Modelview Matrix

	// now build the font
	BuildFont();

    return TRUE;			 	// Initialization Went OK
}
int GLwindow::DrawScene(GLvoid)
{
	//Clear Screen And Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Reset The Current Modelview Matrix
    glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

    return TRUE;			// Keep Going
}

void GLwindow::GLResize(int width,int height)
{
	if(height==0){height=1;} // prevent divide by zero

	glViewport(0,0,width,height);  // set new viewport size

	glMatrixMode(GL_PROJECTION); // reset projection matrix
	glLoadIdentity();

	//calculate new prespective and aspect ratio
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,1000.0f);

	glMatrixMode(GL_MODELVIEW); // reset modelview matrix
	glLoadIdentity();
}

void GLwindow::CreatePixelFormatDescriptor()
{
	//     Tell the window how we want things to be..
    static	PIXELFORMATDESCRIPTOR pfd=
    {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,			// Version Number
	PFD_DRAW_TO_WINDOW |	// Format Must Support Window
	PFD_SUPPORT_OPENGL |	// Format Must Support OpenGL
	PFD_DOUBLEBUFFER,		// Double Buffering
	PFD_TYPE_RGBA,		// Request An RGBA Format
	32,			// Select Our Color Depth
	0, 0, 0, 0, 0, 0,		// Color Bits Ignored
	0,			// No Alpha Buffer
	0,			// Shift Bit Ignored
	0,			// No Accumulation Buffer
	0, 0, 0, 0,		// Accumulation Bits Ignored
	16,			// 16Bit Z-Buffer (Depth Buffer)  
	0,			// No Stencil Buffer
	0,			// No Auxiliary Buffer
	PFD_MAIN_PLANE,		// Main Drawing Layer
	0,			// Reserved
	0, 0, 0			// Layer Masks Ignored	
    };
	

	m_pfd = new PIXELFORMATDESCRIPTOR(pfd);
}

PIXELFORMATDESCRIPTOR GLwindow::GetPFD()
{
	return *m_pfd;
}

