#include <GL/glut.h>
#include <math.h>
#include "bibutilNoTex.h"
#include "bibutil.h"
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

// Define as texturas 2D
TEX *parede, *chao, *teto, *pintura, *lousa, *monitor, *janelaE,
 *janelaD, *madeiraArmario, *lencol, *ventiladorTex, *heliceTex, *luminariaTex;

// Filtros de textura
GLint filtros[] = {
       GL_NEAREST, GL_LINEAR,
       GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,
       GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
};

// Define filtro inicial como GL_NEAREST_MIPMAP_LINEAR
int filtro = 4;

// Define modo inicial de aplicacao da textura
GLint modo = GL_MODULATE;

// Define modo de desenho inicial: textura
char modo_des = 't';

void SetaEscalaTextura(float x,float y)
{
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(x,y,1);
	glMatrixMode(GL_MODELVIEW);
}

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

// Controle da helice
GLfloat angulo;
GLfloat sentido = 1;

// Objetos
OBJ *plano, *mesa, *cadeira, *quadro, *porta, *janela, *lamp,
 *cabeceira, *quadrop, *luminaria, *computador, *armario, *telaComp, *telaLousa;

OBJnotex *ventilador, *cama, *helice;

// Luminosidade base de uma lampada
#define LOW	0.3

// Define parametros de iluminacao
// Luz 1: puntual no teto, frente
GLfloat luzAmb1[4] = { 0.1, 0.1, 0.1, 1 };	// luz ambiente
GLfloat luzDif1[4] = { LOW, LOW, LOW, 1.0 };	// luz difusa
GLfloat luzEsp1[4] = { 0.0, 0.0, 0.0, 1.0 };	// luz especular
GLfloat posLuz1[4] = { 0, 200, 250, 1 };	// posicao da fonte de luz
// Luz 2: puntual no teto, meio da sala
GLfloat luzDif2[4] = { LOW, LOW, LOW, 1.0 };	// luz difusa
GLfloat posLuz2[4] = { 0, 200, 0, 1 };	// posicao da fonte de luz
// Luz 3: puntual no teto, atras
GLfloat luzDif3[4] = { LOW, LOW, LOW, 1.0 };	// luz difusa
GLfloat posLuz3[4] = { 0, 200, -250, 1 };	// posicao da fonte de luz

// Luz 4: direcional, simulando o Sol no chão
GLfloat luzDif4[4] = { 0.4, 0.2, 0.0, 1.0 };	// luz difusa
GLfloat posLuz4[4] = { -1, 0.4 , 0, 0 };		// direcao da fonte de luz

// Luz spot da luminaria
GLfloat luzAmbSpot[4]= {0.2,0.2,0.2,1.0};
GLfloat posLuzSpot[4] = {  260,120,65, 1 };
GLfloat dirLuzSpot[3] = { 0, -1,0 };
GLfloat luzDifusaSpot[4] = { 1,1,1,1 };
GLfloat luzEspecSpot[4] = { 0,0,0,1 };

bool luzes[6] = {true, true, true, false, false};

// Define variaveis para navegacao
GLfloat rotX=0, rotY=0, rotX_ini, rotY_ini;
GLfloat obsX=0, obsY=150, obsZ=100, obsY_ini;
int x_ini,y_ini,bot;

// Arquivo de cena e arquivo de camera

// Desenha 4 paredes
void DesenhaParedes(void)
{	

	SetaEscalaTextura(6,3);

	if(modo_des=='t')
		glColor3f(1,1,1);
	else
		glColor3ub(196,210,184);

	// Parede dos fundos
	// Insere a matriz de transformação corrente na pilha
	glPushMatrix();
	glTranslatef(0,150,-400);
	glScalef(6,3,1);
	plano->textura = parede->texid;
	DesenhaObjeto(plano);

	// Retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	glPopMatrix();

	SetaEscalaTextura(4.76,2);

	// Parede da frente
	glPushMatrix();
	glTranslatef(62.5,100,400);
	glRotatef(180,0,1,0);
	glScalef(4.76,2,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	SetaEscalaTextura(6,1);

	// Parte superior da frente
	glPushMatrix();
	glTranslatef(0,250,400);
	glRotatef(180,0,1,0);
	glScalef(6,1,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	// Parte direita da frente

	SetaEscalaTextura(0.265,2);

	glPushMatrix();
	glTranslatef(-286.7,100,400);
	glRotatef(180,0,1,0);
	glScalef(0.265,2,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	SetaEscalaTextura(8,3);

	// Parede esquerda
	glPushMatrix();
	glTranslatef(300,150,0);
	glRotatef(-90,0,1,0);
	glScalef(8,3,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	SetaEscalaTextura(8,0.75);

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

	SetaEscalaTextura(2.95,1.5);

	// antes do meio
	glPushMatrix();
	glTranslatef(-300,150,-254);
	glRotatef(90,0,1,0);
	glScalef(2.95,1.5,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	SetaEscalaTextura(2.95,1.5);

	// depois do meio
	glPushMatrix();
	glTranslatef(-300,150,254);
	glRotatef(90,0,1,0);
	glScalef(2.95,1.5,1);
	DesenhaObjeto(plano);
	glPopMatrix();

	SetaEscalaTextura(1,1);

}

void DesenhaPorta(){


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

	if(modo_des=='t')
		glColor3f(1,1,1);
	else
		glColor3ub(196,210,184);

	SetaEscalaTextura(1,1);

	// porta esquerda da janela
	glPushMatrix();
	glTranslatef(-300,150, z_ej);
	glRotatef(90,0,1,0);
	glScalef(1,1.5,1);
	plano->textura = janelaE->texid;
	DesenhaObjeto(plano);
	glPopMatrix();

	// porta direita da janela
	glPushMatrix();
	glTranslatef(-301,150, z_dj);
	glRotatef(90,0,1,0);
	glScalef(1,1.5,1);
	plano->textura = janelaD->texid;
	DesenhaObjeto(plano);
	glPopMatrix();

}

void DesenhaObjObrigatorio(){

	if(modo_des=='t')
		glColor3f(1,1,1);
	else
		glColor3ub(196,210,184);


	SetaEscalaTextura(1,1);
	// quadro
	glPushMatrix();
	glTranslatef(297,200,25);
	glRotatef(-90,0,1,0);
	glScalef(10,10,10);
	quadrop->textura = pintura->texid;
	DesenhaObjeto(quadrop);
	glPopMatrix();

	// cama
	glPushMatrix();
	glTranslatef(180,34.5,-180);
	glRotatef(-90,0,1,0);
	glScalef(60,50,60);
	cama->textura = lencol->texid;
	DesenhaObjetoNoTex(cama);
	glPopMatrix();

	
	// ventilador
	glPushMatrix();
	glTranslatef(270,38,-85);
	glRotatef(-135,0,1,0);
	glScalef(60,50,60);
	ventilador->textura = ventiladorTex->texid;
	DesenhaObjetoNoTex(ventilador);
	glPopMatrix();
	
	
	// helice
	glPushMatrix();
	glTranslatef(264,58,-91);
	glRotatef(90, 0, 0, 1);
	glRotatef(-45, 1, 0, 0);
	glRotatef(angulo, 0, 1, 0);
	glScalef(0.28,0.28,0.28);
	helice->textura = heliceTex->texid;
	DesenhaObjetoNoTex(helice);
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

	// lousa de estudo (obj2)
	glPushMatrix();
	glTranslatef(100,160,395);
	glRotatef(180,0,1,0);
	glScalef(0.5,0.5,2);
	DesenhaObjeto(quadro);
	glPopMatrix();

	// tela da lousa
	glPushMatrix();
	glTranslatef(100,160,393);
	glRotatef(180,0,1,0);
	glScalef(2.5,1,2);
	telaLousa->textura = lousa->texid;
	DesenhaObjeto(telaLousa);
	glPopMatrix();

	// luminaria (obj3)
	glPushMatrix();
	glTranslatef(320,71.5,260);
	glRotatef(-110,0,1,0);
	glScalef(250,250,250);
	luminaria->textura = luminariaTex->texid;
	DesenhaObjeto(luminaria);
	glPopMatrix();

	// computador (obj4)
	glPushMatrix();
	glTranslatef(260,72,100);
	glRotatef(-90,0,1,0);
	glScalef(8,8,8);
	DesenhaObjeto(computador);
	glPopMatrix();

	// tela computador
	glPushMatrix();
	glTranslatef(278.1,90,100);
	glRotatef(-90,0,1,0);
	glScalef(0.45,0.29,1);
	telaComp->textura = monitor->texid;
	DesenhaObjeto(telaComp);
	glPopMatrix();

	if(modo_des=='t')
		glColor3f(1,1,1);
	else
		glColor3ub(196,210,184);

	// armario (obj5)
	glPushMatrix();
	glTranslatef(0,53,30);
	glRotatef(90,0,1,0);
	glScalef(130,130,130);
	armario->textura = madeiraArmario->texid;
	DesenhaObjeto(armario);
	glPopMatrix();

}

// Desenha o chao
void DesenhaChao(void)
{
	SetaEscalaTextura(8,8);

	if(modo_des=='t')
		glColor3f(1,1,1);
	else
		glColor3ub(145,105,54);

	glPushMatrix();
	glTranslatef(0,0,0);
	glRotatef(-90,1,0,0);
	glScalef(6,8,1);
	plano->textura = chao->texid;
	DesenhaObjeto(plano);
	glPopMatrix();
}

// Desenha o teto
void DesenhaTeto(void)
{	

	if(modo_des=='t')
		glColor3f(1,1,1);
	else
		glColor3ub(212,199,183);


	glPushMatrix();
	glTranslatef(0,300,0);
	glRotatef(90,1,0,0);
	glScalef(6,8,1);
	plano->textura = teto->texid;
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

	glEnable(GL_TEXTURE_2D);

	// Agora posiciona a fonte de luz do meio da sala
	// Agora posiciona demais fontes de luz
	glLightfv( GL_LIGHT0, GL_POSITION, posLuz1 );
	glLightfv( GL_LIGHT1, GL_POSITION, posLuz2 );
	glLightfv( GL_LIGHT2, GL_POSITION, posLuz3 );
	glLightfv( GL_LIGHT3, GL_POSITION, posLuz4 );
	
	// Luz spot
	glLightfv( GL_LIGHT4, GL_POSITION, posLuzSpot );
	glLightfv( GL_LIGHT4, GL_DIFFUSE, luzDifusaSpot );
	glLightfv( GL_LIGHT4, GL_SPECULAR, luzEspecSpot );
	glLightfv( GL_LIGHT4, GL_SPOT_DIRECTION, dirLuzSpot );
	glLightf( GL_LIGHT4, GL_SPOT_CUTOFF, 70.0 );
	glLightf( GL_LIGHT4, GL_SPOT_EXPONENT, 10 );

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


// Liga/desliga luzes de acordo com o estado
void SetaLuzes()
{
	for(int luz=0;luz<5;++luz)
	{
		if(luzes[luz]) glEnable(GL_LIGHT0+luz);
		else glDisable(GL_LIGHT0+luz);
	}
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
		case 'f':
					if(sentido){
						sentido = 0;
					} else {
						sentido = 1;
					}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
					luzes[key-'1'] = !luzes[key-'1'];
					SetaLuzes();
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
		case GLUT_KEY_LEFT:		
							obsX = obsX - sina;
							obsZ = obsZ + cosa;
							break;
		case GLUT_KEY_RIGHT:	
							obsX = obsX + sina;
							obsZ = obsZ - cosa;
							break;
		case GLUT_KEY_UP:		
							obsX = obsX + sina_01;
							obsZ = obsZ - cosa_01;
							break;

		case GLUT_KEY_DOWN:		
							obsX = obsX - sina_01;
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

void Anima(int value)
{	

	// Muda o angulo de rotacao, e se chegar a 360, passa para zero
	if(sentido){
		if( --angulo > 360.0f )
			angulo = 0.0f;
	} else {
		if( ++angulo > 360.0f )
			angulo = 0.0f;
	}

	glutPostRedisplay();
	glutTimerFunc(5,Anima, 1);
}

// Funcao responsavel por inicializar parametros e variaveis
void Inicializa(void)
{
	// Define a cor de fundo da janela de visualizacao como preto
	glClearColor(0,0,0,1);

	// Carrega as texturas
	madeiraArmario = CarregaTextura("texturas/armario.jpg", true);
	parede = CarregaTextura("texturas/parede.jpg",true);
	chao   = CarregaTextura("texturas/chao.jpg",true);
	teto   = CarregaTextura("texturas/teto.jpg",true);
	pintura = CarregaTextura("texturas/gogh.jpg", true);
	lousa = CarregaTextura("texturas/calculosLousa.jpg", true);
	monitor = CarregaTextura("texturas/monitor.jpg", true);
	janelaE = CarregaTextura("texturas/janelaE.jpg", true);
	janelaD = CarregaTextura("texturas/janelaD.jpg", true);
	lencol = CarregaTextura("texturas/lencol.jpg", true);
	heliceTex = CarregaTextura("texturas/helice.jpg", true);
	ventiladorTex = CarregaTextura("texturas/ventilador.jpg", true);
	luminariaTex = CarregaTextura("texturas/luminaria.jpg", true);

	// Seleciona o modo de aplicacao da textura
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modo);

	// Ajusta iluminacao
	glLightfv( GL_LIGHT0, GL_AMBIENT,  luzAmb1 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  luzDif1 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, luzEsp1 );
	glLightfv( GL_LIGHT1, GL_AMBIENT,  luzAmb1 );
	glLightfv( GL_LIGHT1, GL_DIFFUSE,  luzDif2 );
	glLightfv( GL_LIGHT2, GL_AMBIENT,  luzAmb1 );
	glLightfv( GL_LIGHT2, GL_DIFFUSE,  luzDif3 );
	glLightfv( GL_LIGHT3, GL_AMBIENT,  luzAmb1 );
	glLightfv( GL_LIGHT3, GL_DIFFUSE,  luzDif4 );

	// Habilita todas as fontes de luz
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHTING);

	// Define coeficientes ambiente e difuso
	// do material
	GLfloat matAmb[4] = { 0.2,0.2,0.2,1 };
	GLfloat matDif[4] = { 1,1,1,1 };

	// Material
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,matAmb);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,matDif);

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
	plano = CarregaObjeto("obj/parede.obj", true);
	telaLousa = CarregaObjeto("obj/parede.obj", true);
	telaComp = CarregaObjeto("obj/parede.obj", true);
	mesa = CarregaObjeto("obj/mesa.obj",true);
	cadeira = CarregaObjeto("obj/cadeira.obj",true);
	quadro = CarregaObjeto("obj/quadronegro.obj",false);
	porta = CarregaObjeto("obj/porta.obj",false);
	janela = CarregaObjeto("obj/janela.obj",false);
	lamp = CarregaObjeto("obj/lampada.obj",false);
	cama = CarregaObjetoNoTex("obj/cama.obj", false);
	ventilador = CarregaObjetoNoTex("obj/ventilador.obj", false);
	cabeceira = CarregaObjeto("obj/cabeceira.obj", false);
	quadrop = CarregaObjeto("obj/quadrop.obj", false);
	luminaria = CarregaObjeto("obj/luminaria.obj", false);
	computador = CarregaObjeto("obj/computador.obj", false);
	armario = CarregaObjeto("obj/armario.obj", false);
	helice = CarregaObjetoNoTex("obj/helice.obj", false);

	SetaLuzes();
	SetaModoDesenho(modo_des);

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
	
	glutTimerFunc(60, Anima, 1);

	// Registra a funcao callback de redimensionamento da janela de visualizacao
	glutReshapeFunc(AlteraTamanhoJanela);

	// Chama a funcao responsavel por fazer as inicializacoes
	Inicializa();

	// Inicia o processamento e aguarda interacoes do usuario
	glutMainLoop();
	
	return 0;
}