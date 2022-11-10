#include "Elar.h"
#include <bits/stdc++.h>

Solid *ElarOperator::mvfs(double point[3], Vertex *&vertex)//����һ���㣨�����������꣩�������ڵ��⻷�����޼�����Ϣ���������ڵ��棨���޼�����Ϣ���������ڵ��壨�ݽ���һ���㣩
{
	Solid *solid = new Solid();
	Face *face = new Face();
	Loop *out_lp = new Loop();

	//������������꽨һ���µ� ���Ϊ��һ����+1�����õ������
	vertex = new Vertex(point[0], point[1], point[2]);
	vertex->id = solid->vnum; 
	v_list.push_back(vertex);
	solid->vnum += 1;

	out_lp->id = solid->lnum;
	face->id = solid->fnum;

	//���һ���⻷
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


HalfEdge *ElarOperator::mev(Vertex *sv, double point[3], Loop *loop)//����һ����ʼ�㡢����һ���µ���һ���ߣ�����������ߣ����ñߴ�������ʼ�����ڵĻ�
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge();
	HalfEdge *half_l = new HalfEdge();
	HalfEdge *half_r = new HalfEdge();

	//������������꽨һ���µ� ���Ϊ��һ����+1�����õ������solid�Ľڵ�����++
	Vertex *ev = new Vertex(point[0], point[1], point[2]);
	ev->id = solid->vnum;	
	v_list.push_back(ev);
	solid->vnum += 1;

	//���ð�ߵ������˵㡢�ߺͰ�ߴ�����ϵ
	half_l->sv = sv; half_l->ev = ev;
	half_r->sv = ev; half_r->ev = sv;

	edge->half_l = half_l; edge->half_r = half_r;
	half_l->edge = edge; half_r->edge = edge;

	half_r->brother = half_l;
	half_l->brother = half_r;

	half_l->next = half_r;
	half_r->pre = half_l;

	//������ӵ���
	half_l->lp = loop;
	half_r->lp = loop;


	//���������ߵ���
	if (loop->halfedges == NULL)//�����Ϊ��
	{
		half_r->next = half_l;
		half_l->pre = half_r;
		loop->halfedges = half_l;
	}
	else
	{
		//����һ����ʱ��ߣ���λ�á�������ͷ��ʼ���ң�ֱ���ҵ���㣬�����µİ��
		HalfEdge *tmphalf = loop->halfedges;
		while (tmphalf->ev != sv)
			tmphalf = tmphalf->next;
		half_r->next = tmphalf->next;
		tmphalf->next->pre = half_r;
		tmphalf->next = half_l;
		half_l->pre = tmphalf;
	}

	//���edge����
	addEdgeIntoSolid(edge, solid);
	return half_l;
}

Loop *ElarOperator::mef(Vertex *sv, Vertex *ev, Loop *loop, bool mark)//����һ���ߣ�ʹ��һ��������β����������һ���µ��棨����һ��������ԭ����һ��loop��ֳ�������
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge();
	HalfEdge *half_l = new HalfEdge();
	HalfEdge *half_r = new HalfEdge();
	Loop *newLoop = new Loop();

	//���ð�ߵ�����յ㡢����ͱߵĹ�ϵ
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

	//����µİ�ߵ�����

	HalfEdge *tmphalf = loop->halfedges;
	HalfEdge *tmpa, *tmpb, *tmpc;
	//�ڰ�������У��ҵ������±ߵ�һ����vs��ָ���±ߵİ��
	while (tmphalf->ev != sv)
		tmphalf = tmphalf->next;
	tmpa = tmphalf;
	//�ڰ�������У��ҵ������±ߵ���һ����ve��ָ���±ߵİ��
	while (tmphalf->ev != ev)
		tmphalf = tmphalf->next;
	tmpb = tmphalf;

	//cout << tmpa->sv->id << " " << tmpa->ev->id << "\n";
	//cout << tmpb->sv->id << " " << tmpb->ev->id << "\n";

	//�����ж�sweeping����
	tmphalf = tmphalf->next;
	while (tmphalf->ev != ev)
		tmphalf = tmphalf->next;
	tmpc = tmphalf;

	//�����˵�vs���½���������߽�����ϵ
	half_r->next = tmpa->next;
	tmpa->next->pre = half_r;
	tmpa->next = half_l;
	half_l->pre = tmpa;

	//�����˵�ve���½���������߽�����ϵ
	half_l->next = tmpb->next;
	tmpb->next->pre = half_l;
	tmpb->next = half_r;
	half_r->pre = tmpb;

	//��һ���󻷷ֳ�����С�����������°����Ϊ������װ��
	loop->halfedges = half_l;
	newLoop->halfedges = half_r;
	half_l->lp = loop;
	half_r->lp = newLoop;
	//���»���ӵ�solid��
	newLoop->id = solid->lnum;
	solid->lnum += 1;
	l_list.push_back(newLoop);

	//��һ����face�����뵽solid��face����
	Face *face = new Face();
	addFaceIntoSolid(face, solid);
	
	//���»����뵽��face��
	addLoopIntoFace(newLoop, face);

	if (tmpc == tmpb)
		if (mark)//���ڵ��µ�ʱ��
			sweep_list.push_back(half_l->lp->face);
	//else
	//	sweep_list.push_back(half_r->lp->face);

	addEdgeIntoSolid(edge, solid);

	return loop;
}

Loop *ElarOperator::kemr(Vertex *sv, Vertex *ev, Loop *loop)//ɾ��ͬ����һ���⻷�ıߣ�����һ����
{

	HalfEdge *tmpa, *tmpb, *hal;
	Face *face = loop->face;
	Loop *inlp = new Loop();
	Solid *solid = loop->face->solid;

	//�ҵ���Ҫȥ�����������
	hal = loop->halfedges;
	while (hal->sv != sv || hal->ev != ev)
		hal = hal->next;
	tmpa = hal;

	while (hal->sv != ev || hal->ev != sv)
		hal = hal->next;
	tmpb = hal;

	//ɾ��
	tmpb->pre->next = tmpa->next;
	tmpa->pre->next = tmpb->next;

	loop->face->solid->faces->out_lp->halfedges = tmpa->pre;

	//�����⻷�ֿ�
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

void ElarOperator::kfmrh(Face *fa, Face *fb)// ɾ��һ����b������һ����
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
		//�����⻷
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

		//�ڻ�ҲҪ����
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

	//�⻷ֻ��һ�������ڻ������кܶ�
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
