#include <windows.h>
#include <iostream> //Libreria para entrada y salida de datos
#include <fstream> //Libreria para manejar archivos
#include <string>
#include <thread>   // Necesario para la función sleep_for
#include <chrono>
#include <cstdlib>//Libreria para utilizar tipos de datos string
#include "Librerias/Dibujar bitmaps/gdipload.h"
#include "Librerias/Musica/libzplay.h"

#define Tick 100
#define Timer 100

using namespace libZPlay;//Se declara que se usara la libreria de libZPlay para reproductor de musica
using namespace std; //Utiliza el espacio de nombres de la libreria STD de c++

struct Musica
{
	string Dir;
	string Nombre;
};
Musica Cancion[3];

struct FrameArray{
	int x,y;
	int ancho,alto;
};//Similar a la estructura rect de winapi

struct Input
{
	enum Keys
	{
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
}input;
bool KEYS[256];
enum Animaciones_Enemigo{

Running_E

};
enum Animaciones
{
	Idle,
	Dash,
	Jump,
	Walk,
};
enum Stages
{
	Inicio,
	FirstStage,
	GameOver
};
enum Frames {
	Frame0,
	Frame1,
	Frame2,
	Frame3,
	Frame4,
	Frame5,
	Frame6,
	Frame7,
	Frame8,
	Frame9,
	Frame10,
	Frame11,
	Frame12,
	Frame13,
	Frame14,
	Frame15,
	Frame16,
	Frame17,
	Frame18,
	Frame19,
	Frame20
};
struct DatosPersonaje{
	//Ruta donde esta nuestra imagen
	WCHAR BmpW[MAX_PATH] = L"Recursos/Scott_Pilgrim.bmp";
	miBitmap HojaSprite;

	//Posicion relativa de nuestro Sprite en la ventana
	int XCurrentCoordDraw;
	int YCurrentCoordDraw;

	//Dimensiones en pixeles que se requieren para dibujarlo en la ventana

	int Animaciones = 4;

	int idAnimacionIdle = 0;
	int FramesAnimacionIdle = 6;

	int idAnimacionCorrer = 1;
	int FramesAnimacionCorrer = 8;

	int idAnimacionJump = 2;
	int FramesAnimacionJump= 13    ;

	int idAnimacionWalk = 3;
	int FramesAnimacionWalk = 6;

	FrameArray** FrameSpriteArray;

}miPersonaje;
struct DatosStage{
	int Escenarios = 4;
	miBitmap ImagenEscenario1;
	WCHAR Bmp1[MAX_PATH] = L"Recursos/PantallaInicial.bmp"; 
	miBitmap ImagenEscenario2;
	WCHAR Bmp2[MAX_PATH] = L"Recursos/Frozen_Suburbs_0.bmp";
	miBitmap ImagenEscenario3;
	WCHAR Bmp3[MAX_PATH] = L"";
	miBitmap ImagenEscenario4;
	WCHAR Bmp4[MAX_PATH] = L"";
}miStage;

struct DatosEnemigo {

	WCHAR BmpW[MAX_PATH] = L"Recursos/Enemigo.bmp";
	miBitmap HojaSprite;

	int XCurrentCoordDraw;
	int YCurrentCoordDraw;

	int Animaciones_Enemigo = 2;

	int idAnimacionRunning = 0;
	int FramesAnimacionRunning = 6;

	FrameArray** FrameSpriteArray; 

}miEnemigo; 
//Variables Globales
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
const int BPP = 4;
int Mov_fondo = 0;
//#2EFF82
const unsigned int TRANSPARENCY = 0xFF2EFF82, TRANSPARENCY_E = 0xFF99D9EA;
ZPlay* player = CreateZPlay();//Generamos un objeto puntero para nuestro reproductor
TStreamStatus status;
bool pausa = false;
int* ptrBufferPixelsWindow;
int AnimacionActual, Animacion_E;
int FrameActual = 0;
int DelayFrameAnimation=0;
bool pantallaInicial = true;

const float sinlimite = 0;
const float fps1 = 1000 / 1;
const float fps24 = 1000 / 24;
const float fps30 = 1000 / 30;
const float fps45 = 1000 / 45;
const float fps60 = 1000 / 60;
const float fps120 = 1000 / 120;
float fps = fps30;
float timer = 0.0f;


//Definicion de funciones
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LimpiarFondo(int *ptrBuffer, unsigned int color, int area);
void TranScaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY, const unsigned int TRANSPARENCY, double limiteX);
void MainRender(HWND hWnd);
void Init();
void KeysEvents();
void Movimiento_Enemigo();
void ReproductorPausa();
void ReproductorReproduce();
void ReproductorInicializaYReproduce();
void ReproductorCambiarCancionYReproduce(int);
void CargaFramesSprite();
void CargaFramesSprite_E();

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow)
{
	WNDCLASSEX wc;									// Windows Class Structure
	HWND hWnd;
	MSG msg;

	TCHAR szAppName[] = TEXT("MyWinAPIApp");		
	TCHAR szAppTitle[] = TEXT("ScottPilgrim 'The GAME'");

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance	
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);			// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu	
	wc.lpszClassName	= szAppName;							// Set The Class Name
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,
			L"Fallo al registrar clase (Failed To Register The Window Class).",
			L"ERROR",
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	hWnd = CreateWindowEx(	
		WS_EX_CLIENTEDGE | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,	// Extended Style For The Window
		szAppName,							// Class Name
		szAppTitle,							// Window Title
		WS_OVERLAPPEDWINDOW |				// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		ANCHO_VENTANA,						// Calculate Window Width
		ALTO_VENTANA,						// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL);								// Pass this class To WM_CREATE								

	if(hWnd == NULL) {
		MessageBox(NULL, 
			L"Error al crear ventana (Window Creation Error).", 
			L"ERROR", 
			MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}
		
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	Init();
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);

	SetTimer(hWnd, Timer, Tick, NULL);
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)									// Check For Windows Messages
	{
		case WM_CREATE:
		break;
		case WM_KEYDOWN:
		{
			KEYS[wParam] = true;
		}
			break;
		case WM_KEYUP:
		{
			KEYS[wParam] = false;
			switch (wParam) {
				case VK_F1://F1
				{
					ReproductorPausa();
				}
				return 0;
				case VK_F2://F2
				{
					ReproductorCambiarCancionYReproduce(0);
				}
				return 0;
				case VK_F3://F3
				{
					ReproductorCambiarCancionYReproduce(1);
				}
				return 0;
				case VK_F4://F4
				{
					ReproductorCambiarCancionYReproduce(2);
				}
				return 0;
			}
		}
			break;
		case WM_TIMER:
			if(wParam == Tick)
			{
				MainRender(hWnd);
				player->GetStatus(&status);
				if (!pausa && status.fPlay == 0){
					ReproductorReproduce();
				}
			}
			break;
		case WM_PAINT:
			{
				HDC hdc; 
				PAINTSTRUCT ps;
				hdc = BeginPaint(hWnd, &ps);
				HBITMAP h_CMC = CreateBitmap(ANCHO_VENTANA, ALTO_VENTANA, 1, 32, ptrBufferPixelsWindow);
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, h_CMC);//Creacion de marco para proyecto

				BitBlt(hdc, 0, 0, ANCHO_VENTANA, ALTO_VENTANA, hdcMem, 0, 0, SRCCOPY);

				DeleteObject(h_CMC);
				//SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
				//DeleteObject(hbmOld);
			}
			break;		
		/*case WM_KEYDOWN:							
			{
				KEYS[ wParam ] = true;
			}
			break;*/
		case WM_CLOSE: 
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY: //Send A Quit Message
			{
				KillTimer(hWnd, Tick);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
/* Inicializacion de variables y reserva de memoria.
	*/
void Init()
{
	ReproductorInicializaYReproduce();
	if (!pantallaInicial)
	{
		//Empezamos a reproducir musica
		
		ReproductorCambiarCancionYReproduce(1); 
	}
	else
	{
		srand(time(0)); 
		int M = rand()%100 ; 
		if (M % 2 == 0) { 
			ReproductorCambiarCancionYReproduce(0);
		}else		
		ReproductorCambiarCancionYReproduce(2);
	}

	CargaFramesSprite();
	CargaFramesSprite_E();

	//Cargamos imagen bitmap de nuestro escenario
	miStage.ImagenEscenario1 = gdipLoad(miStage.Bmp1);
	miStage.ImagenEscenario2 = gdipLoad(miStage.Bmp2);

	//Definimos un puntero del total de pixeles que tiene nuestra ventana
	ptrBufferPixelsWindow = new int[ANCHO_VENTANA * ALTO_VENTANA];

	// Definimos la animacion inicial
	AnimacionActual = Idle;
	Animacion_E = Running_E; 
}

void CargaFramesSprite(){
	//Cargamos primero las hojas de sprite
	miPersonaje.HojaSprite = gdipLoad(miPersonaje.BmpW);

	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos
	miPersonaje.XCurrentCoordDraw = 100;
	miPersonaje.YCurrentCoordDraw = 300;
	//Definiendo los tamaños de nuestro sprite para renderizarlo en la ventana


	//Definiendo las dimenciones en base al # de Animaciones
	miPersonaje.FrameSpriteArray = new FrameArray * [miPersonaje.Animaciones];
	//Definiendo la cantidad de Frames en base a cada Animacion
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle] = new FrameArray[miPersonaje.FramesAnimacionIdle];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer] = new FrameArray[miPersonaje.FramesAnimacionCorrer];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump] = new FrameArray[miPersonaje.FramesAnimacionJump];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk] = new FrameArray[miPersonaje.FramesAnimacionWalk]; 


	//Cargando Frames a nuestro arreglo del sprite
	// ------ - Animacion 1 Idle -------- //
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].x = 5; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].y = 9;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].x = 44; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].y = 9;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].x = 84; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].y = 9;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].x = 123; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].y = 9;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].x = 162; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].y = 9;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].x = 200; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].y = 9;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].alto = 62;

	// ------ - Animacion 2 Dash - ------ //
	
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].x = 9; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].alto = 62;
	
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].x = 50; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].ancho = 42; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].x = 96; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].ancho = 52; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].x = 152; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].ancho = 45; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].x = 201; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].x = 243; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].ancho = 40; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].x = 287; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].ancho = 49; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].alto = 62;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].x = 340; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].ancho = 48; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].alto = 62;	
	
	
	// ------ - Animacion 3 Saltando - ------ //
	//1
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].x = 9; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].ancho = 53; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].alto = 77;
	//2
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].x = 62; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].ancho = 42; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].alto = 77;
	//3
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].x = 104; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].ancho = 42; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].alto = 77;
	//4
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].x = 146; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].ancho = 42; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].alto = 77;
	//5
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].x = 188; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].ancho = 43; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].alto = 77;
	//6
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].x = 231; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].ancho = 44; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].alto = 77;
	//7
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].x = 275; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].ancho = 43; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].alto = 77;
	//8
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].x = 318; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].ancho = 42; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].alto = 77;
	//9
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].x = 360; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].alto = 77;
	//10
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].x = 398; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].ancho = 40; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].alto = 77;
	//11
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].x = 438; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].alto = 77;
	//12
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].x = 476; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].alto = 77;
	//13
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].x = 515; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].y = 215;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].ancho = 51; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].alto = 77;
	
	// ------ - Animacion 2 caminando -------- //
	
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].x = 7; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].y = 145;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].x = 44; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].y = 145;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].x = 82; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].y = 145;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].x = 119; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].y = 145;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].ancho = 36; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].x = 155; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].y = 145;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].x = 192; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].y = 145;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].alto = 64;

}

void CargaFramesSprite_E() {
	//Cargamos primero las hojas de sprite
	miEnemigo.HojaSprite = gdipLoad(miEnemigo.BmpW);

	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos
	miEnemigo.XCurrentCoordDraw = 600; //max 600 min 0
	miEnemigo.YCurrentCoordDraw = 300;
	//Definiendo los tamaños de nuestro sprite para renderizarlo en la ventana

	//Definiendo las dimenciones en base al # de Animaciones
	miEnemigo.FrameSpriteArray = new FrameArray * [miEnemigo.Animaciones_Enemigo];
	//Definiendo la cantidad de Frames en base a cada Animacion
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning] = new FrameArray[miEnemigo.FramesAnimacionRunning]; 

	// ------ - Animacion 1 Corriendo - ------ //
	//0
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].x = 607; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].y = 504;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].ancho = 78; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].alto = 180; 
	//1
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].x = 527; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].y = 504;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].ancho = 80; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].alto = 180;
	//3
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].x = 448; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].y = 504;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].ancho = 79; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].alto = 180;
	//4
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].x = 371; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].y = 504;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].ancho = 80; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].alto = 180;
	//5
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].x = 291; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].y = 504;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].ancho = 80; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].alto = 180;
	//6
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].x = 212; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].y = 504;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].ancho = 79; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].alto = 180;

}
/* Funcion principal. Encargada de hacer el redibujado en pantalla cada intervalo (o "Tick") del timer que se haya creado.
	@param hWnd. Manejador de la ventana.
	*/
void DibujaPixeles()
{
	if (pantallaInicial)
	{
		//Dibujamos el fondo
		TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario1.pixeles),
			0, 0,//Iniciamos a dibujar en la ventana en 0,0
			0, 0,//Indicamos cuales son las coordenadas para dibujar desde nuestra imagen; iniciamos en 0,0 desde nuestro escenario
			ANCHO_VENTANA, ALTO_VENTANA,//Definimos cuantos pixeles dibujaremos de nuestra imagen a la pantalla
			800, miStage.ImagenEscenario1.ancho,
			1, 1, TRANSPARENCY, 1);//Si ponemos un numero mayor a 1 estaremos repitiendo 2 veces la linea de pixeles en X o en Y

	}
	else
	{
		//Dibujamos el fondo
		TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario2.pixeles),
			0, 0,//Iniciamos a dibujar en la ventana en 0,0
			Mov_fondo,0,//Indicamos cuales son las coordenadas para dibujar desde nuestra imagen; iniciamos en 0,0 desde nuestro escenario 
			10596, 283,//Definimos cuantos pixeles dibujaremos de nuestra imagen a la pantalla
			800, miStage.ImagenEscenario2.ancho,
			2, 2, TRANSPARENCY, 1);//Si ponemos un numero mayor a 1 estaremos repitiendo 2 veces la linea de pixeles en X o en Y
		//Dibujamos a nuestro personaje

		TranScaleblt(ptrBufferPixelsWindow, (miPersonaje.HojaSprite.pixeles), 
			miPersonaje.XCurrentCoordDraw, miPersonaje.YCurrentCoordDraw,
			miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].x, miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].y,
			miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].ancho, miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].alto,
			800, miPersonaje.HojaSprite.ancho, 
			3, 3, TRANSPARENCY, 1);
		//Dibujamos al enemigo
		TranScaleblt(ptrBufferPixelsWindow, (miEnemigo.HojaSprite.pixeles),
			miEnemigo.XCurrentCoordDraw, miEnemigo.YCurrentCoordDraw,
			miEnemigo.FrameSpriteArray[Animacion_E][FrameActual].x, miEnemigo.FrameSpriteArray[Animacion_E][FrameActual].y,
			miEnemigo.FrameSpriteArray[Animacion_E][FrameActual].ancho, miEnemigo.FrameSpriteArray[Animacion_E][FrameActual].alto,
			800, miEnemigo.HojaSprite.ancho,
			2, 2, TRANSPARENCY_E, 1);
	}


}
void ActualizaAnimacion(HWND hWnd){
	switch (AnimacionActual){

	case Idle:
		if (FrameActual > 5) FrameActual = 0;
		if (Tick % Tick == 0 && FrameActual == 0)//125
		{
			DelayFrameAnimation++;
		}
		else if (Tick % Tick == 0 && FrameActual == 2)
		{
			DelayFrameAnimation += 3;
		}
		if (DelayFrameAnimation % 18 == 0)
		{
			FrameActual++;
			if (FrameActual > 5 ) FrameActual = 0;
		}

		break;

	case Dash: {
		/*
if (Tick % Tick == 0 && FrameActual == 0)//125
{
	DelayFrameAnimation++;
}
else if (Tick % Tick == 0 && FrameActual == 2)
{
	DelayFrameAnimation += 3;
}
if (DelayFrameAnimation % 18 == 0)
{

}*/

		FrameActual++;
		if (FrameActual > 7) FrameActual = 0;
	}break;
	case Jump: {
		FrameActual++;
		if (FrameActual > 12) FrameActual = 0;
	}break;
	case Walk: {
		FrameActual++;
		if (FrameActual > 5) FrameActual = 0;
	}break;

	}
	switch (Animacion_E){
	case Running_E:
		FrameActual++;
		if (FrameActual > 5) FrameActual = 0;
		break;
	default:
		break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}
void MainRender(HWND hWnd) 
{
	LimpiarFondo(ptrBufferPixelsWindow, 0xFFFFFFFF, (ANCHO_VENTANA * ALTO_VENTANA));
	KeysEvents();
	Movimiento_Enemigo();
	DibujaPixeles();
	ActualizaAnimacion(hWnd);
}

void Frame(float deltatime) {

}

bool W_Pressed = false,A_Pressed = false,S_Pressed = false, D_Pressed = false, SPACE_Pressed = false;
void Movimiento_Enemigo() {

	//Movimeinto en X
	//for (int i = 600; i > 0; i--) {
		
		//miEnemigo.XCurrentCoordDraw += i;
		//this_thread::sleep_for(chrono::milliseconds(10000)); 
	//}



}
void KeysEvents()
{
	if (KEYS[input.Enter] && pantallaInicial == true)
	{
		pantallaInicial = false;
		Init();
	}
	if (KEYS[input.Backspace])
	{
		pantallaInicial = true;
		ReproductorPausa();
		Init();
	}
	if (!pantallaInicial)
	{
		
		if (KEYS[input.W] || KEYS[input.Up])
		{
			miPersonaje.YCurrentCoordDraw -= 10;
			AnimacionActual = Walk;
			W_Pressed = true;
		}
		else if(W_Pressed)
		{
			W_Pressed = false;
			AnimacionActual = Idle;
			FrameActual = 0;

		}
		if (KEYS[input.D] || KEYS[input.Right])
		{
			Mov_fondo += 12;
			AnimacionActual = Dash;
			D_Pressed = true;
		}
		else if(D_Pressed)
		{
			D_Pressed = false;
			AnimacionActual = Idle;
			FrameActual = 0; 
			
		}
		if (KEYS[input.S] || KEYS[input.Down])
		{
			miPersonaje.YCurrentCoordDraw += 10;
			AnimacionActual = Walk; 
			S_Pressed = true;
		}else if (S_Pressed)
		{
			S_Pressed = false;
			AnimacionActual = Idle;
			FrameActual = 0;

		}
		if (KEYS[input.A] || KEYS[input.Left])
		{
			Mov_fondo -= 7; 
			AnimacionActual = Walk;  
			A_Pressed = true;
		}else if (A_Pressed )
		{
			A_Pressed = false;
			AnimacionActual = Idle;
			FrameActual = 0;

		}
		if (KEYS[input.Space]&& KEYS[input.D]) {
			//for (int J = 0; J < 7; J = J++) {
			//	miPersonaje.YCurrentCoordDraw -= J;
			//}
			miPersonaje.YCurrentCoordDraw -= 7;
			miPersonaje.XCurrentCoordDraw += 21;
			AnimacionActual = Jump; 
			SPACE_Pressed = true;							
		}
		else if(SPACE_Pressed){ 
			SPACE_Pressed = false;
			AnimacionActual = Idle;
			FrameActual = 0;
		}
		/*
		if (KEYS[input.Space] && KEYS[input.A]) {
			//for (int J = 0; J < 7; J = J++) {
			//	miPersonaje.YCurrentCoordDraw -= J;
			//}
			miPersonaje.YCurrentCoordDraw -= 7; 
			miPersonaje.XCurrentCoordDraw -= 21; 
			AnimacionActual = Jump; 
			SPACE_Pressed = true; 
		}
		else if (SPACE_Pressed) {
			SPACE_Pressed = false;
			AnimacionActual = Idle;
			FrameActual = 0;
		}
		*/
		
	}
}
#pragma region LENS_CODE
/* Pinta el fondo de la ventana de acuerdo al color especificado.
	@param *ptrBuffer.	Puntero al area de memoria reservada para el proceso de dibujado.
	@param color.		Color expresado en formato hexadecimal.
	@param area.		Area de la ventana.
	*/
void LimpiarFondo(int *ptrBuffer, unsigned int colorFondo, int area) 
{
	__asm {
			mov edi, ptrBuffer
			mov ecx, area
			mov eax, colorFondo

			draw: 	
				mov [edi], eax
				add edi, BPP
			loop draw

			rep stosd
	}
}

/* Funcion que pinta una figura rectangular en pantalla.
	@param *ptrBuffer.	Puntero al area de memoria reservada para el dibujado.
	@param color.		Color de la figura expresado en formato hexadecimal.
	@param anchoWnd.	Ancho total de la ventana.
	@param altoWnd.		Alto total de la ventana.
	@param dmnFigura.	Especifica las dimensiones de la figura en relacion con la ventana.
	@param posFigura.	Posiciona la figura en la ventana.
	*/

void TranScaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY, const unsigned int TRANSPARENCY, double limiteX) {
	//blt = block transfer, transferencia de bloque de imagen
	int bytesporlineafondo = anchodefondo * 4;
	int bytesporlineaorigen = anchodeorigen * 4;
	int bytesporlineaimagen = ancho * 4;
	if ((inicioXDestino + (ancho * 2) > 0 && inicioYDestino + alto > 0) && (inicioYDestino < 600 && inicioXDestino < 800)) { // Verificamos si el sprite esta dentro de los limites del buffer

		if (inicioXDestino < 0) { // verificamos si la esquina superior izquierda esta fueda del buffer, de ser asi, hacemos mas peque;o el sprite
			ancho += inicioXDestino;
			bytesporlineaimagen = ancho * 4;
			inicioXDestino = 0;
		}

		if ((inicioXDestino + ancho + limiteX) > 800) { // verificamos si la esquina inferior derecha esta fueda del buffer, de ser asi, hacemos mas peque;o el sprite
			ancho = (800 - (inicioXDestino + (limiteX * 2)));
			bytesporlineaimagen = ancho * 4;
			inicioXDestino = 800 - ancho;
		}

		if (inicioYDestino < 0) { // verificamos si la esquina superior izquierda esta fueda del buffer, de ser asi, hacemos mas peque;o el sprite
			alto += inicioYDestino;
			inicioYDestino = 0;
		}
		if ((inicioYDestino + alto) > 600) { // verificamos si la esquina inferior izquierda esta fueda del buffer, de ser asi, hacemos mas peque;o el sprite
			alto = (600 - inicioYDestino);
			inicioYDestino = 600 - alto;
		}
		if (ancho < 1) ancho = 1;
	}

	__asm {

		mov edi, punteroDestino //movemos la direccion del bitmap a edi, para poder escribir en el
		//Conseguimos el pixel inicial donde empezaremos a dibujar
		mov eax, inicioYDestino
		mul bytesporlineafondo //inicioY * bytesporlineafondo, asi iniciamos en la linea donde queremos 
		mov ebx, eax //ebx contendra el resultado anterior
		mov eax, 4
		mul inicioXDestino //inicioX*4, para asi encontrar la columnda donde queremos empezar a dibujar
		add eax, ebx //posicion de columna + posicion de linea
		add edi, eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
		mov esi, punteroOrigen //movemos la direccion de la imagen a dibujar a esi, para poder escribir de ella
		//Conseguimos el pixel inicial DEL CUAL empezaremos a dibujar
		mov eax, inicioYOrigen
		mul bytesporlineaorigen //inicioY * bytesporlineaorigen, asi iniciamos en la linea donde queremos 
		mov ebx, eax //ebx contendra el resultado anterior
		mov eax, 4
		mul inicioXOrigen //inicioX*4, para asi encontrar la columnda de donde queremos empezar a leer
		add eax, ebx //posicion de columna + posicion de linea
		add esi, eax //Sumamos el desplazamiento anterior al inicio de nuestra imagen para empezar a trabajar en la posicion deseada
		mov eax, TRANSPARENCY

		mov ecx, alto //movemos a ecx la cantidad de lineas que dibujaremos

		lazollenarY :
		push ecx
			mov ecx, escalaY
			escaladoY :
		push ecx //guardamos el valor anterior de ecx, porque lo reemplazaremos en un nuevo ciclo
			mov ecx, ancho //la cantidad de columnas que dibujaremos
			lazollenarX :
		//movsd //mueve un pixel de la direccion apuntada por esi a la apuntada por edi, e incrementa esi y edi por 4
		push ecx
			mov ecx, escalaX
			escaladoX :
		cmp eax, [esi]
			je nodibujar
			mov edx, [esi]
			mov[edi], edx
			nodibujar :
		add edi, 4
			loop escaladoX
			add esi, 4
			pop ecx
			loop lazollenarX
			add edi, bytesporlineafondo //le sumamos la cantidad de bytes de la linea del fondo para pasar a la siguiente linea
			push eax
			mov eax, bytesporlineaimagen
			mul escalaX
			sub edi, eax //y retrocedemos una cantidad igual al a su ancho para dibujar desde la posicion X inicial y que no quede escalonado
			pop eax
			sub esi, bytesporlineaimagen
			pop ecx
			loop escaladoY
			//Lo mismo para esi
			add esi, bytesporlineaorigen
			pop ecx //recuperamos el valor del contador del ciclo exterior
			loop lazollenarY
	}
}
#pragma endregion

void ReproductorInicializaYReproduce() {
	Cancion[0].Nombre = "Inicio";
	Cancion[0].Dir = "Recursos/Audio/Intro.mp3";
	Cancion[1].Nombre = "Nivel1";
	Cancion[1].Dir = "Recursos/Audio/Nivel1.mp3";
	Cancion[2].Nombre = "Inicio_1";
	Cancion[2].Dir = "Recursos/Audio/Character_Select.mp3";
	//Cancion[2].Nombre = "Derrota";
	//Cancion[2].Dir = "Recursos/Mega Man Zero Ost Intermission.mp3";
	ifstream inputFile(Cancion[0].Dir.c_str());

	if (!inputFile.good()) 
		printf("No file found");
	else 
		player->OpenFile(Cancion[0].Dir.c_str(), sfAutodetect);
	player->SetMasterVolume(50, 50);// Sonido tipo estereo Left and Right - Volumen de 0 - 100
	player->Play();
}

void ReproductorCambiarCancionYReproduce(int NumeroCancionAeproducir) {
	player->Stop();
	ifstream inputFile(Cancion[NumeroCancionAeproducir].Dir.c_str());

	if (!inputFile.good())
		printf("No file found");
	else
		player->OpenFile(Cancion[NumeroCancionAeproducir].Dir.c_str(), sfAutodetect);
	player->Play();
}

void ReproductorPausa() {
	player->Pause();
	pausa = true;
}

void ReproductorReproduce() {
	if(pausa)
		player->Resume();
	else 
		player->Play();
	pausa = false;
}