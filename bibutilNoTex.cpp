#include <math.h>
#include <string.h>
#include <vector>
#include "bibutilNoTex.h"

#define DEBUG

using namespace std;


vector<OBJnotex*> _objetosNoTex(0);


vector<MATnotex*> _materiaisNoTex(0);

vector<TEXnotex*> _texturasNoTex(0);


char _modoNoTex = 't';

int _numquadroNoTex =0, _tempoNoTex, _tempoAnteriorNoTex = 0;
float _ultqpsNoTex = 0;

#ifndef __FREEGLUT_EXT_H__

void glutBitmapStringNoTex(void *fonte,char *texto)
{
	
	while (*texto)
    	glutBitmapCharacter(fonte, *texto++);
}
#endif


float CalculaQPSNoTex(void)
{
	
	_numquadroNoTex++;

	
	_tempoNoTex = glutGet(GLUT_ELAPSED_TIME);
	
	if (_tempoNoTex - _tempoAnteriorNoTex > 1000)
	{
		
		_ultqpsNoTex = _numquadroNoTex*1000.0/(_tempoNoTex - _tempoAnteriorNoTex);
		
	 	_tempoAnteriorNoTex = _tempoNoTex;
		_numquadroNoTex = 0;
	}
	
	return _ultqpsNoTex;
}


void Escreve2DNoTex(float x, float y, char *str)
{
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();
	glLoadIdentity();
	
	gluOrtho2D(0,1,0,1);
	
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glLoadIdentity();

	
	glRasterPos2f(x,y);
	glColor3f(0,0,0);
	
	glutBitmapStringNoTex(GLUT_BITMAP_9_BY_15,str);
	
	glMatrixMode(GL_PROJECTION);
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	glPopMatrix();
}

void NormalizaNoTex(VERTnotex &norm)
{
	float tam = sqrt(norm.x*norm.x
			+norm.y*norm.y
			+norm.z*norm.z);
	if(tam == 0) return;
	norm.x /= tam;
	norm.y /= tam;
	norm.z /= tam;
}

void ProdutoVetorialNoTex(VERTnotex &v1, VERTnotex &v2, VERTnotex &vresult)
{
	vresult.x = v1.y * v2.z - v1.z * v2.y;
	vresult.y = v1.z * v2.x - v1.x * v2.z;
	vresult.z = v1.x * v2.y - v1.y * v2.x;
}

void VetorNormalNoTex(VERTnotex vert1, VERTnotex vert2, VERTnotex vert3, VERTnotex &n)
{
    VERTnotex vet1, vet2;
    
    vet1.x = vert1.x - vert2.x;
    vet1.y = vert1.y - vert2.y;
    vet1.z = vert1.z - vert2.z;
    vet2.x = vert3.x - vert2.x;
    vet2.y = vert3.y - vert2.y;
    vet2.z = vert3.z - vert2.z;
    ProdutoVetorialNoTex(vet2, vet1, n);
    NormalizaNoTex(n);
}

void RotaZNoTex(VERTnotex &in, VERTnotex &out, float ang)
{
	float arad = ang*M_PI/180.0;
	out.x =  in.x * cos(arad) + in.y * sin(arad);
	out.y = -in.x * sin(arad) + in.y * cos(arad);
	out.z =  in.z;
}

void RotaYNoTex(VERTnotex &in, VERTnotex &out, float ang)
{
	float arad = ang*M_PI/180.0;
	out.x = in.x * cos(arad) - in.z * sin(arad);
	out.y = in.y;
	out.z = in.x * sin(arad) + in.z * cos(arad);
}

void RotaXNoTex(VERTnotex &in, VERTnotex &out, float ang)
{
	float arad = ang*M_PI/180.0;
	out.x =  in.x;
	out.y =  in.y * cos(arad) + in.z * sin(arad);
	out.z = -in.y * sin(arad) + in.z * cos(arad);
}

int leNumNoTex(char **face, char *sep)
{
	char temp[10];
	int pos = 0;
	while(**face)
	{
		if(**face == '/' || **face==' ') 
		{
			*sep = **face;
			(*face)++;
			break;
		}
		temp[pos++] = **face;
		(*face)++;
	}
	temp[pos]=0;
	if(!pos) 
		return -1;
	return atoi(temp);
}


int _procuraMaterialNoTex(char *nome)
{
	unsigned int i;
	for(i=0;i<_materiaisNoTex.size();++i)
		if(!strcmp(nome,_materiaisNoTex[i]->nome))
			return i;
	return -1;
}


MATnotex *ProcuraMaterialNoTex(char *nome)
{
	int pos = _procuraMaterialNoTex(nome);
	if(pos == -1) return NULL;
	else return _materiaisNoTex[pos];
}


int _procuraTexturaNoTex(char *nome)
{
	unsigned int i;
	for(i=0;i<_texturasNoTex.size();++i)
		if(!strcmp(nome,_texturasNoTex[i]->nome))
			return i;
	return -1;
}


void _leMateriaisNoTex(char *nomeArquivo)
{
	char aux[256];
	FILE *fp;
	MATnotex *ptr;
	fp = fopen(nomeArquivo,"r");

	while(!feof(fp))
	{
		fgets(aux,255,fp);
		if (strlen(aux)>1)
			aux[strlen(aux)-1]=0;	
		if(aux[0]=='#') continue;
		if(!strncmp(aux,"newmtl",6)) 
		{

			if(_procuraMaterialNoTex(&aux[7])!=-1)
			{
				ptr = NULL;
				continue;
			}
			if((ptr = (MATnotex *) malloc(sizeof(MATnotex)))==NULL)
			{
				exit(1);
			}
			_materiaisNoTex.push_back(ptr);
			strcpy(ptr->nome,&aux[7]);

			ptr->ke[0] = ptr->ke[1] = ptr->ke[2] = 0.0;
		}
		if(!strncmp(aux,"Ka ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Ka %f %f %f",&ptr->ka[0],&ptr->ka[1],&ptr->ka[2]);
		}
		if(!strncmp(aux,"Kd ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Kd %f %f %f",&ptr->kd[0],&ptr->kd[1],&ptr->kd[2]);
		}
		if(!strncmp(aux,"Ks ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Ks %f %f %f",&ptr->ks[0],&ptr->ks[1],&ptr->ks[2]);
		}
		if(!strncmp(aux,"Ns ",3)) 
		{
			if(ptr==NULL) continue;
			sscanf(aux,"Ns %f",&ptr->spec);

			ptr->spec = ptr->spec/1000.0 * 128;
		}
		if(!strncmp(aux,"d ",2)) 
		{
			if(ptr==NULL) continue;

			float alpha;
			sscanf(aux,"d %f",&alpha);
			ptr->ka[3] = alpha;
			ptr->kd[3] = alpha;
			ptr->ks[3] = alpha;
		}
	}
	fclose(fp);
}

OBJnotex *CarregaObjetoNoTex(char *nomeArquivo, bool mipmap)
{	
	
	int i;
	int vcont,ncont,fcont,tcont;
	int material, texid;
	char aux[256];
	TEXnotex *ptr;
	FILE *fp;
	OBJnotex *obj;

	fp = fopen(nomeArquivo, "r");  

#ifdef DEBUG
#endif

	if(fp == NULL){
          return NULL;
	}

	if ( ( obj = (OBJnotex *) malloc(sizeof(OBJnotex)) ) == NULL){
		return NULL;
	}

	obj->numVertices  = 0;
	obj->numFaces     = 0;
	obj->numNormais   = 0;
	obj->numTexcoords = 0;
	obj->normais_por_vertice = false;
	obj->tem_materiais = false;
	obj->textura = -1;	
	obj->dlist = -1;	

	obj->vertices = NULL;
	obj->faces = NULL;
	obj->normais = NULL;
	obj->texcoords = NULL;

	while(!feof(fp))
	{
		fgets(aux,255,fp);
		if(!strncmp(aux,"v ",2)) 
			obj->numVertices++;
		if(!strncmp(aux,"f ",2))
			obj->numFaces++;
		if(!strncmp(aux,"vn ",3)) 
			obj->numNormais++;
		if(!strncmp(aux,"vt ",3)) 
			obj->numTexcoords++;
	}

	rewind(fp);

#ifdef DEBUG
#endif

	
	if ( ( obj->vertices = (VERTnotex *) malloc((sizeof(VERTnotex)) * obj->numVertices) ) == NULL ){
		return NULL;
	}


	if ( ( obj->faces = (FACEnotex *) malloc((sizeof(FACEnotex)) * obj->numFaces) ) == NULL ){
		return NULL;
	}


	if(obj->numNormais){
		if ( ( obj->normais = (VERTnotex *) malloc((sizeof(VERTnotex)) * obj->numNormais) ) == NULL )
			return NULL;
	}

	if(obj->numTexcoords){
		if ( ( obj->texcoords = (TEXCOORDnotex *) malloc((sizeof(TEXCOORDnotex)) * obj->numTexcoords) ) == NULL )
			return NULL;
	}


	vcont = 0;
	ncont = 0;
	tcont = 0;
	fcont = 0;
	material = -1;
	texid = -1;

	
	float minx,miny,minz;
	float maxx,maxy,maxz;


	while(!feof(fp))
	{
		fgets(aux,255,fp);
		aux[strlen(aux)-1]=0;	

		if(aux[0]=='#') continue;

		if(!strncmp(aux,"v ",2))
		{
			sscanf(aux,"v %f %f %f",&obj->vertices[vcont].x,
					&obj->vertices[vcont].y,
					&obj->vertices[vcont].z);
			if(!vcont)
			{
				minx = maxx = obj->vertices[vcont].x;
				miny = maxy = obj->vertices[vcont].y;
				minz = maxz = obj->vertices[vcont].z;
			}
			else
			{
				if(obj->vertices[vcont].x < minx) minx = obj->vertices[vcont].x;
				if(obj->vertices[vcont].y < miny) miny = obj->vertices[vcont].y;
				if(obj->vertices[vcont].z < minz) minz = obj->vertices[vcont].z;
				if(obj->vertices[vcont].x > maxx) maxx = obj->vertices[vcont].x;
				if(obj->vertices[vcont].y > maxy) maxy = obj->vertices[vcont].y;
				if(obj->vertices[vcont].z > maxz) maxz = obj->vertices[vcont].z;
			}
			vcont++;
		}
		if(!strncmp(aux,"vn ",3))
		{
			sscanf(aux,"vn %f %f %f",&obj->normais[ncont].x,
					&obj->normais[ncont].y,
					&obj->normais[ncont].z);
			ncont++;

			obj->normais_por_vertice = true;
		}

		if(!strncmp(aux,"vt ",3))
		{
			sscanf(aux,"vt %f %f %f",&obj->texcoords[tcont].s,
					&obj->texcoords[tcont].t,
					&obj->texcoords[tcont].r);
			tcont++;
		}
		if(!strncmp(aux,"f ",2))
		{
			char *ptr = aux+2;

			obj->faces[fcont].mat = material;

			obj->faces[fcont].texid = texid;
			int vi[10],ti[10],ni[10];
			char sep;
			obj->faces[fcont].nv = 0;
			int nv = 0;
			bool tem_t = false;
			bool tem_n = false;
			while(*ptr)
			{
				vi[nv] = leNumNoTex(&ptr,&sep);
				if(sep==' ')
				{
					nv++;
					continue;
				}
				int aux = leNumNoTex(&ptr,&sep);
				if(aux!=-1)
				{
					ti[nv] = aux;
					tem_t = true;
				}
				if(sep==' ')
				{
					nv++;
					continue;
				}
				aux = leNumNoTex(&ptr,&sep);
				if(aux!=-1)
				{
					ni[nv] = aux;
					tem_n = true;
				}
				nv++;
			}

			obj->faces[fcont].nv = nv;
			obj->faces[fcont].vert = (int *) malloc(sizeof(int)*nv);
			if(tem_n) obj->faces[fcont].norm = (int *) malloc(sizeof(int)*nv);
				else obj->faces[fcont].norm = NULL;
			if(tem_t) obj->faces[fcont].tex  = (int *) malloc(sizeof(int)*nv);
				else obj->faces[fcont].tex = NULL;
			for(i=0;i<nv;++i)
			{

				obj->faces[fcont].vert[i] = vi[i]-1;
				if(tem_n) obj->faces[fcont].norm[i] = ni[i]-1;
				if(tem_t) obj->faces[fcont].tex[i]  = ti[i]-1;
			}
			fcont++;
		}
	}
#ifdef DEBUG
#endif
	fclose(fp);
	_objetosNoTex.push_back(obj);
	return obj;
}

void SetaModoDesenhoNoTex(char modo)
{
	if(modo!='w' && modo!='s' && modo!='t') return;
	_modoNoTex = modo;
}

void DesenhaObjetoNoTex(OBJnotex *obj)
{	
	
	int i;	
	GLint ult_texid, texid;	
	GLenum prim = GL_POLYGON;	

	GLfloat branco[4] = { 1.0, 1.0, 1.0, 1.0 };	
	
	
	if(obj->dlist >= 1000){
		
		glNewList(obj->dlist-1000,GL_COMPILE_AND_EXECUTE);
	}	
	else if(obj->dlist > -1)
	{
		glCallList(obj->dlist);
		return;
	}

	
	
	if(_modoNoTex=='w') prim = GL_LINE_LOOP;

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_TEXTURE_2D);

	if(obj->tem_materiais)
		glDisable(GL_COLOR_MATERIAL);

	
	ult_texid = -1;
	
	for(i=0; i<obj->numFaces; i++)
	{
		
		if(!obj->normais_por_vertice)
			glNormal3f(obj->normais[i].x,obj->normais[i].y,obj->normais[i].z);

		
		if(obj->faces[i].mat != -1)
		{
			
			int mat = obj->faces[i].mat;
			glMaterialfv(GL_FRONT,GL_AMBIENT,_materiaisNoTex[mat]->ka);
			
			
			if(obj->faces[i].texid != -1 && _modoNoTex=='t')
				glMaterialfv(GL_FRONT,GL_DIFFUSE,branco);
			else
				glMaterialfv(GL_FRONT,GL_DIFFUSE,_materiaisNoTex[mat]->kd);
			glMaterialfv(GL_FRONT,GL_SPECULAR,_materiaisNoTex[mat]->ks);
			glMaterialfv(GL_FRONT,GL_EMISSION,_materiaisNoTex[mat]->ke);
			glMaterialf(GL_FRONT,GL_SHININESS,_materiaisNoTex[mat]->spec);
		}

		
		if(obj->textura != -1)
			texid = obj->textura;
		else
			
			texid = obj->faces[i].texid;

		
		if(texid == -1 && ult_texid != -1)
			glDisable(GL_TEXTURE_2D);

		if (texid != -1 && texid != ult_texid && _modoNoTex=='t')
		{
		       glEnable(GL_TEXTURE_2D);
		       glBindTexture(GL_TEXTURE_2D,texid);
		}

		glBegin(prim);

		for(int vf=0; vf<obj->faces[i].nv;++vf)
		{

			if(obj->normais_por_vertice)
				glNormal3f(obj->normais[obj->faces[i].norm[vf]].x,
				obj->normais[obj->faces[i].norm[vf]].y,
				obj->normais[obj->faces[i].norm[vf]].z);

			if(texid!=-1)
				glTexCoord2f(obj->texcoords[obj->faces[i].tex[vf]].s,
				obj->texcoords[obj->faces[i].tex[vf]].t);

			glVertex3f(obj->vertices[obj->faces[i].vert[vf]].x,
		    	obj->vertices[obj->faces[i].vert[vf]].y,
			obj->vertices[obj->faces[i].vert[vf]].z);
		}

		glEnd();


		ult_texid = texid;
	} 
	
	glDisable(GL_TEXTURE_2D);

	glPopAttrib();

	if(obj->dlist >= 1000)
	{
		glEndList();

		obj->dlist-=1000;
	}
}

void _liberaObjetoNoTex(OBJnotex *obj)
{

	if (obj->vertices != NULL)  free(obj->vertices);
	if (obj->normais != NULL)   free(obj->normais);
	if (obj->texcoords != NULL) free(obj->texcoords);

	for(int i=0; i<obj->numFaces;++i)
	{
		if (obj->faces[i].vert != NULL) free(obj->faces[i].vert);
		if (obj->faces[i].norm != NULL) free(obj->faces[i].norm);
		if (obj->faces[i].tex  != NULL) free(obj->faces[i].tex);
	}

	if (obj->faces != NULL) free(obj->faces);

	free(obj);
}


void LiberaObjetoNoTex(OBJnotex *obj)
{
	unsigned int o;
	if(obj==NULL)	
	{
		for(o=0;o<_objetosNoTex.size();++o)
			_liberaObjetoNoTex(_objetosNoTex[o]);
		_objetosNoTex.clear();
	}
	else
	{

		vector<OBJnotex*>::iterator it = _objetosNoTex.begin();
		for(it = _objetosNoTex.begin(); it<_objetosNoTex.end(); ++it)

			if(*it == obj)
				break;

		_objetosNoTex.erase(it);

		_liberaObjetoNoTex(obj);
	}
}


void LiberaMateriaisNoTex()
{
	unsigned int i;
#ifdef DEBUG

#endif

	for(i=0;i<_materiaisNoTex.size();++i)
	{
#ifdef DEBUG

#endif

		free(_materiaisNoTex[i]);
	}

	_materiaisNoTex.clear();
#ifdef DEBUG

#endif

	for(i=0;i<_texturasNoTex.size();++i)
	{

#ifdef DEBUG

#endif
		free(_texturasNoTex[i]);
	}

	_texturasNoTex.clear();
}


void CalculaNormaisPorFaceNoTex(OBJnotex *obj)
{
	int i;

	if(obj->normais_por_vertice) return;

	if ( ( obj->normais = (VERTnotex *) malloc((sizeof(VERTnotex)) * obj->numFaces) ) == NULL )
			return;

	for(i=0; i<obj->numFaces; i++)
	VetorNormalNoTex(obj->vertices[obj->faces[i].vert[0]],
		obj->vertices[obj->faces[i].vert[1]],
		obj->vertices[obj->faces[i].vert[2]],obj->normais[i]);
}

GLenum face[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

char *nomesNoTex[] = {
		"posx", "negx", "posy", "negy", "posz", "negz" };

void SetaFiltroTexturaNoTex(GLint tex, GLint filtromin, GLint filtromag)
{
	glEnable(GL_TEXTURE_2D);
	if(tex!=-1)
	{
		glBindTexture(GL_TEXTURE_2D,tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtromin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtromag);
	}
	else
	for(unsigned int i=0;i<_texturasNoTex.size();++i)
	{
		glBindTexture(GL_TEXTURE_2D,_texturasNoTex[i]->texid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtromin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtromag);
	}
	glDisable(GL_TEXTURE_2D);
}


void DesabilitaDisplayListNoTex(OBJnotex *ptr)
{
	if(ptr == NULL) return;
	if(ptr->dlist > 0 && ptr->dlist < 1000) 
	{
			glDeleteLists(ptr->dlist,1);
	}

	ptr->dlist = -2;
}


void _criaDListNoTex(OBJnotex *ptr)
{

	if(ptr->dlist == -1) ptr->dlist = glGenLists(1);

	ptr->dlist = ptr->dlist + 1000;
}


void CriaDisplayListNoTex(OBJnotex *ptr)
{
	if(ptr==NULL)
	{
		for(unsigned int i=0;i<_objetosNoTex.size();++i)
		{
			ptr = _objetosNoTex[i];

			if(ptr->dlist == -2) continue;
			_criaDListNoTex(ptr);
		}
	}
	else if(ptr->dlist != -2)_criaDListNoTex(ptr);
}
