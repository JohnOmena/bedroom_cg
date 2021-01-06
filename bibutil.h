#ifndef __BIBUTIL_H
#define __BIBUTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

typedef struct
{
	char nome[50];			
	int ncomp;				
	GLint dimx;				
	GLint dimy;				
	GLuint texid;			
	unsigned char *data;	
} TEX;

typedef struct {
	GLfloat x,y,z;
} VERT;

typedef struct {
	GLint nv;		
	GLint *vert;	
	GLint *norm;	
	GLint *tex;		
	GLint mat;		
	GLint texid;	
} FACE;

typedef struct {
	GLfloat s,t,r;
} TEXCOORD;

typedef struct {
	GLint numVertices;
	GLint numFaces;
	GLint numNormais;
	GLint numTexcoords;
	bool normais_por_vertice;	
	bool tem_materiais;			
	GLint textura;				
	GLint dlist;				
	VERT *vertices;
	VERT *normais;
	FACE *faces;
	TEXCOORD *texcoords;
} OBJ;

// Define um material
typedef struct {
	char nome[20];	
	GLfloat ka[4];	
	GLfloat kd[4];	
	GLfloat ks[4];	
	GLfloat ke[4];	
	GLfloat spec;	
} MAT;


void Normaliza(VERT &norm);
void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult);
void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n);
void RotaZ(VERT &in, VERT &out, float ang);
void RotaY(VERT &in, VERT &out, float ang);
void RotaX(VERT &in, VERT &out, float ang);

OBJ *CarregaObjeto(char *nomeArquivo, bool mipmap);
void CriaDisplayList(OBJ *obj);
void DesabilitaDisplayList(OBJ *ptr);
void DesenhaObjeto(OBJ *obj);
void SetaModoDesenho(char modo);

void LiberaObjeto(OBJ *obj);
void LiberaMateriais();


float CalculaQPS(void);
void Escreve2D(float x, float y, char *str);

void CalculaNormaisPorFace(OBJ *obj);

void SetaFiltroTextura(GLint tex, GLint filtromin, GLint filtromag);
MAT *ProcuraMaterial(char *nome);

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
