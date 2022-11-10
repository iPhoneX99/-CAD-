#include<iostream>
#include<GL/glut.h>
#include "Elar.h"
#include "HalfEdge.h"

using namespace std;

ElarOperator* el;

GLfloat p[12] = { 0,0,0,0,10,0,10,10,0,10,0,0 };

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����

	//glColor3f(0.0, 0.0, 0.0);//���û�ͼ��ɫ��Ĭ��
	glLineWidth(1.0f);//�����߿�Ĭ��1.0f

	for (int i = 0; i < el->getLoop_list().size(); i++)
	{
		//cout << "Loop"<<i<<"\n";
		Loop *lp = el->getLoop_list()[i];
		HalfEdge* hf = lp->halfedges;
		glBegin(GL_LINES);
		glColor3f(1, 0, 1);
		GLdouble p1x = hf->sv->coordinate[0];
		GLdouble p1y = hf->sv->coordinate[1];
		GLdouble p1z = hf->sv->coordinate[2];
		GLdouble p2x = hf->ev->coordinate[0];
		GLdouble p2y = hf->ev->coordinate[1];
		GLdouble p2z = hf->ev->coordinate[2];
		glVertex3f(p1x, p1y, p1z);
		glVertex3f(p2x, p2y, p2z);
		glEnd();
		Vertex* ev = hf->ev;
		hf = hf->next;
		while (hf->ev->id != ev->id)
		{
			/*cout << hf->ev->id;*/
			glBegin(GL_LINES);
			glColor3f(1, 0, 1);
			GLdouble p1x = hf->sv->coordinate[0];
			GLdouble p1y = hf->sv->coordinate[1];
			GLdouble p1z = hf->sv->coordinate[2];
			GLdouble p2x = hf->ev->coordinate[0];
			GLdouble p2y = hf->ev->coordinate[1];
			GLdouble p2z = hf->ev->coordinate[2];
			glVertex3f(p1x, p1y, p1z);
			glVertex3f(p2x, p2y, p2z);
			glEnd();
			hf = hf->next;
		}
	}
	
	//cout << "?";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(30, 1, 0, 0);
	glRotatef(30, 0, 1, 0);
	glRotatef(30, 0, 0, 1);
	//glFlush();//ǿ��ˢ�»��壬��֤��ͼ�����ִ��
	glutSwapBuffers();
}

void init()
{
	//ȫ����ɫ��ɺ�ɫ
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//������ȣ��赲�����Ԫ��
	glEnable(GL_DEPTH_TEST);
	//�ı�ͶӰ��ͼ��
	glMatrixMode(GL_PROJECTION);
	//opengl��һ��״̬����Ҫ�����֮ǰ�ı任�������ݣ�����ÿ���ӽǲ���ʱ��Ҫ�ȱ�Ϊ��λ����
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	//�޸��ӽ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(30, 1, 0, 0);
	glRotatef(30, 0, 1, 0);
	glRotatef(30, 0, 0, 1);
}

ElarOperator* go()
{
	ElarOperator *elar = new ElarOperator();
	HalfEdge* halfedge = new HalfEdge();
	
	//��������⻷�ĵ�
	double point1[3] = { 0,0,0 };
	double point2[3] = { 0,1,0 };
	double point3[3] = { 1,1,0 };
	double point4[3] = { 1,0,0 };
	Vertex* v1 = new Vertex(0,0,0);

	//��������ڻ��ĵ�
	double point5[3] = { 0.2,0.2,0 };
	double point6[3] = { 0.2,0.8,0 };
	double point7[3] = { 0.8,0.8,0 };
	double point8[3] = { 0.8,0.2,0 };

	//�����ɵ����⻷
	Solid *solid = elar->mvfs(point1,v1);
	halfedge = elar->mev(v1, point2, elar->getLoop_list()[0]);
	Vertex *v2 = halfedge->ev;
	halfedge = elar->mev(v2, point3, elar->getLoop_list()[0]);
	Vertex* v3 = halfedge->ev;
	halfedge = elar->mev(v3, point4, elar->getLoop_list()[0]);
	Vertex* v4 = halfedge->ev;
	elar->mef(v4, v1, elar->getLoop_list()[0],1);

	//����ͨ��V1���Ӹ����ߣ������ڻ�
	halfedge = elar->mev(v1, point5, elar->getLoop_list()[0]);
	Vertex* v5 = halfedge->ev;
	halfedge = elar->mev(v5, point6, elar->getLoop_list()[0]);
	Vertex* v6 = halfedge->ev;
	halfedge = elar->mev(v6, point7, elar->getLoop_list()[0]);
	Vertex* v7 = halfedge->ev;
	halfedge = elar->mev(v7, point8, elar->getLoop_list()[0]);
	Vertex* v8 = halfedge->ev;
	elar->mef(v8, v5, elar->getLoop_list()[0], 1);

	//for (int i = 0; i < elar->getLoop_list().size(); i++)
	//{
	//	elar->printLoop(elar->getLoop_list()[i]);
	//	cout << "\n";
	//}

	//ɾ��������
	elar->kemr(v1,v5, elar->getLoop_list()[0]);

	//��������ͨ��
	elar->kfmrh(solid->faces,solid->faces->next->next);

	//for (int i = 0; i < elar->getLoop_list().size(); i++)
	//{
	//	elar->printLoop(elar->getLoop_list()[i]);
	//	cout << "\n";
	//}

	double dir[3] = { 0,0,0.1 };

	elar->sweep(dir, 10);

	//��������ͨ��
	/*elar->kfmrh(solid->faces, solid->faces->next);*/

	Face *tmpface = new Face();
	tmpface = solid->faces;

	//while (tmpface->next != NULL)
	//{
	//	cout <<"ID:" << tmpface->id << "\n";
	//	Loop* lp = new Loop();
	//	lp = tmpface->out_lp;
	//	HalfEdge* hf = new HalfEdge();
	//	hf = lp->halfedges;
	//	Vertex* sv = hf->sv;

	//	while (hf->ev != sv)
	//	{
	//		cout << hf->sv->id << " ";
	//		hf = hf->next;
	//	}
	//	cout << hf->sv->id << " ";
	//	cout << "\n";

	//	if(tmpface->inner_lp != NULL)
	//		cout<<"ID:" << tmpface->id << "�����ڻ�\n";

	//	tmpface = tmpface->next;
	//}

	//for (int i = 0; i < elar->getLoop_list().size(); i++)
	//{
	//	elar->printLoop(elar->getLoop_list()[i]);
	//	cout << "\n";
	//}

	solid;

	return elar;

}


int main(int argc, char** argv) 
{
	el = go();

	//��ʼ��������Ĭ�Ͽ�ͷ
	glutInit(&argc, argv);//��ʼ��GLUT�⣬��������� main ������ȡ����������
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//����ͼ����ʾģʽ
	glutInitWindowPosition(200, 200);//���ô���λ�ã������Ͻ�
	glutInitWindowSize(640, 480);//���ô��ڴ�С
	glutCreateWindow("Cubic");//�������ڣ��ַ���Ϊ��������

	//glutInit(&argc, argv);
	////displayMode������GLUT_DEPTHʹ�����
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	////���ô�����
	//glutCreateWindow("Cubic");


	//ͼ�λ���
	glutDisplayFunc(&display);//����
	init();
	glutMainLoop();//glut�¼�����ѭ��������ͼ�λ��ƣ����̡���������

	return 0;
}
