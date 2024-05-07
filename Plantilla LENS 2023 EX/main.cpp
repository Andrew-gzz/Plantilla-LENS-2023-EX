#include <windows.h>
#include <iostream> //Libreria para entrada y salida de datos
#include <fstream> //Libreria para manejar archivos
#include <sstream>
#include <iomanip>  
#include <string>
#include <vector>
#include <thread>   // Necesario para la función sleep_for
#include <chrono>
#include <ctime>  
#include <cstdlib>//Libreria para utilizar tipos de datos string
#include "Librerias/Dibujar bitmaps/gdipload.h"
#include "Librerias/Musica/libzplay.h"
/*
Promedio de Pixeles de Scott Aproximadamente Alto:70 Ancho: 40
Tamaño en Pixeles de la pantalla 800x600 pixeles

*/

#define Tick 100
#define Timer 100

using namespace libZPlay;//Se declara que se usara la libreria de libZPlay para reproductor de musica
using namespace std; //Utiliza el espacio de nombres de la libreria STD de c++
//Volumen de la musica
struct Musica
{
	string Dir;
	string Nombre;
};
Musica Cancion[4];

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
	Dance,
	Death
};
enum Animaciones_Coin
{
	Coin1,
	Coin2,
	Coin3
};
enum Animaciones_Piggy {

	Piggy,
	Piggy_used

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
//Recursos
struct DatosPersonaje{
	//Ruta donde esta nuestra imagen
	WCHAR BmpW[MAX_PATH] = L"Recursos/Scott_Pilgrim.bmp";
	miBitmap HojaSprite;

	//Posicion relativa de nuestro Sprite en la ventana
	int XCurrentCoordDraw;
	int YCurrentCoordDraw;

	//Dimensiones en pixeles que se requieren para dibujarlo en la ventana

	int Animaciones = 6;

	int idAnimacionIdle = 0;
	int FramesAnimacionIdle = 6;

	int idAnimacionCorrer = 1;
	int FramesAnimacionCorrer = 8;

	int idAnimacionJump = 2;
	int FramesAnimacionJump= 13    ;

	int idAnimacionWalk = 3;
	int FramesAnimacionWalk = 6;

	int idAnimacionDance = 4;
	int FramesAnimacionDance = 15;

	int idAnimacionDeath = 5;
	int FramesAnimacionDeath = 14;

	FrameArray** FrameSpriteArray;

}miPersonaje;
struct DatosStage{
	int Escenarios = 4;
	miBitmap ImagenEscenario1;
	WCHAR Bmp1[MAX_PATH] = L"Recursos/PantallaInicial.bmp"; 
	miBitmap ImagenEscenario2;
	WCHAR Bmp2[MAX_PATH] = L"Recursos/Frozen_Suburbs_0.bmp";
	miBitmap ImagenEscenario3;
	WCHAR Bmp3[MAX_PATH] = L"Recursos/Scott_kiss.bmp";
	miBitmap ImagenEscenario4;
	WCHAR Bmp4[MAX_PATH] = L"";
}miStage;
struct Recursos {
	int objeto = 5;
	miBitmap Title1;
	WCHAR Bmp1[MAX_PATH] = L"Recursos/Scott_Pilgrim_Title.bmp";
	miBitmap Title2;
	WCHAR Bmp2[MAX_PATH] = L"Recursos/Press_Enter.bmp";
	miBitmap Title3;
	WCHAR Bmp3[MAX_PATH] = L"Recursos/Level_Complete.bmp";
	miBitmap Title4;
	WCHAR Bmp4[MAX_PATH] = L"Recursos/Try_Again.bmp";
	miBitmap Title5;
	WCHAR Bmp5[MAX_PATH] = L"Recursos/Barra_de_vida.bmp";

}misRecursos;
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
struct DatosCoins{
	WCHAR BmpW[MAX_PATH] = L"Recursos/Coins.bmp";
	miBitmap HojaSprite;

	int XCurrentCoordDraw;
	int YCurrentCoordDraw;

	int Animaciones_Coin = 4;

	int idAnimacionCoin1= 0;
	int FramesAnimacionCoin1= 6;

	int idAnimacionCoin2 = 1;
	int FramesAnimacionCoin2 = 6;

	int idAnimacionCoin3 = 2;
	int FramesAnimacionCoin3 = 6;

	FrameArray** FrameSpriteArray;
}miMoneda;
struct DatosPowerUp {
	WCHAR BmpW[MAX_PATH] = L"Recursos/Piggy_Bank.bmp";
	miBitmap HojaSprite;

	int XCurrentCoordDraw;
	int YCurrentCoordDraw;

	int Animaciones_PowerUp = 3;

	int idAnimacionPowerUp = 0;
	int FramesAnimacionPowerUp = 4;
	int idAnimacionPowerUp1 = 1;
	int FramesAnimacionPowerUp1 = 5;

	FrameArray** FrameSpriteArray;
}misPowerUp;
//Variables Globales
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
const int BPP = 4;
int Mov_fondo = 0;
//#2EFF82
const unsigned int TRANSPARENCY = 0xFF2EFF82, TRANSPARENCY_E = 0xFF99D9EA, TRANSPARENCY_P = 0xFF59D9DB;
const unsigned int Verde = 0xFF00ED20, Rojo = 0xFFED1C24, Blanco = 0xFFFFFFFF;
ZPlay* player = CreateZPlay();//Generamos un objeto puntero para nuestro reproductor
TStreamStatus status;
bool pausa = false;
int* ptrBufferPixelsWindow;
//Animacion de scott, enemigo, monedas, piggy powerup
int AnimacionActual, Animacion_E, Animacion_C, Animacion_P;
int FrameActual = 0, Max_Frame = 0;
//Animaciones Actuales//
int E_ActualFrame = 0;
int E_Velocidad = 10;
int C_ActualFrame = 0;
int C_Velocidad = 30;
int P_ActualFrame = 0;
int DelayFrameAnimation=0;
bool pantallaInicial = true;
bool pantallaVictoria = false;
bool pantallaNivel = false;
bool Estatus = true; // false = muerto true = vivo
bool Muerte = false;
//Musica
unsigned int i=10, d=10;
bool FinalMusic = false;
//Codigo para el Score
bool ActiveScore = false;
bool A_Coin1 = false;
int BERE = 0;//Para validar cuantos cochinitos agarró
float Extra_Bonus = 1;
float MUL = 0.1;
time_t tiempoInicio; 
int Segundos;
int Monedas = 0, Contador = 0; //Contador sirve para el update de las monedas
float PuntajeT;
//Codigo para PowerUps
bool Once = true;
bool P_Power = false;
int Contador_YIYI = 0;
int F_Velocidad = 15;
int Max_Power_Up = 0;
const float sinlimite = 0;
const float fps1 = 1000 / 1;
const float fps24 = 1000 / 24;
const float fps30 = 1000 / 30;
const float fps45 = 1000 / 45;
const float fps60 = 1000 / 60;
const float fps120 = 1000 / 120;
float fps = fps30;
float timer = 0.0f;
bool W_Pressed = false, A_Pressed = false, S_Pressed = false, D_Pressed = false, SPACE_Pressed = false, END_GAME = false, F_Pressed = false, M_Pressed = false, G_Pressed;

//Definicion de funciones
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LimpiarFondo(int *ptrBuffer, unsigned int color, int area);
void TranScaleblt(int* punteroDestino, int* punteroOrigen, int inicioXDestino, int inicioYDestino, int inicioXOrigen, int inicioYOrigen, int ancho, int alto, int anchodefondo, int anchodeorigen, int escalaX, int escalaY, const unsigned int TRANSPARENCY, double limiteX);
void DibujaHitbox(int* ptrBuffer,unsigned int color, int anchoWnd, int altoWnd, int HitboxX, int HitboxY, int HitboxAncho, int HitboxAlto, int escalaX, int escalaY);
void MainRender(HWND hWnd);
void Init();
void KeysEvents();
bool DetectaColisiones();
bool Collbox_Piggy();
void Puntaje(int);
void ReproductorPausa();
void ReproductorReproduce();
void ReproductorInicializaYReproduce();
void ReproductorCambiarCancionYReproduce(int);
void CargaFramesSprite();
void CargaFramesSprite_E();
void CargaFramesSprites_C();
void CargarFramesPiggy();
void Reset_All();
int Tiempo(DWORD tiempoInicio, DWORD tiempoFinal); 
//Baile Final
int Bailes = 0;
//Guardado de datos
float Puntuaciones[3];
int numPuntuaciones = 0;
const int maxPuntuaciones = 3;
void ordenarDescendente(float arr[], int n) {
	for (int i = 0; i < n - 1; ++i) {
		for (int j = 0; j < n - i - 1; ++j) {
			if (arr[j] < arr[j + 1]) {
				// Intercambiar elementos si están en el orden incorrecto
				float temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}
void leerDesdeArchivo(float arr[], int n) {
	ifstream archivo("puntuaciones.txt");
	if (archivo.is_open()) {
		for (int i = 0; i < n; ++i) {
			archivo >> arr[i];
			numPuntuaciones++;  
		}
		archivo.close();		
	}	
}
void guardarEnArchivo(const float arr[], int n) {
	ofstream archivo("puntuaciones.txt");
	if (archivo.is_open()) {
		for (int i = 0; i < n; ++i) {
			archivo << fixed << setprecision(2) << arr[i] << endl;
		}
		archivo.close();
		cout << "Los datos se han guardado en el archivo 'puntuaciones.txt' correctamente." << endl;
	}
	else {
		cout << "No se pudo abrir el archivo para escritura." << endl;
	}
}
// Función para calcular el tiempo transcurrido en segundos
int Tiempo(DWORD tiempoInicio, DWORD tiempoFinal) {
	return (int)difftime(tiempoFinal, tiempoInicio); // Convertir de milisegundos a segundos 
}

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow)
{
	WNDCLASSEX wc;									// Windows Class Structure
	HWND hWnd;
	MSG msg;

	TCHAR szAppName[] = TEXT("ScottPilgrim 'The GAME'");		
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
			if(wParam == Tick){
			
				MainRender(hWnd);
				if(pantallaVictoria==false){
				player->GetStatus(&status);
					if (!pausa && status.fPlay == 0) {
						ReproductorReproduce();
					}
				}
				if (pantallaNivel && Mov_fondo < 10110){
					if (Estatus == true) {//Se detiene el fondo si moriste

						if (F_Velocidad > 15) {
							F_Velocidad -= 2;														
						}
				
					Mov_fondo += F_Velocidad;//Suma velocidad
					miMoneda.XCurrentCoordDraw -= C_Velocidad;
				
					misPowerUp.XCurrentCoordDraw -= 40;
													
					if (F_Velocidad>15){					
						miEnemigo.XCurrentCoordDraw = 800;						
					}
					
					miEnemigo.XCurrentCoordDraw -= E_Velocidad;
					}						
				}else if(pantallaNivel){
					AnimacionActual = Dance;
					if (Bailes == 1) {//Cuando llega al 3er baile
						pantallaNivel = false;
						PuntajeT = 0;
						time_t tiempoFinal;
						time(&tiempoFinal);
						Segundos = Tiempo(tiempoInicio, tiempoFinal);
						Puntaje(Segundos);
						pantallaVictoria = true;
						END_GAME = true;
					} 

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
			guardarEnArchivo(Puntuaciones, 3); 
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
	
		if(pantallaInicial){
			srand(time(0)); 
			int M = rand()%100 ; 
			if (M % 2 == 0) { 
				ReproductorCambiarCancionYReproduce(0);
			}else		
			ReproductorCambiarCancionYReproduce(2);
		}
		else if(pantallaNivel){		
			ReproductorCambiarCancionYReproduce(1); 
		}	

	CargarFramesPiggy();
	CargaFramesSprite();
	CargaFramesSprite_E();
	CargaFramesSprites_C();

	//Cargamos imagen bitmap de nuestro escenario
	miStage.ImagenEscenario1 = gdipLoad(miStage.Bmp1);
	miStage.ImagenEscenario2 = gdipLoad(miStage.Bmp2);
	miStage.ImagenEscenario3 = gdipLoad(miStage.Bmp3);
	//Cargamos recurso
	misRecursos.Title1 = gdipLoad(misRecursos.Bmp1);
	misRecursos.Title2 = gdipLoad(misRecursos.Bmp2); 
	misRecursos.Title3 = gdipLoad(misRecursos.Bmp3);
	misRecursos.Title4 = gdipLoad(misRecursos.Bmp4);
	misRecursos.Title5 = gdipLoad(misRecursos.Bmp5);


	//Definimos un puntero del total de pixeles que tiene nuestra ventana
	ptrBufferPixelsWindow = new int[ANCHO_VENTANA * ALTO_VENTANA];

	// Definimos la animacion inicial
	AnimacionActual = Dash;
	Animacion_E = Running_E; 
	Animacion_C = Coin1;
	Animacion_P = Piggy;  
	leerDesdeArchivo(Puntuaciones, maxPuntuaciones); 
}

//Scott
void CargaFramesSprite(){
	//Cargamos primero las hojas de sprite
	miPersonaje.HojaSprite = gdipLoad(miPersonaje.BmpW);

	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos
	miPersonaje.XCurrentCoordDraw = 100;
	miPersonaje.YCurrentCoordDraw = 360;
	//Definiendo los tamaños de nuestro sprite para renderizarlo en la ventana


	//Definiendo las dimenciones en base al # de Animaciones
	miPersonaje.FrameSpriteArray = new FrameArray * [miPersonaje.Animaciones];
	//Definiendo la cantidad de Frames en base a cada Animacion
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle] = new FrameArray[miPersonaje.FramesAnimacionIdle];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer] = new FrameArray[miPersonaje.FramesAnimacionCorrer];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump] = new FrameArray[miPersonaje.FramesAnimacionJump];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk] = new FrameArray[miPersonaje.FramesAnimacionWalk]; 
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance] = new FrameArray[miPersonaje.FramesAnimacionDance];
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath] = new FrameArray[miPersonaje.FramesAnimacionDeath];

	//Cargando Frames a nuestro arreglo del sprite
	// ------ - Animacion 1 Idle -------- // Sprite Corregido
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].x = 5; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].y = 11;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame0].alto = 59;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].x = 44; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].y = 11;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame1].alto = 59;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].x = 84; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].y = 11;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame2].alto = 59;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].x = 123; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].y = 11;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame3].alto = 59;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].x = 162; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].y = 11;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame4].alto = 59;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].x = 200; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].y = 11;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionIdle][Frame5].alto = 59;

	// ------ - Animacion 2 Dash - ------ // Sprite Corregido
	
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].x = 10; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame0].alto = 60;
	
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].x = 50; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].ancho = 43; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame1].alto = 60;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].x = 96; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].ancho = 53; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame2].alto = 60;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].x = 152; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].ancho = 46; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame3].alto = 60;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].x = 201; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame4].alto = 60;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].x = 243; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].ancho = 41; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame5].alto = 60;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].x = 287; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].ancho = 50; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame6].alto = 60;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].x = 340; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].y = 77;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].ancho = 49; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionCorrer][Frame7].alto = 60;	
	
	
	// ------ - Animacion 3 Saltando - ------ // Sprite Corregido
	//1
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].x = 13; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].y = 229;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].ancho = 47; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame0].alto = 60;
	//2
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].x = 63; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame1].alto = 64;
	//3
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].x = 105; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame2].alto = 64;
	//4
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].x = 147; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame3].alto = 64;
	//5
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].x = 190; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame4].alto = 58;
	//6
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].x = 232; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].ancho = 41; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame5].alto = 54;
	//7
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].x = 276; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].ancho = 41; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame6].alto = 51;
	//8
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].x = 320; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame7].alto = 53;
	//9
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].x = 362; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].ancho = 34; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame8].alto = 60;
	//10
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].x = 400; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].ancho = 36; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame9].alto = 67;
	//11
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].x = 439; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].ancho = 36; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame10].alto = 67;
	//12
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].x = 478; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].y = 217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].ancho = 36; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame11].alto = 67;
	//13
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].x = 517; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].y = 229;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].ancho = 49; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionJump][Frame12].alto = 60;
	
	// ------ - Animacion 2 caminando -------- //Sprite Corregido
	
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].x = 7; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].y = 146;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame0].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].x = 44; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].y = 146;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame1].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].x = 82; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].y = 146;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame2].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].x = 119; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].y = 146;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].ancho = 36; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame3].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].x = 155; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].y = 146;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame4].alto = 64;

	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].x = 192; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].y = 146;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].ancho = 37; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionWalk][Frame5].alto = 64;

	// ------ - Animacion 4 Bailando - ------ //
	//1
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame0].x = 10; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame0].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame0].ancho = 40; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame0].alto = 70;
	//2
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame1].x = 50; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame1].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame1].ancho = 40; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame1].alto = 70;
	//3
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame2].x = 90; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame2].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame2].ancho = 43; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame2].alto = 70;
	//4
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame3].x = 133; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame3].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame3].ancho = 43; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame3].alto = 70;
	//5
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame4].x = 176; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame4].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame4].ancho = 44; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame4].alto = 70;
	//6
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame5].x = 220; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame5].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame5].ancho = 60; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame5].alto = 70;
	//7
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame6].x = 280; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame6].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame6].ancho = 56; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame6].alto = 70;
	//8
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame7].x = 336; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame7].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame7].ancho = 56; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame7].alto = 70;
	//9
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame8].x = 392; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame8].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame8].ancho = 56; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame8].alto = 70;
	//10
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame9].x = 448; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame9].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame9].ancho = 56; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame9].alto = 70;
	//11
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame10].x = 504; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame10].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame10].ancho = 56; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame10].alto = 70;
	//12
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame11].x = 560; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame11].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame11].ancho = 56; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame11].alto = 70;
	//13
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame12].x = 616; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame12].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame12].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame12].alto = 70;
	//14
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame13].x = 655; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame13].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame13].ancho = 38; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame13].alto = 70;
	//15
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame14].x = 693; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame14].y = 1909;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame14].ancho = 39; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDance][Frame14].alto = 70;

	// ------ - Animacion 4 Muerte - ------ //
	//1
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame0].x = 5; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame0].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame0].ancho = 53; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame0].alto = 54;
	//2
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame1].x = 58; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame1].y = 1204;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame1].ancho = 60; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame1].alto = 67;
	//3
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame2].x = 118; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame2].y = 1201;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame2].ancho = 68; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame2].alto = 70;
	//4
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame3].x = 186; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame3].y = 1191;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame3].ancho = 65; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame3].alto = 80;
	//5
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame4].x = 251; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame4].y = 1186;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame4].ancho = 61; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame4].alto = 85;
	//6
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame5].x = 312; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame5].y = 1175;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame5].ancho = 50; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame5].alto = 96;
	//7
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame6].x = 362; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame6].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame6].ancho = 47; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame6].alto = 54;
	//8
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame7].x = 409; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame7].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame7].ancho = 60; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame7].alto = 54;
	//9
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame8].x = 469; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame8].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame8].ancho = 65; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame8].alto = 54;
	//10
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame9].x = 534; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame9].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame9].ancho = 65; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame9].alto = 54;
	//11
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame10].x = 599; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame10].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame10].ancho = 64; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame10].alto = 54;
	//12
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame11].x = 663; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame11].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame11].ancho = 67; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame11].alto = 54;
	//13
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame12].x = 730; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame12].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame12].ancho = 70; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame12].alto = 54;
	//14
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame13].x = 800; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame13].y = 1217;
	miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame13].ancho = 70; miPersonaje.FrameSpriteArray[miPersonaje.idAnimacionDeath][Frame13].alto = 54;
}
//Enemigo
void CargaFramesSprite_E() {
	//Cargamos primero las hojas de sprite
	miEnemigo.HojaSprite = gdipLoad(miEnemigo.BmpW);

	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos
	
	//if(KEYS[input.E])
	int Coords = rand() % (400 - 150 +1)+150;
	
	miEnemigo.XCurrentCoordDraw = 700; //max 600 min 0
	miEnemigo.YCurrentCoordDraw = Coords; //Max 150 Min 400 
	//Definiendo los tamaños de nuestro sprite para renderizarlo en la ventana

	//Definiendo las dimenciones en base al # de Animaciones
	miEnemigo.FrameSpriteArray = new FrameArray * [miEnemigo.Animaciones_Enemigo];
	//Definiendo la cantidad de Frames en base a cada Animacion
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning] = new FrameArray[miEnemigo.FramesAnimacionRunning]; 

	// ------ - Animacion 1 Corriendo - ------ //
	//0
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].x = 609; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].y = 506;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].ancho = 74; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame0].alto = 81; 
	//1
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].x = 529; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].y = 506;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].ancho = 77; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame1].alto = 81;
	//3
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].x = 450; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].y = 506;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].ancho = 76; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame2].alto = 81;
	//4
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].x = 373; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].y = 506;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].ancho = 74; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame3].alto = 81;
	//5
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].x = 293; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].y = 506;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].ancho = 77; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame4].alto = 81;
	//6
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].x = 214; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].y = 506;
	miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].ancho = 76; miEnemigo.FrameSpriteArray[miEnemigo.idAnimacionRunning][Frame5].alto = 81;//180

}
//Monedas
void CargaFramesSprites_C(){
	//Cargamos primero las hojas de sprite
	miMoneda.HojaSprite = gdipLoad(miMoneda.BmpW);
	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos
	int Coordsc = rand() % (400 - 150 + 1) + 150;
	miMoneda.XCurrentCoordDraw = 700; //max 600 min 0
	miMoneda.YCurrentCoordDraw = Coordsc; //Max 150 Min 400 

	//Definiendo las dimenciones en base al # de Animaciones
	miMoneda.FrameSpriteArray = new FrameArray * [miMoneda.Animaciones_Coin];

	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1] = new FrameArray[miMoneda.FramesAnimacionCoin1]; 
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2] = new FrameArray[miMoneda.FramesAnimacionCoin2];
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3] = new FrameArray[miMoneda.FramesAnimacionCoin3];
	// ------ - Animacion 1 Moneda 1- ------ //
	//0
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame0].x = 22; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame0].y = 134;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame0].ancho = 39; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame0].alto = 57;
	//1
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame1].x = 66; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame1].y = 134;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame1].ancho = 15; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame1].alto = 57;
	//3
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame2].x = 88; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame2].y = 134;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame2].ancho = 39; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame2].alto = 57;
	//4
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame3].x = 134; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame3].y = 134;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame3].ancho = 51; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame3].alto = 57;
	//5
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame4].x = 194; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame4].y = 134;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame4].ancho = 57; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame4].alto = 57;
	//6
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame5].x = 262; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame5].y = 134;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame5].ancho = 51 ; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin1][Frame5].alto = 57;
	// ------ - Animacion 2 Moneda 2- ------ //
	//0
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame0].x = 21; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame0].y = 203;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame0].ancho = 39; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame0].alto = 63;
	//1
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame1].x = 66; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame1].y = 203;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame1].ancho = 15; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame1].alto = 63;
	//3
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame2].x = 87; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame2].y = 203;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame2].ancho = 38; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame2].alto = 63;
	//4
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame3].x = 132; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame3].y = 203;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame3].ancho = 57; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame3].alto = 63;
	//5
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame4].x = 192; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame4].y = 203;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame4].ancho = 63; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame4].alto = 63;
	//6
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame5].x = 258; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame5].y = 203;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame5].ancho = 57; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin2][Frame5].alto = 63;
	// ------ - Animacion 3 Moneda 3 - ------ //
	//0
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame0].x = 21; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame0].y = 276;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame0].ancho = 39; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame0].alto = 68;
	//1
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame1].x = 66; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame1].y = 276;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame1].ancho = 15; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame1].alto = 68;
	//3
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame2].x = 89; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame2].y = 276;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame2].ancho = 39; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame2].alto = 68;
	//4
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame3].x = 134; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame3].y = 276;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame3].ancho = 63; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame3].alto = 68;
	//5
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame4].x = 204; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame4].y = 276;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame4].ancho = 69; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame4].alto = 68;
	//6
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame5].x = 278; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame5].y = 276;
	miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame5].ancho = 63; miMoneda.FrameSpriteArray[miMoneda.idAnimacionCoin3][Frame5].alto = 68;
}

void CargarFramesPiggy() {
	//Cargamos primero las hojas de sprite
	misPowerUp.HojaSprite = gdipLoad(misPowerUp.BmpW);

	//Definiendo las coordenadas iniciales en pantalla donde iniciaremos
	int Coordsp = rand() % (400 - 150 + 1) + 150;
	misPowerUp.XCurrentCoordDraw = 800; //max 600 min 0
	misPowerUp.YCurrentCoordDraw = Coordsp; //Max 150 Min 400 
	//Definiendo los tamaños de nuestro sprite para renderizarlo en la ventana

	//Definiendo las dimenciones en base al # de Animaciones
	misPowerUp.FrameSpriteArray = new FrameArray * [misPowerUp.Animaciones_PowerUp];
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp] = new FrameArray[misPowerUp.FramesAnimacionPowerUp];
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1] = new FrameArray[misPowerUp.FramesAnimacionPowerUp1];
	//----- -Piggy- ------//
	//0
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame0].x = 938; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame0].y = 3;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame0].ancho = 92; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame0].alto = 91;
	//1
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame1].x = 841; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame1].y = 3;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame1].ancho = 94; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame1].alto = 91;
	//3
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame2].x = 744; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame2].y = 3;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame2].ancho = 94; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame2].alto = 91;
	//4
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame3].x = 649; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame3].y = 3;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame3].ancho = 92; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp][Frame3].alto = 91;
	//----- -Piggy- ------//
	//0
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame0].x = 916; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame0].y = 97;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame0].ancho = 137; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame0].alto = 138;
	//1
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame1].x = 758; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame1].y = 97;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame1].ancho = 137; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame1].alto = 138;
	//3
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame2].x = 607; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame2].y = 97;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame2].ancho = 137; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame2].alto = 138;
	//4
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame3].x = 451; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame3].y = 97;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame3].ancho = 137; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame3].alto = 138;
	//5
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame4].x = 298; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame4].y = 97;
	misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame4].ancho = 137; misPowerUp.FrameSpriteArray[misPowerUp.idAnimacionPowerUp1][Frame4].alto = 138;
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
		//Dibujamos Titulo
		TranScaleblt(ptrBufferPixelsWindow, (misRecursos.Title1.pixeles),
			290, 350,//Iniciamos a dibujar en la ventana en 0,0
			0, 0,//Indicamos cuales son las coordenadas para dibujar desde nuestra imagen; iniciamos en 0,0 desde nuestro escenario
			500, 218,//Definimos cuantos pixeles dibujaremos de nuestra imagen a la pantalla
			800, misRecursos.Title1.ancho,
			1, 1, TRANSPARENCY_E, 1);//Si ponemos un numero mayor a 1 estaremos repitiendo 2 veces la linea de pixeles en X o en Y
		//Dibujamos "Press_Enter"
		TranScaleblt(ptrBufferPixelsWindow, (misRecursos.Title2.pixeles),
			100, 20,//Iniciamos a dibujar en la ventana en 0,0
			0, 0,//Indicamos cuales son las coordenadas para dibujar desde nuestra imagen; iniciamos en 0,0 desde nuestro escenario
			589, 30,//Definimos cuantos pixeles dibujaremos de nuestra imagen a la pantalla
			800, misRecursos.Title2.ancho,
			1, 1, TRANSPARENCY, 1);//Si ponemos un numero mayor a 1 estaremos repitiendo 2 veces la linea de pixeles en X o en Y

	}
	else if(pantallaNivel)
	{
		
		//Dibujamos el Primer Escenario
		TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario2.pixeles),
			0, 0,//Iniciamos a dibujar en la ventana en 0,0
			Mov_fondo, 0,//Indicamos cuales son las coordenadas para dibujar desde nuestra imagen; iniciamos en 0,0 desde nuestro escenario 
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
		if(F_Velocidad <= 15){				
			TranScaleblt(ptrBufferPixelsWindow, (miEnemigo.HojaSprite.pixeles),
				miEnemigo.XCurrentCoordDraw, miEnemigo.YCurrentCoordDraw,
				miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].x, miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].y,
				miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].ancho, miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].alto,
				800, miEnemigo.HojaSprite.ancho,
				1, 1, TRANSPARENCY_E, 1);
		}
	
		//Dibujamos las monedas
			TranScaleblt(ptrBufferPixelsWindow, (miMoneda.HojaSprite.pixeles),
				miMoneda.XCurrentCoordDraw, miMoneda.YCurrentCoordDraw,
				miMoneda.FrameSpriteArray[Animacion_C][C_ActualFrame].x, miMoneda.FrameSpriteArray[Animacion_C][C_ActualFrame].y,
				miMoneda.FrameSpriteArray[Animacion_C][C_ActualFrame].ancho, miMoneda.FrameSpriteArray[Animacion_C][C_ActualFrame].alto,
				800, miMoneda.HojaSprite.ancho,
				1, 1, TRANSPARENCY, 1);
			
			DetectaColisiones();
			Collbox_Piggy();
			if (Monedas == 500 && Contador_YIYI == 0) {
				Contador_YIYI += 1;
				Max_Power_Up += 1;
			}
			if (Monedas == 1000 && Contador_YIYI == 1) {
				Contador_YIYI += 1;
				Max_Power_Up += 1;
			}
			if (Contador == 11 && Contador_YIYI == 2) {
				Contador_YIYI += 1;
				Max_Power_Up += 1;
			}
			if (Max_Power_Up == 1 && BERE != 3) {
				if (Once) {					
					misPowerUp.XCurrentCoordDraw = 800;
					Once = false;
				}
				TranScaleblt(ptrBufferPixelsWindow, (misPowerUp.HojaSprite.pixeles),
					misPowerUp.XCurrentCoordDraw, misPowerUp.YCurrentCoordDraw,
					misPowerUp.FrameSpriteArray[Animacion_P][P_ActualFrame].x, misPowerUp.FrameSpriteArray[Animacion_P][P_ActualFrame].y,
					misPowerUp.FrameSpriteArray[Animacion_P][P_ActualFrame].ancho, misPowerUp.FrameSpriteArray[Animacion_P][P_ActualFrame].alto,
					800, misPowerUp.HojaSprite.ancho,
					1, 1, TRANSPARENCY_P, 1);

			}						
			if (Muerte) { 
				AnimacionActual = Death;
				Estatus = false;
			}
			if (A_Coin1) {
				A_Coin1 = false;
				Monedas += 100;
				Contador += 1;
				if (Contador == 3) {
					Animacion_C = Coin2;
				}
				if (Contador == 10) {
					Animacion_C = Coin3; 
				}
			}		
			if (P_Power) {
				Max_Power_Up -= 1;
				P_Power = false;				
				if (BERE <= 2)
				F_Velocidad += 70;
				if (BERE == 3){				
					//SORTEO DE X2 A TODO!!! O 8000 PTS
					int W = rand() % 100;
					if (W % 2 == 0) {
						Monedas += 4000;
					}
					else {
						Extra_Bonus = 1.5;
					}
				}									
			}
			//Dibujamos la barra de vida
			if (AnimacionActual != Death && Mov_fondo <= 10000) {
				TranScaleblt(ptrBufferPixelsWindow, (misRecursos.Title5.pixeles), 0, 0, 0, 0, 290, 112, 800, misRecursos.Title5.ancho, 1, 1, TRANSPARENCY, 1);
			}
	}
	else if(pantallaVictoria){	

		if (FinalMusic == false) {
			FinalMusic = true;
		ReproductorCambiarCancionYReproduce(3);
		}
		//Pantalla de Victoria Scott_KISS
		TranScaleblt(ptrBufferPixelsWindow, (miStage.ImagenEscenario3.pixeles),
			0, 0,//Iniciamos a dibujar en la ventana en 0,0
			133, 0,//Indicamos cuales son las coordenadas para dibujar desde nuestra imagen; iniciamos en 0,0 desde nuestro escenario 
			1066, 600,//Definimos cuantos pixeles dibujaremos de nuestra imagen a la pantalla
			800, miStage.ImagenEscenario3.ancho,
			1, 1, TRANSPARENCY, 1);//Si ponemos un numero mayor a 1 estaremos repitiendo 2 veces la linea de pixeles en X o en Y

		//Victoria
		TranScaleblt(ptrBufferPixelsWindow, (misRecursos.Title3.pixeles), 40, 50, 0, 0, 700, 58, 800, misRecursos.Title3.ancho, 1, 1, TRANSPARENCY, 1);
	 
		if (!ActiveScore) {
			
			ActiveScore = true;	  			
			if (numPuntuaciones < maxPuntuaciones) { 
				// Guardar la puntuación
				Puntuaciones[numPuntuaciones] = PuntajeT;
				numPuntuaciones++;
				ordenarDescendente(Puntuaciones, numPuntuaciones); 
			}
			else{
				int indiceMenor = 0; 
				float menorPuntuacion = Puntuaciones[0]; 
				for (int i = 1; i < 3; ++i) { //Encuentra el #menor del arreglo
					if (Puntuaciones[i] < menorPuntuacion) { 
						menorPuntuacion = Puntuaciones[i]; 
						indiceMenor = i; 
					}
				}

				if (PuntajeT > menorPuntuacion) {
					// Si el nuevo puntaje es mayor que la puntuación más baja, reemplaza esa puntuación
					Puntuaciones[indiceMenor] = PuntajeT; 

					// Ordenar nuevamente las puntuaciones de mayor a menor
					ordenarDescendente(Puntuaciones, 3); 
				}				
			}
			ostringstream ss;
			ss << fixed << std::setprecision(0) << PuntajeT;
			string str = ss.str();
			wstring mensaje = L"Su puntuacion ha sido: " + wstring(str.begin(), str.end());			
			MessageBox(NULL, mensaje.c_str(), L"Score", MB_OK | MB_ICONINFORMATION);
		}
	}

}

bool Active_Animation = true; //Validacion si hay alguna animacion en curso 
//Actualizacion de las animaciones
//Scott
void ActualizaAnimacion(HWND hWnd){
	switch (AnimacionActual) {

		case Idle:

		FrameActual++;
		if (FrameActual > 5) FrameActual = 0;			

			break;

		case Dash: {

			FrameActual++;
			if (FrameActual > 7) {
				FrameActual = 0;				
			}
		}break;
		case Jump: {
			if (Active_Animation == true) {
				FrameActual = 0;
				Active_Animation = false;
			}			
			FrameActual++;				
			if (FrameActual > 12) {//Mientras los frames no superen 12 seguira haciendo la animacion de saltar
				FrameActual = 0;
				AnimacionActual = Idle;
				Active_Animation = true;
			}
						
		}break;
		case Walk: {			
			FrameActual++;
			if (FrameActual > 5) {		
				FrameActual = 0;
			}
			
		}break;
		case Dance: {
		if(Bailes < 1){
			if (Tick % Tick == 0 && FrameActual == 0){//125			
				DelayFrameAnimation++;
			}
			else if (Tick % Tick == 0 && FrameActual == 7){			
				DelayFrameAnimation += 5;
			}
			if (DelayFrameAnimation % 1 == 0){			
				FrameActual++;		
				if (FrameActual > 14) {				
					FrameActual = 0; 
					Bailes += 1;
				}
			}
		}	
		}break;
		case Death: {
			TranScaleblt(ptrBufferPixelsWindow, (misRecursos.Title4.pixeles), 10, 400, 0, 0, 749, 98, 800, misRecursos.Title4.ancho, 1, 1, TRANSPARENCY, 1);
			FrameActual++;
			if (FrameActual > 13) { 			
				FrameActual = 13;
			}			
		}break;
	}
	
		InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}
//Monedas
void ActualizaAnimacionC(HWND hWnd) {
	switch (Animacion_C) {
		case Coin1:{
			C_ActualFrame++;
			if (C_ActualFrame > 5 ) C_ActualFrame = 0;
		}break;
		case Coin2: { 
			C_ActualFrame++;
			if (C_ActualFrame > 5) C_ActualFrame = 0;	
		}break;
		case Coin3: {
			C_ActualFrame++;
			if (C_ActualFrame > 5) C_ActualFrame = 0;
		}break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}
//Enemigo
void ActualizaAnimacionE(HWND hWnd) {
	switch (Animacion_E) {
	case Running_E:
		E_ActualFrame++;		
		if (E_ActualFrame > 5) E_ActualFrame = 0;
		break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}
//Piggy(PowerUp)
void ActualizaAnimacionP(HWND hWnd) {
	switch (Animacion_P) {
	case Piggy:
		P_ActualFrame++; 
		if (P_ActualFrame > 3) P_ActualFrame = 0; 
		break; 
	case Piggy_used:
		if (Tick % Tick == 0 && FrameActual == 0)//125
		{
			DelayFrameAnimation++;
		}
		else if (Tick % Tick == 0 && FrameActual == 7)
		{
			DelayFrameAnimation += 5;
		}
		if (DelayFrameAnimation % 1 == 0)
		{
			FrameActual++;
			if (FrameActual > 4) {
				FrameActual = 0;
				Animacion_P = Piggy;
			}
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

void MainRender(HWND hWnd) 
{
	LimpiarFondo(ptrBufferPixelsWindow, 0xFFFFFFFF, (ANCHO_VENTANA * ALTO_VENTANA));
	KeysEvents();	
	DibujaPixeles();
	ActualizaAnimacion(hWnd); 
	ActualizaAnimacionC(hWnd);
	ActualizaAnimacionE(hWnd);
	ActualizaAnimacionP(hWnd);
	
}

void Frame(float deltatime) {

}
bool Collbox_Piggy(){

	/*DibujaHitbox(ptrBufferPixelsWindow, Blanco, ANCHO_VENTANA, ALTO_VENTANA, //Tamaño de la ventana
		misPowerUp.XCurrentCoordDraw, misPowerUp.YCurrentCoordDraw, //Cordenadas de la caja de colision
		94, 91,//Ancho y Alto de la caja
		1, 1);//Escala de la caja de colision*/
	
	if (misPowerUp.XCurrentCoordDraw < 20 + 40 &&  // Ax < Bx  + BWx 
		misPowerUp.XCurrentCoordDraw + 94 > 0 &&   // Ax + AWx > Bx 
		misPowerUp.YCurrentCoordDraw < 0 + 600 && // Ay < By  + BHy
		misPowerUp.YCurrentCoordDraw + 91 > 0) {   // Ay + AHy > By		
		 
		misPowerUp.XCurrentCoordDraw = 800;
		misPowerUp.YCurrentCoordDraw = rand() % (400 - 150 + 1) + 150;
		if (Max_Power_Up == 1) Max_Power_Up -= 1;
		return Once = true;		
	}	
	if(Max_Power_Up == 1 && BERE != 3){

		if (misPowerUp.XCurrentCoordDraw < miPersonaje.XCurrentCoordDraw + miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].ancho + 80 &&
			misPowerUp.XCurrentCoordDraw + 94 > miPersonaje.XCurrentCoordDraw &&
			misPowerUp.YCurrentCoordDraw < miPersonaje.YCurrentCoordDraw + miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].alto + 100 &&
			misPowerUp.YCurrentCoordDraw + 91 > miPersonaje.YCurrentCoordDraw) {

			BERE += 1;
			misPowerUp.XCurrentCoordDraw = 700;
			misPowerUp.YCurrentCoordDraw = rand() % (400 - 150 + 1) + 150;
			if(Contador >= 10)
			Contador = 0;
			//Animacion_P = Piggy_used;
			return P_Power = true;
		}

	}

}
bool DetectaColisiones() {
	//Colison extrema izq
	/*DibujaHitbox(ptrBufferPixelsWindow, Blanco, ANCHO_VENTANA, ALTO_VENTANA, //Tamaño de la ventana
		0, 0, //Cordenadas de la caja de colision
		20, 600,//Ancho y Alto de la caja
		1, 1);//Escala de la caja de colision
	
	//Colision del enemigo
		DibujaHitbox(ptrBufferPixelsWindow, Rojo, ANCHO_VENTANA, ALTO_VENTANA, //Tamaño de la ventana
		miEnemigo.XCurrentCoordDraw , miEnemigo.YCurrentCoordDraw, //Cordenadas de la caja de colision
		miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].ancho, 81,//Ancho y Alto de la caja
		1, 1);//Escala de la caja de colision
	//Colision de Scott Pilgrim	
		DibujaHitbox(ptrBufferPixelsWindow, Verde, ANCHO_VENTANA, ALTO_VENTANA, //Tamaño de la ventana
		miPersonaje.XCurrentCoordDraw, miPersonaje.YCurrentCoordDraw, //Cordenadas de la caja de colision
		miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].ancho+80, miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].alto+100,//Ancho y Alto de la caja
		1 ,1);//Escala de la caja de colision 

		DibujaHitbox(ptrBufferPixelsWindow, Blanco, ANCHO_VENTANA, ALTO_VENTANA, //Tamaño de la ventana
		miMoneda.XCurrentCoordDraw, miMoneda.YCurrentCoordDraw, //Cordenadas de la caja de colision
		51, 57,//Ancho y Alto de la caja
		1, 1);//Escala de la caja de colision

		DibujaHitbox(ptrBufferPixelsWindow, Blanco, ANCHO_VENTANA, ALTO_VENTANA, //Tamaño de la ventana
			misPowerUp.XCurrentCoordDraw, misPowerUp.YCurrentCoordDraw, //Cordenadas de la caja de colision
			94, 91,//Ancho y Alto de la caja
			1, 1);//Escala de la caja de colision

*/
	
		/**/if (F_Velocidad <= 15) {
			if (miEnemigo.XCurrentCoordDraw < miPersonaje.XCurrentCoordDraw + miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].ancho+80 &&
				miEnemigo.XCurrentCoordDraw + miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].ancho > miPersonaje.XCurrentCoordDraw &&
				miEnemigo.YCurrentCoordDraw < miPersonaje.YCurrentCoordDraw + miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].alto+100 &&
				miEnemigo.YCurrentCoordDraw + 71  > miPersonaje.YCurrentCoordDraw) {
				return Muerte=true;			
			}	
		}
		if (miEnemigo.XCurrentCoordDraw < 15 + 15 &&
		miEnemigo.XCurrentCoordDraw + miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].ancho + 70 > 20 &&
		miEnemigo.YCurrentCoordDraw < 585 && 
		miEnemigo.YCurrentCoordDraw + miEnemigo.FrameSpriteArray[Animacion_E][E_ActualFrame].alto > 0) {
		// Generar nuevas coordenadas aleatorias para el enemigo
		
		miEnemigo.XCurrentCoordDraw = 770; // max 600 min 0
		miEnemigo.YCurrentCoordDraw = rand() % (400 - 150 + 1) + 150;
		}

		if (miMoneda.XCurrentCoordDraw < 0  + 40  && // Ax < Bx  + BWx 
			miMoneda.XCurrentCoordDraw + 51 > 0   && // Ax + AWx > Bx 
			miMoneda.YCurrentCoordDraw < 0  + 600 && //	Ay < By  + BHy
			miMoneda.YCurrentCoordDraw + 57 > 0) {   // Ay + AHy > By
			Animacion_C = Coin1;
			Contador = 0;//Se le fue una moneda reinicia el contador
			miMoneda.XCurrentCoordDraw = 800; 
			miMoneda.YCurrentCoordDraw = rand() % (400 - 150 + 1) + 150;
		}

		if (miMoneda.XCurrentCoordDraw < miPersonaje.XCurrentCoordDraw + miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].ancho + 80 &&
			miMoneda.XCurrentCoordDraw + 51 > miPersonaje.XCurrentCoordDraw &&
			miMoneda.YCurrentCoordDraw < miPersonaje.YCurrentCoordDraw + miPersonaje.FrameSpriteArray[AnimacionActual][FrameActual].alto + 100 &&
			miMoneda.YCurrentCoordDraw + 57 > miPersonaje.YCurrentCoordDraw) {

			if (Animacion_C == Coin2) Monedas += 200; //Agarro la moneda tipo 2 suma 200 pts
			if (Animacion_C == Coin3) { 
				Monedas += 500;				
			}
			Animacion_C = Coin1;					  //Reset a Moneda tipo 1		
			miMoneda.XCurrentCoordDraw = 800;
			miMoneda.YCurrentCoordDraw = rand() % (400 - 150 + 1) + 150;
			if(E_Velocidad<23)E_Velocidad += 1;		//Dificultad del enemigo
			return A_Coin1 = true;			
		}								
}

void Puntaje(int seg) {
	// 1 min con 20s es lo mas tardado que puedes
	if (seg < 61) {
		PuntajeT += ((2600 + Monedas) * 1.5)* Extra_Bonus;
	}
	else if (62 <= seg < 79) {
		PuntajeT += ((2100 + Monedas) * 1) * Extra_Bonus;
	}
	else if (seg > 79) {
		PuntajeT += ((1600 + Monedas) * 0.5) * Extra_Bonus;
	}
	
}

//Funcion para resetear variables al perder o ganar
void Reset_All() {
	
	pantallaNivel = true;//Setear pantallas
	pantallaVictoria = false;
	pantallaInicial = false;
	Estatus = true;
	Muerte = false;
	ActiveScore = false;
	FinalMusic = false;
	Contador_YIYI = 0;
	Extra_Bonus = 1;  //Codigo para resetear bonus
	Monedas = 0;
	Contador = 0;
	BERE = 0;		  //Reset contador de cuantos piggy agarraste
	Bailes = 0;			
	AnimacionActual = Dash; 
	F_Velocidad = 15; //Resetear la velocidad del fondo
} 

bool SpacePressed = false;

void KeysEvents()
{
	int count = 0;
	if (KEYS[input.Enter] && pantallaInicial == true){	
		Reset_All();
		time(&tiempoInicio); 
		Init();
	}
	if (KEYS[input.Backspace]){	
		pantallaInicial = true;
		//resetar todo
		END_GAME = false;
		Mov_fondo = 0;
		Max_Power_Up = 0; //Codigo para resetear a piggy  
		ReproductorPausa();
		Init();
	}
	if (!END_GAME && Estatus) {//Si Endgame es true, scott llego al final
		if (!pantallaInicial){//Si no es la pantalla de inicio haz lo siguiente
		
			//Movimiento
			if (KEYS[input.W] || KEYS[input.Up]){
				if (AnimacionActual == Dash) FrameActual = 0;
				if (miPersonaje.YCurrentCoordDraw >= 130) {
					miPersonaje.YCurrentCoordDraw -= 20;
					AnimacionActual = Walk;						
				}else{
					AnimacionActual = Walk; 
				}
				W_Pressed = true;
			}else if(W_Pressed == true){
				W_Pressed = false;
				AnimacionActual = Dash;
			}
			if (KEYS[input.S] || KEYS[input.Down])
			{	
				if (AnimacionActual == Dash) FrameActual = 0; 
				if (miPersonaje.YCurrentCoordDraw < 366) {
					miPersonaje.YCurrentCoordDraw += 20;
					AnimacionActual = Walk; 
				}
				else {
					AnimacionActual = Walk;						
				}
				S_Pressed = true;
			}else if(S_Pressed == true) {
				S_Pressed = false;
				AnimacionActual = Dash; 
			}
			///Baile
			if (KEYS[input.F]) {			
					F_Pressed = true;
					AnimacionActual = Dance;
			}
			else if(F_Pressed){
				F_Pressed = false;
				AnimacionActual = Idle; 
				FrameActual = 0;
			}			
		}
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
void DibujaHitbox(int* ptrBuffer,
	unsigned int color, int anchoWnd, int altoWnd, int HitboxX, int HitboxY, int HitboxAncho, int HitboxAlto, int escalaX, int escalaY)
{
	HitboxAncho = HitboxAncho * escalaX;
	HitboxAlto = HitboxAlto * escalaY;
	__asm {
		mov edi, ptrBuffer

		mov eax, HitboxY
		mul BPP
		mul anchoWnd
		add edi, eax

		mov eax, HitboxX
		mul BPP
		add edi, eax

		mov eax, color

		mov ecx, HitboxAlto
		lazo_alto :
		push ecx
			mov ecx, HitboxAncho
			lazo_ancho :
		mov[edi], eax
			add edi, BPP
			//stosd
			loop lazo_ancho
			push eax
			mov eax, anchoWnd
			sub eax, HitboxAncho
			mul BPP
			add edi, eax
			pop eax
			pop ecx
			loop lazo_alto
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
	Cancion[3].Nombre = "Victoria";
	Cancion[3].Dir = "Recursos/Audio/Level_Complete.mp3";
	ifstream inputFile(Cancion[0].Dir.c_str());

	if (!inputFile.good()) 
		printf("No file found");
	else 
		player->OpenFile(Cancion[0].Dir.c_str(), sfAutodetect);
	player->SetMasterVolume(i, d);// Sonido tipo estereo Left and Right - Volumen de 0 - 100
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