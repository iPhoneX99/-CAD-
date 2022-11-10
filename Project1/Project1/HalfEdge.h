#ifndef __HALF_EDGE_STRUCTURE__
#define __HALF_EDGE_STRUCTURE__

#include <stdlib.h>
#include <bits/stdc++.h>

struct Solid;
struct Face;
struct Loop;
struct HalfEdge;
struct Vertex;
struct Edge;

struct Solid
{
	int id;  
	Edge *edges; // solid�е��׸�edge
	Face *faces; // solid�е��׸�face
	Solid *next;
	Solid *pre;

	int vnum;//vertex��Ŀ
	int fnum;//face��Ŀ
	int lnum;//loop��Ŀ

	//���캯��
	Solid() : id(0), faces(NULL), edges(NULL), next(NULL), pre(NULL), fnum(0), vnum(0), lnum(0){}
};

struct Face
{
	int id;
	Solid *solid; // Face���ڵ�solid
	Loop *out_lp; // �⻷
	Loop *inner_lp;//�ڻ�
	Face *next;
	Face *pre;
	int innum;//�ڻ���Ŀ

	//���캯��
	Face() : id(0), solid(NULL), out_lp(NULL), next(NULL), pre(NULL), inner_lp(NULL), innum(0){}
};

struct Loop
{
	int id;
	HalfEdge *halfedges; // ��ɻ��İ�߱���������
	Face *face; // ��ɻ��ı߱�
	Loop *next;
	Loop *pre;

	//���캯��
	Loop() : id(0), halfedges(NULL), face(NULL), next(NULL), pre(NULL){}
};

struct Edge
{
	Vertex* sv; //���
	Vertex* ev; //�յ�
	HalfEdge *half_l; //�����
	HalfEdge *half_r; //�Ҳ���
	Edge *next;
	Edge *pre;

	//���캯��
	Edge() : half_l(NULL), half_r(NULL), next(NULL), pre(NULL){}
};

struct HalfEdge
{
	Edge *edge; //�����ı�
	Vertex *sv; //���
	Vertex *ev; //�յ�
	Loop *lp; //�����Ļ�
	HalfEdge *next;
	HalfEdge *pre;
	HalfEdge *brother;//����İ��

	//���캯��
	HalfEdge() : edge(NULL), sv(NULL), lp(NULL), next(NULL), pre(NULL), brother(NULL){}
};

struct Vertex
{
	int id;
	double coordinate[3]; //��ļ�������
	Vertex *next;
	Vertex *pre;

	//���캯��
	Vertex(double x, double y, double z) : id(0), next(NULL), pre(NULL)
	{
		coordinate[0] = x;
		coordinate[1] = y;
		coordinate[2] = z;
	}
};

#endif
