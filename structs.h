#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

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
