#include "Elar.h"
#include <bits/stdc++.h>

Solid *ElarOperator::mvfs(double point[3], Vertex *&vertex)//设置一个点（包含几何坐标）、点属于的外环（暂无几何信息）、点属于的面（暂无几何信息）、面属于的体（暂仅有一个点）
{
	Solid *solid = new Solid();
	Face *face = new Face();
	Loop *out_lp = new Loop();

	//根据输入的坐标建一个新点 编号为上一个点+1，将该点存入点表
	vertex = new Vertex(point[0], point[1], point[2]);
	vertex->id = solid->vnum; 
	v_list.push_back(vertex);
	solid->vnum += 1;

	out_lp->id = solid->lnum;
	face->id = solid->fnum;

	//添加一个外环
	l_list.push_back(out_lp);

	//printf("%lf %lf %lf\n", vertex->coordinate[0], vertex->coordinate[1], vertex->coordinate[2]);
	
	solid->fnum += 1;
	solid->lnum += 1;

	solid->faces = face;
	face->solid = solid;

	face->out_lp = out_lp;
	out_lp->face = face;

	return solid;
}


HalfEdge *ElarOperator::mev(Vertex *sv, double point[3], Loop *loop)//输入一个起始点、设置一个新点连一条边（存在两条半边），该边从属于起始点属于的环
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge();
	HalfEdge *half_l = new HalfEdge();
	HalfEdge *half_r = new HalfEdge();

	//根据输入的坐标建一个新点 编号为上一个点+1，将该点存入点表，solid的节点总数++
	Vertex *ev = new Vertex(point[0], point[1], point[2]);
	ev->id = solid->vnum;	
	v_list.push_back(ev);
	solid->vnum += 1;

	//设置半边的两个端点、边和半边从属关系
	half_l->sv = sv; half_l->ev = ev;
	half_r->sv = ev; half_r->ev = sv;

	edge->half_l = half_l; edge->half_r = half_r;
	half_l->edge = edge; half_r->edge = edge;

	half_r->brother = half_l;
	half_l->brother = half_r;

	half_l->next = half_r;
	half_r->pre = half_l;

	//半边连接到环
	half_l->lp = loop;
	half_r->lp = loop;


	//添加两条半边到环
	if (loop->halfedges == NULL)//如果环为空
	{
		half_r->next = half_l;
		half_l->pre = half_r;
		loop->halfedges = half_l;
	}
	else
	{
		//建立一个临时半边，存位置。从链表头开始查找，直到找到起点，加入新的半边
		HalfEdge *tmphalf = loop->halfedges;
		while (tmphalf->ev != sv)
			tmphalf = tmphalf->next;
		half_r->next = tmphalf->next;
		tmphalf->next->pre = half_r;
		tmphalf->next = half_l;
		half_l->pre = tmphalf;
	}

	//添加edge到体
	addEdgeIntoSolid(edge, solid);
	return half_l;
}

Loop *ElarOperator::mef(Vertex *sv, Vertex *ev, Loop *loop, bool mark)//建立一条边，使得一个物理环首尾相连。建立一个新的面（已有一个），把原本的一个loop拆分成两个。
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge();
	HalfEdge *half_l = new HalfEdge();
	HalfEdge *half_r = new HalfEdge();
	Loop *newLoop = new Loop();

	//设置半边的起点终点、互相和边的关系
	half_l->sv = sv;
	half_l->ev = ev;
	half_r->sv = ev;
	half_r->ev = sv;

	half_r->brother = half_l;
	half_l->brother = half_r;

	half_l->edge = edge;
	half_r->edge = edge;
	edge->half_l = half_l;
	edge->half_r = half_r;

	//添加新的半边到环中

	HalfEdge *tmphalf = loop->halfedges;
	HalfEdge *tmpa, *tmpb, *tmpc;
	//在半边链表中，找到连接新边的一个点vs中指向新边的半边
	while (tmphalf->ev != sv)
		tmphalf = tmphalf->next;
	tmpa = tmphalf;
	//在半边链表中，找到连接新边的另一个点ve中指向新边的半边
	while (tmphalf->ev != ev)
		tmphalf = tmphalf->next;
	tmpb = tmphalf;

	//cout << tmpa->sv->id << " " << tmpa->ev->id << "\n";
	//cout << tmpb->sv->id << " " << tmpb->ev->id << "\n";

	//用来判断sweeping过程
	tmphalf = tmphalf->next;
	while (tmphalf->ev != ev)
		tmphalf = tmphalf->next;
	tmpc = tmphalf;

	//把拓扑点vs和新建的两条半边建立关系
	half_r->next = tmpa->next;
	tmpa->next->pre = half_r;
	tmpa->next = half_l;
	half_l->pre = tmpa;

	//把拓扑点ve和新建的两条半边建立关系
	half_l->next = tmpb->next;
	tmpb->next->pre = half_l;
	tmpb->next = half_r;
	half_r->pre = tmpb;

	//把一个大环分成两个小环，把两条新半边作为链表的首半边
	loop->halfedges = half_l;
	newLoop->halfedges = half_r;
	half_l->lp = loop;
	half_r->lp = newLoop;
	//把新环添加到solid下
	newLoop->id = solid->lnum;
	solid->lnum += 1;
	l_list.push_back(newLoop);

	//建一个新face，加入到solid的face表中
	Face *face = new Face();
	addFaceIntoSolid(face, solid);
	
	//把新环加入到新face中
	addLoopIntoFace(newLoop, face);

	if (tmpc == tmpb)
		if (mark)//边在底下的时候
			sweep_list.push_back(half_l->lp->face);
	//else
	//	sweep_list.push_back(half_r->lp->face);

	addEdgeIntoSolid(edge, solid);

	return loop;
}

Loop *ElarOperator::kemr(Vertex *sv, Vertex *ev, Loop *loop)//删除同属于一个外环的边，生成一个环
{

	HalfEdge *tmpa, *tmpb, *hal;
	Face *face = loop->face;
	Loop *inlp = new Loop();
	Solid *solid = loop->face->solid;

	//找到需要去掉的两条半边
	hal = loop->halfedges;
	while (hal->sv != sv || hal->ev != ev)
		hal = hal->next;
	tmpa = hal;

	while (hal->sv != ev || hal->ev != sv)
		hal = hal->next;
	tmpb = hal;

	//删边
	tmpb->pre->next = tmpa->next;
	tmpa->pre->next = tmpb->next;

	loop->face->solid->faces->out_lp->halfedges = tmpa->pre;

	//把内外环分开
	inlp->halfedges = tmpb->pre;
	tmpb->pre->lp = inlp; 

	inlp->id = solid->lnum;
	solid->lnum += 1;
	l_list.push_back(inlp);

	addLoopIntoFace(inlp, tmpa->pre->brother->lp->face);

	delete tmpa;
	delete tmpb;

	return NULL;
}

void ElarOperator::kfmrh(Face *fa, Face *fb)// 删除一个面b，建立一个柄
{
	Loop *loop = fb->out_lp;
	addLoopIntoFace(loop, fa);
	//fa->next = NULL;
	fa->solid->lnum -= 1;
	fa->solid->fnum -= 1;

	Solid *solid = fa->solid;
	Face *face = solid->faces;
	if (face == fb)
	{
		solid->faces = face->next;
	}
	else
	{
		Face *tf = face;
		while (face != fb && face != NULL)
		{
			tf = face;
			face = face->next;
		}
		tf->next = face->next;
	}
	delete fb;
}

void ElarOperator::sweep(double dir[3], double d)
{
	Vertex *startv, *nextv, *upv, *upprev;
	HalfEdge *he, *suphe, *uphe;
	double point[3];

	vector<Face *>::iterator ite;
	for (ite = sweep_list.begin(); ite != sweep_list.end(); ++ite)
	{
		//先做外环
		Loop *loop = (*ite)->out_lp;
		he = loop->halfedges;
		startv = he->sv;
		point[0] = startv->coordinate[0] + d*dir[0];
		point[1] = startv->coordinate[1] + d*dir[1];
		point[2] = startv->coordinate[2] + d*dir[2];


		suphe = mev(startv, point, loop);
		upprev = suphe->ev;
		he = he->next;
		nextv = he->sv;

		Loop *lp = loop;

		while (nextv != startv)
		{
			point[0] = nextv->coordinate[0] + d*dir[0];
			point[1] = nextv->coordinate[1] + d*dir[1];
			point[2] = nextv->coordinate[2] + d*dir[2];
			uphe = mev(nextv, point, lp);
			upv = uphe->ev;

			lp = mef(upprev, upv, loop, false);

			upprev = upv;
			he = he->next;
			nextv = he->sv;
		}
		mef(upprev, suphe->ev, loop, false);

		//内环也要处理
		loop = (*ite)->inner_lp;
		he = loop->halfedges;
		startv = he->sv;
		point[0] = startv->coordinate[0] + d * dir[0];
		point[1] = startv->coordinate[1] + d * dir[1];
		point[2] = startv->coordinate[2] + d * dir[2];


		suphe = mev(startv, point, loop);
		upprev = suphe->ev;
		he = he->next;
		nextv = he->sv;

		lp = loop;
		while (nextv != startv)
		{
			point[0] = nextv->coordinate[0] + d * dir[0];
			point[1] = nextv->coordinate[1] + d * dir[1];
			point[2] = nextv->coordinate[2] + d * dir[2];
			uphe = mev(nextv, point, lp);
			upv = uphe->ev;

			lp = mef(upprev, upv, loop, false);

			upprev = upv;
			he = he->next;
			nextv = he->sv;
		}
		mef(upprev, suphe->ev, loop, false);

	}
}

inline
void ElarOperator::addEdgeIntoSolid(Edge *edge, Solid *&solid)
{
	Edge *tmpedge = solid->edges;

	if (tmpedge == NULL)
		solid->edges = edge;
	else
	{
		while (tmpedge->next != NULL)
			tmpedge = tmpedge->next;
		tmpedge->next = edge;
		edge->pre = tmpedge;
	}
}

inline
void ElarOperator::addFaceIntoSolid(Face *face, Solid *&solid)
{
	Face *tface = solid->faces;
	if (tface == NULL)
	{
		solid->faces = face;
	}
	else
	{
		while (tface->next != NULL)tface = tface->next;
		tface->next = face;
		face->pre = tface;
	}
	face->solid = solid;

	face->id = solid->fnum;

	solid->fnum += 1;
}

inline
void ElarOperator::addLoopIntoFace(Loop *loop, Face *face)
{
	loop->face = face;

	//外环只有一个，但内环可以有很多
	if (face->out_lp == NULL)
	{
		face->out_lp = loop;
	}
	else
	{
		Loop *tlp = face->inner_lp;
		if (tlp == NULL)face->inner_lp = loop;
		else
		{
			while (tlp->next != NULL)tlp = tlp->next;
			tlp->next = loop;
			loop->pre = tlp;
		}
		face->innum += 1;
	}
}
