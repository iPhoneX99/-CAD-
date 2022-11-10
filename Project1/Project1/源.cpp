#include<iostream>
#include<GL/glut.h>
#include "Elar.h"
#include "HalfEdge.h"

using namespace std;

ElarOperator* el;

GLfloat p[12] = { 0,0,0,0,10,0,10,10,0,10,0,0 };

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲

	//glColor3f(0.0, 0.0, 0.0);//设置绘图颜色，默认
	glLineWidth(1.0f);//设置线宽，默认1.0f

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
	//glFlush();//强制刷新缓冲，保证绘图命令将被执行
	glutSwapBuffers();
}

void init()
{
	//全屏颜色变成黑色
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//开启深度，阻挡后面的元素
	glEnable(GL_DEPTH_TEST);
	//改变投影视图，
	glMatrixMode(GL_PROJECTION);
	//opengl是一个状态机，要先清空之前的变换矩阵数据，所以每次视角操作时都要先变为单位矩阵
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	//修改视角
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
	
	//定义底面外环的点
	double point1[3] = { 0,0,0 };
	double point2[3] = { 0,1,0 };
	double point3[3] = { 1,1,0 };
	double point4[3] = { 1,0,0 };
	Vertex* v1 = new Vertex(0,0,0);

	//定义底面内环的点
	double point5[3] = { 0.2,0.2,0 };
	double point6[3] = { 0.2,0.8,0 };
	double point7[3] = { 0.8,0.8,0 };
	double point8[3] = { 0.8,0.2,0 };

	//先生成底面外环
	Solid *solid = elar->mvfs(point1,v1);
	halfedge = elar->mev(v1, point2, elar->getLoop_list()[0]);
	Vertex *v2 = halfedge->ev;
	halfedge = elar->mev(v2, point3, elar->getLoop_list()[0]);
	Vertex* v3 = halfedge->ev;
	halfedge = elar->mev(v3, point4, elar->getLoop_list()[0]);
	Vertex* v4 = halfedge->ev;
	elar->mef(v4, v1, elar->getLoop_list()[0],1);

	//生成通过V1连接辅助边，底面内环
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

	//删除辅助边
	elar->kemr(v1,v5, elar->getLoop_list()[0]);

	//生成中心通孔
	elar->kfmrh(solid->faces,solid->faces->next->next);

	//for (int i = 0; i < elar->getLoop_list().size(); i++)
	//{
	//	elar->printLoop(elar->getLoop_list()[i]);
	//	cout << "\n";
	//}

	double dir[3] = { 0,0,0.1 };

	elar->sweep(dir, 10);

	//生成中心通孔
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
	//		cout<<"ID:" << tmpface->id << "存在内环\n";

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

	//初始化操作，默认开头
	glutInit(&argc, argv);//初始化GLUT库，这个函数从 main 函数获取其两个参数
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//设置图形显示模式
	glutInitWindowPosition(200, 200);//设置窗口位置，距左上角
	glutInitWindowSize(640, 480);//设置窗口大小
	glutCreateWindow("Cubic");//创建窗口，字符串为窗口名称

	//glutInit(&argc, argv);
	////displayMode，增加GLUT_DEPTH使得深度
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	////设置窗口名
	//glutCreateWindow("Cubic");


	//图形绘制
	glutDisplayFunc(&display);//绘制
	init();
	glutMainLoop();//glut事件处理循环，包括图形绘制，键盘、鼠标输入等

	return 0;
}
