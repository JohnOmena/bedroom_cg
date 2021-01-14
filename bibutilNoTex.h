#ifndef __BIBUTILNOTEX_H
#define __BIBUTILNOTEX_H
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
} TEXnotex;

typedef struct {
	GLfloat x,y,z;
} VERTnotex;

typedef struct {
	GLint nv;		
	GLint *vert;	
	GLint *norm;	
	GLint *tex;		
	GLint mat;		
	GLint texid;	
} FACEnotex;

typedef struct {
	GLfloat s,t,r;
} TEXCOORDnotex;

typedef struct {
	GLint numVertices;
	GLint numFaces;
	GLint numNormais;
	GLint numTexcoords;
	bool normais_por_vertice;	
	bool tem_materiais;			
	GLint textura;				
	GLint dlist;				
	VERTnotex *vertices;
	VERTnotex *normais;
	FACEnotex *faces;
	TEXCOORDnotex *texcoords;
} OBJnotex;

// Define um material
typedef struct {
	char nome[20];	
	GLfloat ka[4];	
	GLfloat kd[4];	
	GLfloat ks[4];	
	GLfloat ke[4];	
	GLfloat spec;	
} MATnotex;


void NormalizaNoTex(VERTnotex &norm);
void ProdutoVetorialNoTex(VERTnotex &v1, VERTnotex &v2, VERTnotex &vresult);
void VetorNormalNoTex(VERTnotex vert1, VERTnotex vert2, VERTnotex vert3, VERTnotex &n);
void RotaZNoTex(VERTnotex &in, VERTnotex &out, float ang);
void RotaYNoTex(VERTnotex &in, VERTnotex &out, float ang);
void RotaXNoTex(VERTnotex &in, VERTnotex &out, float ang);

OBJnotex *CarregaObjetoNoTex(char *nomeArquivo, bool mipmap);
void CriaDisplayListNoTex(OBJnotex *obj);
void DesabilitaDisplayListNoTex(OBJnotex *ptr);
void DesenhaObjetoNoTex(OBJnotex *obj);
void SetaModoDesenhoNoTex(char modo);

void LiberaObjetoNoTex(OBJnotex *obj);
void LiberaMateriaisNoTex();


float CalculaQPSNoTex(void);
void Escreve2DNoTex(float x, float y, char *str);

void CalculaNormaisPorFaceNoTex(OBJnotex *obj);

void SetaFiltroTexturaNoTex(GLint tex, GLint filtromin, GLint filtromag);
MATnotex *ProcuraMaterialNoTex(char *nome);

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
