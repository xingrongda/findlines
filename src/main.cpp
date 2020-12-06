#include  <iostream>
#include  <fstream>
#include  <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h> 

using   namespace  std;
using namespace cv;
//c++语言里面绝对路径要用\\ 向右双斜杠  ？？？？？？？？？
//思路：取首尾点确定直线，找离该直线最远的点即为拐点，继续，直到距离小于阈值
//思路2：把找点封装成函数，找到一个点之后，找两端
int noTurningPointFlag = 0;
vector<Point> savePoints;
//vector<Point> saveTurningPoints;
// childPoint
// lPoint
// rPoint
vector<vector<int>> nodes;                                                      //nodes[0][0]=0;??????????????????????????????????????
vector<int> aNode;
vector<int> turningPointNumber;

//用来输出一个空行
void  OutPutAnEmptyLine()   
{
	cout << "  \n";
}
/*
*brief  输入文件里的数据
*author rongda
*/
void  ReadDataFromFileWBW()    
{
	
	vector<int> readfile;
	ifstream fin("/home/xingrongda/code/cmake/lidertest/data.txt");             
	int  data;
	int  i = 0;
	while (fin >> data)
	{
		readfile.push_back(data);         
		i++;
	}
	for(i=0;i<(readfile.size()/3);i++)
	{
		Point writePoint = {(readfile[3*i]),(readfile[3*i+1])};
		savePoints.push_back(writePoint);
	}
}
void  ReadDataWithErrChecking()
{
	string  filename = "/home/xingrongda/code/cmake/lidertest/data.txt";
	ifstream fin(filename.c_str());
	if (!fin)
	{
		cout << "Error opening " << filename << " for input" << endl;
		exit(-1);
	}
}

/*
*brief  找两个点之间对两点连线最远的点
*param  起点
*param  终点
*author rongda
*/
void GetTurningPoint(int firstPoint, int lastPoint)
{
	vector<Point> inputPoints;
	for(int i=firstPoint; i<lastPoint; i++)
	{
		inputPoints.push_back(savePoints[i]);
	}
	Point point;
	int pointNumber;
	float distance = 0;	
	float k;
	float b;
	int num = inputPoints.size()-1;
	for(int i=0;i<num;i++)
	{
		float test1=inputPoints[0].y-inputPoints[num].y;
		float test2=inputPoints[0].x-inputPoints[num].x;
		k=test1/test2;
		b=(float)inputPoints[0].y-k*inputPoints[0].x;
		float eachDistance=fabs(k*inputPoints[i].x-inputPoints[i].y+b)/sqrt(k*k+1);//点到直线距离公式
		if(eachDistance>distance)
		{
			distance=eachDistance;
			point = {inputPoints[i].x/4,inputPoints[i].y/4};
			pointNumber=i;
		}
	}	
	inputPoints.clear();	
	cout << "y=" <<  k <<"*x+"<< b << endl;
	cout << "distance=" <<  distance << endl;
	if(distance>400)                                              //////////////调阈值                                          //
	{
		turningPointNumber.push_back(pointNumber);
		aNode.push_back(pointNumber);
		aNode.push_back(firstPoint);
		aNode.push_back(lastPoint);
		nodes.push_back(aNode);
		aNode.clear();
		cout << "done" <<  endl;
		return;
	}
	else
	{	
		return;
	}
	
}
int  main()
{
	Mat m = Mat::zeros(1000,1000,CV_8UC1);
	Point point1,point2;
	int i;
	int pointNumber;
    ReadDataFromFileWBW();
	int num = savePoints.size();
	point1={savePoints[0].x/4,savePoints[0].y/4};
	point2={savePoints[num-1].x/4,savePoints[num-1].y/4};
	cout << "point1 " << point1.x << ' ' << point1.y << endl;
	cout << "point2 " << point2.x << ' ' << point2.y << endl;
	for(i=0;i<savePoints.size() ;i++)    
	{
		int rol=savePoints[i].y/4;
		int col=savePoints[i].x/4;
	    m.at<uchar>(rol, col) = 255 ;
	}
	
    //由初始点找到第一个节点
	GetTurningPoint(0,num);
	cout<<"nodes.size()= "<<nodes.size()<<endl;
	//每找到一个节点多循环一次，每一个节点可以再找出两个节点
	for(i=0;i<nodes.size();i++)
	{
		GetTurningPoint(nodes[i][1],nodes[i][0]);
		cout<<"nodes.size()= "<<nodes.size()<<endl;
		GetTurningPoint(nodes[i][0],nodes[i][2]);
	    cout<<"nodes.size()= "<<nodes.size()<<endl;
	}
	
	for(i=0;i<(turningPointNumber.size());i++)
	{
		savePoints[turningPointNumber[i]].x=savePoints[turningPointNumber[i]].x/4;
		savePoints[turningPointNumber[i]].y=savePoints[turningPointNumber[i]].y/4;
	}
	line(m,point2,savePoints[turningPointNumber[0]],255);  
	cout<<"turningPoint"<<savePoints[turningPointNumber[0]].x<<' '<<savePoints[turningPointNumber[0]].y<<endl;  
	if(turningPointNumber.size()>1)  
	{
		for(i=0;i<(turningPointNumber.size()-1);i++)
		{
			line(m,savePoints[turningPointNumber[i]],savePoints[turningPointNumber[i+1]],255);
			cout<<"turningPoint"<<savePoints[turningPointNumber[i]].x<<' '<<savePoints[turningPointNumber[i]].y<<endl;
			cout<<"turningPoint"<<savePoints[turningPointNumber[i+1]].x<<' '<<savePoints[turningPointNumber[i+1]].y<<endl;
		}
		line(m,savePoints[turningPointNumber[i]],point1,255);
		cout<<"turningPoint"<<savePoints[turningPointNumber[i]].x<<' '<<savePoints[turningPointNumber[i]].y<<endl;
	}
	else
	{		
		line(m,savePoints[turningPointNumber[0]],point1,255);
	}
	
	for(i=0;i<(turningPointNumber.size());i++)
	{		
		// cout<<"turningPoint"<<savePoints[turningPointNumber[i]].x<<' '<<savePoints[turningPointNumber[i]].y<<endl;
		cout<<"turningPointNumber= "<<turningPointNumber[i]<<endl;
	}
	cout<<"turningPointNumber.size= "<<turningPointNumber.size()<<endl;
	imshow("lines",m);
    OutPutAnEmptyLine();  
	ReadDataWithErrChecking();  
	system("pause");
	waitKey(0);
	return   0;
}
