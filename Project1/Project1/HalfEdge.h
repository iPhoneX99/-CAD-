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
	Edge *edges; // solid中的首个edge
	Face *faces; // solid中的首个face
	Solid *next;
	Solid *pre;

	int vnum;//vertex数目
	int fnum;//face数目
	int lnum;//loop数目

	//构造函数
	Solid() : id(0), faces(NULL), edges(NULL), next(NULL), pre(NULL), fnum(0), vnum(0), lnum(0){}
};

struct Face
{
	int id;
	Solid *solid; // Face所在的solid
	Loop *out_lp; // 外环
	Loop *inner_lp;//内环
	Face *next;
	Face *pre;
	int innum;//内环数目

	//构造函数
	Face() : id(0), solid(NULL), out_lp(NULL), next(NULL), pre(NULL), inner_lp(NULL), innum(0){}
};

struct Loop
{
	int id;
	HalfEdge *halfedges; // 组成环的半边表的首条半边
	Face *face; // 组成环的边表
	Loop *next;
	Loop *pre;

	//构造函数
	Loop() : id(0), halfedges(NULL), face(NULL), next(NULL), pre(NULL){}
};

struct Edge
{
	Vertex* sv; //起点
	Vertex* ev; //终点
	HalfEdge *half_l; //左侧半边
	HalfEdge *half_r; //右侧半边
	Edge *next;
	Edge *pre;

	//构造函数
	Edge() : half_l(NULL), half_r(NULL), next(NULL), pre(NULL){}
};

struct HalfEdge
{
	Edge *edge; //从属的边
	Vertex *sv; //起点
	Vertex *ev; //终点
	Loop *lp; //从属的环
	HalfEdge *next;
	HalfEdge *pre;
	HalfEdge *brother;//对面的半边

	//构造函数
	HalfEdge() : edge(NULL), sv(NULL), lp(NULL), next(NULL), pre(NULL), brother(NULL){}
};

struct Vertex
{
	int id;
	double coordinate[3]; //点的几何坐标
	Vertex *next;
	Vertex *pre;

	//构造函数
	Vertex(double x, double y, double z) : id(0), next(NULL), pre(NULL)
	{
		coordinate[0] = x;
		coordinate[1] = y;
		coordinate[2] = z;
	}
};

#endif
