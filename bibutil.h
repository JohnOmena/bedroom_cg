#ifndef __BIBUTIL_H
#define __BIBUTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

// Define a estrutura de uma imagem
typedef struct
{
	char nome[50];			// nome do arquivo carregado
	int ncomp;				// numero de componentes na textura (1-intensidade, 3-RGB)
	GLint dimx;				// largura 
	GLint dimy;				// altura
	GLuint texid;			// identificao da textura em OpenGL
	unsigned char *data;	// apontador para a imagem em si
} TEX;

// Define a estrutura de um v�rtice
typedef struct {
	GLfloat x,y,z;
} VERT;

// Define a estrutura de uma face
typedef struct {
	GLint nv;		// numero de vertices na face
	GLint *vert;	// indices dos vertices
	GLint *norm;	// indices das normais
	GLint *tex;		// indices das texcoords
	GLint mat;		// indice para o material (se houver)
	GLint texid;	// indice para a textura (se houver)
} FACE;

// Define a estrutura de uma coordenada
// de textura (s,t,r) - r nao e usado
typedef struct {
	GLfloat s,t,r;
} TEXCOORD;

// Define a estrutura de um objeto 3D
typedef struct {
	GLint numVertices;
	GLint numFaces;
	GLint numNormais;
	GLint numTexcoords;
	bool normais_por_vertice;	// true se houver normais por vertice
	bool tem_materiais;			// true se houver materiais
	GLint textura;				// contem a id da textura a utilizar, caso o objeto nao tenha textura associada
	GLint dlist;				// display list, se houver
	VERT *vertices;
	VERT *normais;
	FACE *faces;
	TEXCOORD *texcoords;
} OBJ;

// Define um material
typedef struct {
	char nome[20];	// Identificacao do material
	GLfloat ka[4];	// Ambiente
	GLfloat kd[4];	// Difuso
	GLfloat ks[4];	// Especular
	GLfloat ke[4];	// Emissao
	GLfloat spec;	// Fator de especularidade
} MAT;

// Prototipos das funcoes
// Funcaes para calculos diversos
void Normaliza(VERT &norm);
void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult);
void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n);
void RotaZ(VERT &in, VERT &out, float ang);
void RotaY(VERT &in, VERT &out, float ang);
void RotaX(VERT &in, VERT &out, float ang);

// Funcoes para carga e desenho de objetos
OBJ *CarregaObjeto(char *nomeArquivo, bool mipmap);
void CriaDisplayList(OBJ *obj);
void DesabilitaDisplayList(OBJ *ptr);
void DesenhaObjeto(OBJ *obj);
void SetaModoDesenho(char modo);

// Funçoes para liberacao de memoria
void LiberaObjeto(OBJ *obj);
void LiberaMateriais();


// Funcaes para calculo e exibicao da taxa de quadros por segundo
float CalculaQPS(void);
void Escreve2D(float x, float y, char *str);

// Funcoes para calculo de normais
void CalculaNormaisPorFace(OBJ *obj);

// Funcoes para manipulacao de texturas e materiais
void SetaFiltroTextura(GLint tex, GLint filtromin, GLint filtromag);
MAT *ProcuraMaterial(char *nome);

// Constantes utilizadas caso n�o existam em GL/gl.h
#ifndef GL_ARB_texture_cube_map
# define GL_NORMAL_MAP					0x8511
# define GL_REFLECTION_MAP				0x8512
# define GL_TEXTURE_CUBE_MAP			0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP	0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X	0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X	0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y	0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z	0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP		0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE	0x851C
#endif

#endif
