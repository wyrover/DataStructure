#include<stdio.h>
#include<stdlib.h>
#include <limits.h> 

#define INFINITY INT_MAX
#define MAX_VERTEX_NUM 20
#define OK 1
#define ERROR -1
#define TRUE 1
#define FALSE 0

typedef enum{DG , DN, UDG, UDN} GraphKind;//{有向图，有向网，无向图，无向网}
typedef int VRType;
typedef char InfoType;
typedef int VertexType;//向量类型
typedef int Status;

typedef struct ArcCell{

	VRType adj;          //无权图表示0和1，有权是权值
	InfoType *info;      //该弧的相关信息指针
	//int visit;

}ArcCell,AdjMaxtrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];


typedef struct {//邻接图结构

	VertexType   vexs[MAX_VERTEX_NUM];     //顶点向量
	AdjMaxtrix   arcs;                     //邻接矩阵
	int          vexnum;                   //当前顶点数
	int    		 arcnum;                   //弧度数
	GraphKind    kind;                     //图的种类标志	

}MGraph;


Status LocateVex(MGraph G,VertexType v);


Status CreateUDN(MGraph &G)//无向网
{
//	int IncInfo;
	int i,j,k;
	int v1,v2;
	int w;//权重
	printf("请输入当前顶点数和弧度数,以及弧的相关信息:");

	scanf("%d%d",&G.vexnum,&G.arcnum);//如果IncInfo，表示弧没有信息

	printf("请依次输入顶点:\n");
	for(i=0;i<G.vexnum;i++)
		scanf("%d",&G.vexs[i]); //构造顶点向量矩阵

	for(i=0;i<G.vexnum;i++)//初始化邻接矩阵
		for(j=0;j<G.vexnum;j++)
		{
			G.arcs[i][j].adj=0;
			if(i==j)
				G.arcs[i][j].adj=0;
			else
				G.arcs[i][j].adj=15;
		}
	
	printf("请输入两个邻接点及其权重\n");
	for(k=0;k<G.arcnum;k++)//根据弧度数输入顶点
	{
		scanf("%d%d%d",&v1,&v2,&w);
		i=LocateVex(G,v1);
		j=LocateVex(G,v2);
		G.arcs[i][j].adj=w;//给v1和v2赋上权重
		G.arcs[j][i].adj=w;
		//if(IncInfo)
		//	scanf("%c",&(*(G.arcs[i][j].info)));
	}	
	return OK;
}//CreateUDN

Status LocateVex(MGraph G,VertexType v)//得到当前顶点矩阵的位置
{
	for(int i=0;i<G.vexnum;i++)
		if(G.vexs[i]==v)
			return i;

	return ERROR;
}


Status GetVex(MGraph G,int v)//返回v的某个顶点
{
	if(v>=G.arcnum||v<0)
		exit(ERROR);
	return G.vexs[v];
}

Status FirstAdjVex(MGraph &G, VertexType v)//返回为的第一个邻接顶点的序列号
{
	int i=LocateVex(G,v);
	for(int k=0;k<G.vexnum;k++)
		if(G.arcs[i][k].adj)
			return G.vexs[k];
	return ERROR;
}

Status NextAdjVex(MGraph G,VertexType v,VertexType w)//返回w的的下一个邻接顶点
{
	int i=LocateVex(G,v);
	int k=LocateVex(G,w);
	for(int j=k;j<G.vexnum;j++)
		if(G.arcs[i][j].adj==1&&j>k)
			return G.vexs[j];

	return ERROR;
}

Status InsertVex(MGraph &G,VertexType v)//在G图中新添结点
{
	int i;
	if(G.vexnum==MAX_VERTEX_NUM)//矩阵存满
		return ERROR;
	else
		G.vexs[G.vexnum]=v;
	G.vexnum++;//顶点数+1

	for( i=0;i<G.vexnum;i++)//将新增的行初始化
		G.arcs[G.vexnum-1][i].adj=0;

	for( i=0;i<G.vexnum;i++)//将新增的列初始化
		G.arcs[i][G.vexnum-1].adj=0;

	return OK;
}


void  DeleteVex(MGraph &G,VertexType v)//删除结点v,针对无向图和无向网
{
	int loc=LocateVex(G,v);
	int i,j;

	for(j=0;j<G.vexnum;j++)//删掉相关的弧
			if(G.arcs[loc][j].adj==1)
				G.arcnum--;

	for(i=0;i<G.vexnum;i++)//将所在的列删除
		for(j=loc;j<G.vexnum-1;j++)
			G.arcs[i][j]=G.arcs[i][j+1];

	for(i=loc;i<G.vexnum-1;i++)
		for(j=0;j<G.vexnum-1;j++)
			G.arcs[i][j]=G.arcs[i+1][j];
		
	for(i=loc;i<G.vexnum-1;i++)
		G.vexs[i]=G.vexs[i+1];
	G.vexnum--;//结点数减一
}

void InsertArc(MGraph &G,VertexType v,VertexType w)//在G图中添加<v,w>序列
{
	int i,j;//获取w和v在矩阵当中的位置
	i=LocateVex(G,v);
	j=LocateVex(G,w);
	G.arcs[i][j].adj=1;
	G.arcs[j][i].adj=1;//对称弧
}

void DeleteArc(MGraph &G,VertexType v,VertexType w)
{
	//删除弧<v,w>,若G是无向的则还需删除对称弧<w,v>
	int i,j;
	G.vexnum-=2;
	G.arcnum-=1;
	i=LocateVex(G,v);
	j=LocateVex(G,w);
	G.arcs[i][j].adj=0; 	
	G.arcs[j][i].adj=0;
	
}

int Visited[MAX_VERTEX_NUM];//标志数组

Status Visit(VertexType v)
{
	printf("%d ",v);
	return OK;
}

void DFS(MGraph G,VertexType v)
{
	VertexType w;
	int i=LocateVex(G,v);
	if(!Visited[i])
	{
		Visited[i]=TRUE;
		Visit(v);
	}
	for(w=FirstAdjVex(G,v);w>=0;w=NextAdjVex(G,v,w))
		if(!Visited[LocateVex(G,w)])
			DFS(G,w);		
}

Status DFSTraverse(MGraph G,Status(*Visit)(int v))
{
	int i;
	for(i=0;i<G.vexnum;i++)//初始化标志数组
		Visited[i]=FALSE;

	printf("深度优先遍历:");	
	for(i=0;i<G.vexnum;i++)
		if(!Visited[i])
			DFS(G,G.vexs[i]);
	
		printf("\n");
	return OK;
}

void PrintVex(MGraph G)
{
	printf("邻接矩阵为:\n");
	for(int i=0;i<G.vexnum;i++)
	{
		for(int j=0;j<G.vexnum;j++)
			printf("%d ",G.arcs[i][j].adj);
		printf("\n");
	}
}

//Prim算法求最小生成树

typedef struct {//作用是存储该节点的邻接点的权值

	VertexType adjvex;
	VRType     lowcost;
}closedge[MAX_VERTEX_NUM];


int mininum(MGraph G,VertexType v,closedge &close)
{ 
	VRType min;//保存最小权值
	int j,k;
	k=LocateVex(G,v);
	min=15;//设置最大权重
	for(j=0;j<G.vexnum;j++)//找到权值最小的值
		if(close[j].lowcost>0&&close[j].lowcost<=min)
			min=close[j].lowcost;

	for(j=0;j<G.vexnum;j++)
		if(min==close[j].lowcost)
		{
			close[j].adjvex=G.vexs[k];
			return j;
		}	
	return ERROR;
}

void MiniSpanTree_Prim(MGraph &G,VertexType v)
{
	int i,j,k,t;
	closedge close;

	k=LocateVex(G,v);//第一个顶点标记为访问
	for(j=0;j<G.vexnum;j++)
		if(j!=k)
			close[j].lowcost=G.arcs[k][j].adj;
	close[k].lowcost=0;

	for(i=1;i<G.vexnum;i++)
	{
		k=mininum(G,G.vexs[k],close);
		printf("(%d---->%d)\n",close[k].adjvex,G.vexs[k]);
		close[k].lowcost=0;
		for(j=k;j<G.vexnum;j++)//找处当前各个点的最小边
			if(G.arcs[k][j].adj<=close[j].lowcost)
				close[j].lowcost=G.arcs[k][j].adj;
	}
}

int main()
{
	VertexType v;
	MGraph G;
	CreateUDN(G);
	PrintVex(G);
	printf("输入想从那一条顶点建立二叉树:");
	scanf("%d",&v);
	MiniSpanTree_Prim(G,v);

	return OK;
}