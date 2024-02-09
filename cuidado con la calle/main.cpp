#include <iostream>
#include <vector>
#include <conio2.h>
#include <ctime>
#include <windows.h>
#include <cstdlib> 

using namespace std;

const int WIDTH = 41; 
const int HEIGHT = 10; 

class Jugador; 

class Enemigo {
private:
	int x, y;
	char forma[6]; 
	int velocidad; 
	bool direccionDerecha; 
public:
	Enemigo(int startX, int startY, bool dir) : x(startX), y(startY), direccionDerecha(dir) {
		forma[0] = '<';
		forma[1] = '[';
		forma[2] = '-';
		forma[3] = '-';
		forma[4] = ']';
		forma[5] = '>';
		establecerVelocidad();
	}
	
	void establecerVelocidad() {
		
		velocidad = rand() % 5 + 1;
	}
	
	void dibujar() {
		textcolor(RED);
		putchxy(x, y, forma[0]); 
		putchxy(x + 1, y, forma[1]);
		putchxy(x + 2, y, forma[2]);
		putchxy(x + 3, y, forma[3]);
		putchxy(x + 4, y, forma[4]);
		putchxy(x + 5, y, forma[5]);
	}
	
	void borrar() {
		for (int i = 0; i < 6; i++)
			putchxy(x + i, y, ' ');
	}
	
	void mover() {
		borrar();
		if (direccionDerecha) {
			x += velocidad;
			if (x >= WIDTH)
				x = 1; 
		}
		else {
			x -= velocidad;
			if (x <= 0)
				x = WIDTH - 6; 
		}
		dibujar();
	}
	
	int getX() const { return x; }
	int getY() const { return y; }
	
	friend class Jugador; 
};

class Jugador {
private:
	int x, y; 
public:
	Jugador() {
		x = WIDTH / 2;
		y = HEIGHT - 2;
	}
	
	void dibujar() {
		textcolor(GREEN);
		putchxy(x, y, '(');
		putchxy(x + 1, y, '*');
		putchxy(x + 2, y, '-');
		putchxy(x + 3, y, '*');
		putchxy(x + 4, y, ')');
	}
	
	void borrar() {
		for (int i = 0; i < 5; i++)
			putchxy(x + i, y, ' ');
	}
	
	void mover(int dx, int dy) {
		borrar();
		x += dx;
		y += dy;
		dibujar();
	}
	
	int getX() const { return x; }
	int getY() const { return y; }
	
	void morir() {
		textcolor(GREEN);
		gotoxy(WIDTH / 2 - 18, HEIGHT / 2);
		cout << "JAJAJA PERDISTE INTENTA OTRA VEZ";
	}
	
	bool colision(Enemigo& enemigo) {
		
		return (x >= enemigo.getX() && x <= enemigo.getX() + 5 && y == enemigo.getY());
	}
	
	bool tocaBarrera() {
		
		return (y == HEIGHT - 1);
	}
	
	bool colisionBarreraDerecha(int x_barrera) {
		
		return (x == x_barrera);
	}
};

class Juego {
private:
	Jugador jugador;
	vector<Enemigo> enemigos;
	int x_barrera_derecha; 
	
public:
	Juego() {
		
		srand(time(nullptr));
		for (int i = 0; i < 3; ++i) {
			enemigos.push_back(Enemigo(2, 2 * i + 2, true)); 
			enemigos.push_back(Enemigo(WIDTH - 6, 2 * i + 3, false)); 
		}
		x_barrera_derecha = WIDTH - 2;
	}
	
	void iniciar() {
		clrscr();
		dibujarBordes();
		dibujarBarreras(); 
		jugador.dibujar();
		for (auto& enemigo : enemigos) {
			enemigo.dibujar();
		}
		while (true) {
			if (_kbhit()) {
				int tecla = _getch();
				switch (tecla) {
				case 72: // Arriba
					jugador.mover(0, -1);
					break;
				case 80: // Abajo
					jugador.mover(0, 1);
					break;
				case 75: // Izquierda
					jugador.mover(-1, 0);
					break;
				case 77: // Derecha
					jugador.mover(1, 0);
					if (jugador.colisionBarreraDerecha(x_barrera_derecha)) {
						jugador.morir();
						return;
					}
					break;
				case 27: 
					return;
				}
			}
			for (auto& enemigo : enemigos) {
				enemigo.mover();
				if (jugador.colision(enemigo) || jugador.tocaBarrera()) {
					
					jugador.morir();
					return;
				}
			}
			Sleep(100); 
		}
	}
	
private:
		void dibujarBordes() {
			
			textcolor(WHITE);
			for (int i = 0; i < HEIGHT; i++) {
				putchxy(0, i, '|');
				putchxy(WIDTH - 1, i, '|');
			}
			for (int i = 0; i < WIDTH; i++) {
				putchxy(i, HEIGHT - 1, '-');
			}
		}
		
		void dibujarBarreras() {
			
			textcolor(YELLOW);
			for (int i = 1; i < HEIGHT - 1; i++) {
				putchxy(WIDTH - 2, i, '|');
			}
		}
};

int main() {
	Juego juego;
	juego.iniciar();
	return 0;
}   

