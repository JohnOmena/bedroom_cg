#include <GL/glut.h>
#include <math.h>
#include "bibutil.h"
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

// Variaveis para controle da projecao
GLfloat fAspect;
GLfloat ang_cam = 60;

// Variaveis para controle da janela
#define Z_LIM_EJ 50
#define Z_LIM_DJ -50
GLfloat z_ej = 50, z_dj = -50;

// Variaveis para controle da porta
#define ANGLE_DOOR_MAX 71
#define X_LIM_DOOR 55
#define Z_LIM_DOOR -55
#define XY_ONE_ANGLE 0.616666666
GLfloat x_trans_angle = 0, z_trans_angle = 0, angle_door = 0;

// Objetos
OBJ *plano, *mesa, *cadeira, *quadro, *porta, *janela, *lamp, *cama,
 *ventilador, *cabeceira, *quadrop, *luminaria, *computador, *armario;

// Luminosidade base de uma lampada
#define LOW	0.3

// Define parametros de iluminacao
// Luz 1: puntual no teto, frente
GLfloat luzAmb1[4] = { 0.1, 0.1, 0.1, 1 };	// luz ambiente
GLfloat luzDif1[4] = { LOW, LOW, LOW, 1.0 };	// luz difusa

// Luz 2: puntual no teto, meio da sala
GLfloat luzDif2[4] = { LOW, LOW, LOW, 1.0 };	// luz difusa
GLfloat posLuz2[4] = { 0, 200, 0, 1 };	// posicao da fonte de luz


// Define variaveis para navegacao
GLfloat rotX=0, rotY=0, rotX_ini, rotY_ini;
GLfloat obsX=0, obsY=150, obsZ=100, obsY_ini;
int x_ini,y_ini,bot;

// Arquivo de cena e arquivo de camera

// Desenha 4 paredes
void DesenhaParedes(void)
{	

	// Seta cor da parede
	glColor3ub(196,210,184);

	// Parede dos fundos
	// Insere a matriz de transformação corrente na pilha
	glPushMatrix();
	glTranslatef(0,150,-400);
	glScalef(6,3,1);
	DesenhaObjeto(plano);

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	// Parede da frente
	glPushMatrix();
	glTranslatef(62.5,100,400);
	glRotatef(180,0,1,0);
	glScalef(4.76,2,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parte superior da frente
	glPushMatrix();
	glTranslatef(0,250,400);
	glRotatef(180,0,1,0);
	glScalef(6,1,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parte direita da frente

	glPushMatrix();
	glTranslatef(-286.7,100,400);
	glRotatef(180,0,1,0);
	glScalef(0.265,2,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parede esquerda
	glPushMatrix();
	glTranslatef(300,150,0);
	glRotatef(-90,0,1,0);
	glScalef(8,3,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parte inferior
	glPushMatrix();
	glTranslatef(-300,37.5,0);
	glRotatef(90,0,1,0);
	glScalef(8,0.75,1);	// 8 m x 75 cm
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parte superior
	glPushMatrix();
	glTranslatef(-300,300-37.5,0);
	glRotatef(90,0,1,0);
	glScalef(8,0.75,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// antes do meio
	glPushMatrix();
	glTranslatef(-300,150,-254);
	glRotatef(90,0,1,0);
	glScalef(2.95,1.5,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// depois do meio
	glPushMatrix();
	glTranslatef(-300,150,254);
	glRotatef(90,0,1,0);
	glScalef(2.95,1.5,1);
	DesenhaObjeto(plano);
	glPopMatrix();


}

void DesenhaPorta(){

	glColor3ub(230,230,230);

	// Desenha a porta
	glPushMatrix();
	glTranslatef(-230,101,400);
	glScalef(1,0.99,1);
	glRotatef(-180,0,1,0);
	glRotated(180,0,0,1);
	glRotated(angle_door,0,1,0);
	glTranslatef(x_trans_angle,0,z_trans_angle);
	DesenhaObjeto(porta);
	glPopMatrix();

}

void DesenhaJanela(){
	
	// janela
	glPushMatrix();
	glTranslatef(-300,150,0);
	glRotatef(-90,0,1,0);
	DesenhaObjeto(janela);
	glPopMatrix();

	// porta esquerda da janela
	glPushMatrix();
	glTranslatef(-300,150, z_ej);
	glRotatef(90,0,1,0);
	glScalef(1,1.5,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// porta direita da janela
	glPushMatrix();
	glTranslatef(-300,150, z_dj);
	glRotatef(90,0,1,0);
	glScalef(1,1.5,1);
	DesenhaObjeto(plano);
	glPopMatrix();

}

void DesenhaObjObrigatorio(){

	// quadro
	glPushMatrix();
	glTranslatef(297,200,25);
	glRotatef(-90,0,1,0);
	glScalef(10,10,10);
	DesenhaObjeto(quadrop);
	glPopMatrix();

	// cama
	glPushMatrix();
	glTranslatef(180,34.5,-180);
	glRotatef(-90,0,1,0);
	glScalef(60,50,60);
	DesenhaObjeto(cama);
	glPopMatrix();

	// ventilador
	glPushMatrix();
	glTranslatef(270,38,-85);
	glRotatef(-135,0,1,0);
	glScalef(60,50,60);
	DesenhaObjeto(ventilador);
	glPopMatrix();
	
	// mesa
	glPushMatrix();
	glTranslatef(260,59.3,100);
	glRotatef(-90,0,1,0);
	DesenhaObjeto(mesa);
	glPopMatrix();

	// cadeira
	glPushMatrix();
	glTranslatef(240,47.3,100);
	glRotatef(90,0,1,0);
	DesenhaObjeto(cadeira);
	glPopMatrix();

}

void DesenhaObjExtra(){

	// lampada (obj1)
	glPushMatrix();
	glTranslatef(0,297,0);
	glRotatef(90,1,0,0);
	glRotatef(90,0,0,1);
	DesenhaObjeto(lamp);
	glPopMatrix();

	// quadro de estudo (obj2)
	glPushMatrix();
	glTranslatef(100,160,395);
	glRotatef(180,0,1,0);
	glScalef(0.5,0.5,2);
	DesenhaObjeto(quadro);
	glPopMatrix();

	// luminaria (obj3)
	glPushMatrix();
	glTranslatef(320,71.5,260);
	glRotatef(-110,0,1,0);
	glScalef(250,250,250);
	DesenhaObjeto(luminaria);
	glPopMatrix();

	// computador (obj4)
	glPushMatrix();
	glTranslatef(260,72,100);
	glRotatef(-90,0,1,0);
	glScalef(8,8,8);
	DesenhaObjeto(computador);
	glPopMatrix();

	// armario (obj5)
	glPushMatrix();
	glTranslatef(0,53,30);
	glRotatef(90,0,1,0);
	glScalef(130,130,130);
	DesenhaObjeto(armario);
	glPopMatrix();

}

// Desenha o chao
void DesenhaChao(void)
{
	glColor3ub(145,105,54);
	glPushMatrix();
	glTranslatef(0,0,0);
	glRotatef(-90,1,0,0);
	glScalef(6,8,1);
	DesenhaObjeto(plano);
	glPopMatrix();
}

// Desenha o teto
void DesenhaTeto(void)
{
	glColor3ub(212,199,183);
	glPushMatrix();
	glTranslatef(0,300,0);
	glRotatef(90,1,0,0);
	glScalef(6,8,1);
	DesenhaObjeto(plano);
	glPopMatrix();
}

// Desenha o quadro de estudo
void DesenhaQuadro()
{
	glPushMatrix();
	glTranslatef(0,160,398);
	glRotatef(180,0,1,0);
	DesenhaObjeto(quadro);
	glPopMatrix();
}

// Desenha toda a cena
void Desenha(void)
{
	// Limpa a janela de visualizacao com a cor
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Inicializa sistema de coordenadas do modelo
	// antes da execucao de qualquer operacao de manipulacao de matrizes
	glLoadIdentity();

	// Posiciona e orienta observador
	glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
	glTranslatef(-obsX,-obsY,-obsZ);

	// Agora posiciona a fonte de luz do meio da sala
	glLightfv( GL_LIGHT1, GL_POSITION, posLuz2 );

	// Desenha todos os elementos da cena
	DesenhaParedes();
	DesenhaChao();
	DesenhaTeto();
	DesenhaPorta();
	DesenhaJanela();
	DesenhaObjObrigatorio();
	DesenhaObjExtra();

	// Faz a troca dos buffers
	glutSwapBuffers();
}

// Funcao usada para especificar o volume de visualizacao
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projecao
	glMatrixMode(GL_PROJECTION);
	
	// Inicializa sistema de coordenadas de projecao
	glLoadIdentity();

	// Especifica a projecao perspectiva
	// (angulo, aspecto, zMin, zMax)
	gluPerspective(ang_cam,fAspect,0.1,1000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
}

// Funcao callback chamada quando o tamanho da janela e alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisao por zero
	if ( h == 0 ) h = 1;

	// Especifica as dimensoes da viewport
	glViewport(0, 0, w, h);

	// Calcula a correcao de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Funcao callback para eventos de teclado
void Teclado(unsigned char key, int x, int y)
{

	// Trata as diversas teclas
	switch(key)
	{
		case 'n':	
					if(angle_door < ANGLE_DOOR_MAX){
						angle_door = angle_door + 1;
						x_trans_angle = x_trans_angle + 0.5;
						z_trans_angle = z_trans_angle - 0.6;
					}
					break;
		case 'm':	
					if(angle_door > 0){
						angle_door = angle_door - 1;
						x_trans_angle = x_trans_angle - 0.5;
						z_trans_angle = z_trans_angle + 0.6;
					}
					break;
		case 'y':	
					if(z_ej > Z_LIM_DJ){
						z_ej = z_ej - 1;
					}

					break;
		case 'u':	
					if(z_ej < Z_LIM_EJ){
						z_ej = z_ej + 1; 
					}
					break;
		case 'h':
					if(z_dj < Z_LIM_EJ){
						z_dj = z_dj + 1;
					}
					break;		
		case 'j':
					if(z_dj > Z_LIM_DJ){
						z_dj = z_dj - 1; 
					}
					break;
		// Sai do programa
		case 27:	// Libera todos os objetos carregados...
					LiberaObjeto(NULL);
					// e materiais
					LiberaMateriais();
					exit(1);
				break;
	}
	// Na próxima iteração por meio de glutMainLoop essa janela será exibida novamente
	glutPostRedisplay();
}

// Funcao callback para eventos de teclas especiais
// navegacao atraves das setas
void TecladoEspecial(int key, int x, int y)
{
	float sina, cosa, sina_01, cosa_01;

	// Pre-calcula o seno e cosseno do angulo
	// de direcao atual + 90 graus, ou seja,
	// a direcao para deslocamento lateral
	sina = 5*sin((rotY+90)*M_PI/180.0);
	cosa = 5*cos((rotY+90)*M_PI/180.0);

	// Pre-calcula o seno e cosseno do angulo
	// sem os 90 graus
	sina_01 = 5*sin(rotY*M_PI/180.0);
	cosa_01 = 5*cos(rotY*M_PI/180.0);

	// Trata as teclas especiais
	switch(key)
	{
		// Controles de deslocamento lateral
		case GLUT_KEY_LEFT:		obsX = obsX - sina;
								obsZ = obsZ + cosa;
								break;
		case GLUT_KEY_RIGHT:	obsX = obsX + sina;
								obsZ = obsZ - cosa;
								break;
		case GLUT_KEY_UP:		obsX = obsX + sina_01;
								obsZ = obsZ - cosa_01;
								break;

		case GLUT_KEY_DOWN:		obsX = obsX - sina_01;
								obsZ = obsZ + cosa_01;
								break;
	}
	glutPostRedisplay();
}

// Funcao callback para eventos de botoes do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)
	{
		// Atualiza as variaveis de navegação com os valores atuais
		x_ini = x;
		y_ini = y;
		obsY_ini = obsY;
		rotX_ini = rotX;
		rotY_ini = rotY;
		bot = button;
	}
	else bot = -1;
}

// Funcao callback para eventos de movimento do mouse
#define SENS_ROT	5.0
#define SENS_OBS	10.0

void GerenciaMovim(int x, int y)
{
	// Botao esquerdo ?
	if(bot==GLUT_LEFT_BUTTON)
	{
		// Calcula diferencas
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica angulos
		rotY = rotY_ini - deltax/SENS_ROT;
		rotX = rotX_ini - deltay/SENS_ROT;
	}
	// Botao direito ?
	else if(bot==GLUT_RIGHT_BUTTON)
	{
		// Calcula diferenca
		int deltay = y_ini - y;
		// E modifica altura do observador
		obsY = obsY_ini + deltay/SENS_OBS;
	}
	glutPostRedisplay();
}

// Funcao responsavel por inicializar parametros e variaveis
void Inicializa(void)
{
	// Define a cor de fundo da janela de visualizacao como preto
	glClearColor(0,0,0,1);

	// Ajusta iluminacao
	glLightfv( GL_LIGHT0, GL_AMBIENT,  luzAmb1 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  luzDif1 );

	glLightfv( GL_LIGHT1, GL_AMBIENT,  luzAmb1 );
	glLightfv( GL_LIGHT1, GL_DIFFUSE,  luzDif2 );

	glLightfv( GL_LIGHT2, GL_AMBIENT,  luzAmb1 );

	glLightfv( GL_LIGHT3, GL_AMBIENT,  luzAmb1 );

	glLightfv( GL_LIGHT4, GL_AMBIENT,  luzAmb1 );

	// Habilita todas as fontes de luz
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	// Faz com que seja utilizado os parametros atuais
	// para calcular a cor ou indice do vertice
	glEnable(GL_LIGHTING);

	// Seleciona o modo de GL_COLOR_MATERIAL
	// faz com que uma cor de material acompanhe a cor atual
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Habilita normalizacao automatica
	// Vetores normais sao normalizados para valores unitarios
	// apos transformacao e antes da iluminacao
	glEnable(GL_NORMALIZE);

	// Habilita Z-Buffer
	// Realiza comparacoes de profundidade
	// e atualiza o buffer de profundidade
	glEnable(GL_DEPTH_TEST);

	// Carrega objetos
	plano = CarregaObjeto("obj/parede.obj",false);
	mesa = CarregaObjeto("obj/mesa.obj",false);
	cadeira = CarregaObjeto("obj/cadeira.obj",false);
	quadro = CarregaObjeto("obj/quadronegro.obj",false);
	porta = CarregaObjeto("obj/porta.obj",false);
	janela = CarregaObjeto("obj/janela.obj",false);
	lamp = CarregaObjeto("obj/lampada.obj",false);
	cama = CarregaObjeto("obj/cama.obj", false);
	ventilador = CarregaObjeto("obj/ventilador.obj", false);
	cabeceira = CarregaObjeto("obj/cabeceira.obj", false);
	quadrop = CarregaObjeto("obj/quadrop.obj", false);
	luminaria = CarregaObjeto("obj/luminaria.obj", false);
	computador = CarregaObjeto("obj/computador.obj", false);
	armario = CarregaObjeto("obj/armario.obj", false);

}

// Programa Principal
int main(int argc, char** argv)
{
	// Inicilizar a Glut
	glutInit(&argc, argv);

	// Define o modo de operacao da GLUT
	// GLUT_DOUBLE : dois buffers de cor
	// GLUT_RGB : define que as cores do programa serão especificadas por componentes GLUT_RGB
	// GLUT_DEPTH : buffer de profundidade
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Especifica a posicao e o tamanho inicial em pixels da janela GLUT
	glutInitWindowPosition(100,70);
	glutInitWindowSize(1700,900);

	// Cria a janela passando como argumento o titulo da mesma
	glutCreateWindow("John's bedroom");

	// Registra a funcao callback de redesenho da janela de visualizacao
	glutDisplayFunc(Desenha);

	// Registra a funcao callback de teclado
	glutKeyboardFunc(Teclado);

	// Registra a funcao callback de teclas especiais
	glutSpecialFunc(TecladoEspecial);
	
	// Registra a funcao callback para eventos de botoes do mouse
	glutMouseFunc(GerenciaMouse);
	
	// Registra a funcao callback para eventos de movimento do mouse
	glutMotionFunc(GerenciaMovim);
	
	// Registra a funcao callback de redimensionamento da janela de visualizacao
	glutReshapeFunc(AlteraTamanhoJanela);

	// Chama a funcao responsavel por fazer as inicializacoes
	Inicializa();

	// Inicia o processamento e aguarda interacoes do usuario
	glutMainLoop();
	
	return 0;
}