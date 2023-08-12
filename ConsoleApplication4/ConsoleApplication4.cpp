#include <iostream>
#include <GL/glut.h>
#include <sstream>
#include <math.h>

int largura = 700;
int altura = 400;

int barra_largura = 10;
int barra_altura = 80;
int barra_velocidade = 9;

float barra_esquerda_x = 10.0f;
float barra_esquerda_y = 150.0f;

float barra_direita_x = largura - barra_largura - 10;
float barra_direita_y = 150;

float bola_pos_x = largura / 2; 
float bola_pos_y = altura / 2;
float bola_dir_x = -1.0f;
float bola_dir_y = 0.0f;
int bola_tamanho = 8;
int bola_velocidade = 2;


int placar_esq = 0;
int placar_dir = 0;

std::string int2str(int x) {
	// converts int to string
	std::stringstream ss;
	ss << x;
	return ss.str();
}

void DrawText(const char *text, int length, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	// barra 1
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(barra_esquerda_x, barra_esquerda_y);
	glVertex2f(barra_esquerda_x + barra_largura, barra_esquerda_y);
	glVertex2f(barra_esquerda_x + barra_largura, barra_esquerda_y + barra_altura);
	glVertex2f(barra_esquerda_x, barra_esquerda_y + barra_altura);
	glEnd();
	glPopMatrix();
	// barra 2
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(barra_direita_x, barra_direita_y); //lado superior esquerdo
	glVertex2f(barra_direita_x + barra_largura, barra_direita_y); // lado inferior esquerdo
	glVertex2f(barra_direita_x + barra_largura, barra_direita_y + barra_altura); // lado inferior direito
	glVertex2f(barra_direita_x, barra_direita_y + barra_altura); // lado superior direito
	glEnd();
	glPopMatrix();
	// bola
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(bola_pos_x, bola_pos_y); //lado superior esquerdo
	glVertex2f(bola_pos_x + bola_tamanho, bola_pos_y); // lado inferior esquerdo
	glVertex2f(bola_pos_x + bola_tamanho, bola_pos_y + bola_tamanho); // lado inferior direito
	glVertex2f(bola_pos_x, bola_pos_y + bola_tamanho); // lado superior direito
	glEnd();
	glPopMatrix();

	std::string text, text2, text3;
	text =int2str(placar_dir);
	text2 = int2str(placar_esq);
	text3 = ":";

	DrawText(text2.data(), text2.size(), 350, 540);
	DrawText(text.data(), text.size(), 380, 540);
	DrawText(text3.data(), text3.size(), 365, 540);



	glFlush();

}

void GerenciaTeclado(unsigned char key, int x, int y) {
	switch (key) {
	case 'W':
	case 'w':
		if (barra_esquerda_y > (altura - 83)) barra_esquerda_y = barra_esquerda_y += 0;
		else barra_esquerda_y += barra_velocidade;
		break;
	case 'S':
	case 's':
		if (barra_esquerda_y < 5) barra_esquerda_y = barra_esquerda_y += 0;
		else barra_esquerda_y -= barra_velocidade;
		break;
	}
	glutPostRedisplay();
}


void Anima(int passo) {

	
	bola_pos_x += bola_dir_x * bola_velocidade;
	bola_pos_y += bola_dir_y * bola_velocidade;
	
	// bateu na barra da esquerda
	if (bola_pos_x < barra_esquerda_x + barra_largura && bola_pos_x > barra_esquerda_x &&
		bola_pos_y < barra_esquerda_y + barra_altura && bola_pos_y > barra_esquerda_y) {
		float direcao = ((bola_pos_y - barra_esquerda_y) / barra_altura) - 0.5f;
		bola_dir_x = fabs(bola_dir_x);
		bola_dir_y = direcao;
	}

	// bateu na barra da direita
	if (bola_pos_x > barra_direita_x && bola_pos_x < barra_direita_x + barra_largura &&
		bola_pos_y < barra_direita_y + barra_altura && bola_pos_y > barra_direita_y) {

		float direcao = ((bola_pos_y - barra_direita_y) / barra_altura) - 0.5f;
		bola_dir_x = -fabs(bola_dir_x); 
		bola_dir_y = direcao;
	}

	// marcou ponto na esquerda
	if (bola_pos_x < 0) {
		placar_dir = placar_dir + 1;
		bola_pos_x = largura / 2;
		bola_pos_y = altura / 2;
		bola_dir_x = fabs(bola_dir_x); 
		bola_dir_y = 0;
	}

	//marcou ponto na direita 
	if (bola_pos_x > largura) {
		placar_esq = placar_esq + 1;
		bola_pos_x = largura / 2;
		bola_pos_y = altura / 2;
		bola_dir_x = -fabs(bola_dir_x); 
		bola_dir_y = 0;
	}

	// bateu no teto
	if (bola_pos_y > altura) {
		bola_dir_y = -fabs(bola_dir_y); 
	}

	// bateu no chao
	if (bola_pos_y < 0) {
		bola_dir_y = fabs(bola_dir_y); 
	}
	

	glutPostRedisplay();
	glutTimerFunc(7, Anima, 1);
}


void TeclasEspeciais(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		if (barra_direita_y > (altura - 83)) barra_direita_y = barra_direita_y += 0;
		else barra_direita_y = barra_direita_y += barra_velocidade;
		
	}
	if (key == GLUT_KEY_DOWN) {
		if (barra_direita_y < 5) barra_direita_y = barra_direita_y += 0;
		else barra_direita_y = barra_direita_y -= barra_velocidade;
	}
	glutPostRedisplay();
}

void init() {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, largura, 0.0, altura);

}

int main() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(largura, altura);
	glutInitWindowPosition(250, 250);
	glutCreateWindow("Pong");
	glutDisplayFunc(display);
	glutKeyboardFunc(GerenciaTeclado);
	glutSpecialFunc(TeclasEspeciais);
	glutTimerFunc(7, Anima, 1);
	init();
	glColor3d(1.0f, 1.0f, 1.0f);
	glutMainLoop();
	return 0;
}