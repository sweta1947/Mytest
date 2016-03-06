#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "std_msgs/String.h"
#include <sstream>

using namespace cv;
using namespace std;
 Mat image = imread("ps1.jpg", CV_LOAD_IMAGE_COLOR);
struct Node
  {
     int x;
     int y;
     struct Node *next;
  };
Node *push(Node* head,int x,int y)
{
  if(head != NULL)
  {
      Node* temp = new Node;
      temp->x =x;
      temp->y = y;
      temp->next = head;
      return temp;
  }
    else
        {
        head=new Node;
        head->x = x;
        head->y = y;
        return head;
    }
}
Node *pop(Node *head)
{
if(head==NULL)
return head;
return head->next;
}
void change(int x,int y)  //fuction for changing path point into blue and displaying them
{

image.at<Vec3b>(x,y).val[0]=255;
image.at<Vec3b>(x,y).val[1]=0;
image.at<Vec3b>(x,y).val[2]=0;  
}
int exist(int x,int y)
{
if(x<200&&x>=0&&y<200&&y>=0)
return 1;
else
return 0;
}
Node *search(Node *path,int xi,int yi,int xf,int yf)
{
int i,j,x,y,m,n;
int flag[200][200];  //to store if point is visited
for(i=0;i<200;i++)
{
for(j=0;j<200;j++)
{
flag[i][j] = 0;
}
}
printf("1\n");
int count;
Node *stack = new Node;
stack = NULL;
Node *parent = new Node;
stack = push(stack,xi,yi);
parent = push(parent,xi,yi);
flag[xi][yi]=1;
while(stack != NULL)
{
printf("%d  %d\n",stack->x,stack->y);
x = stack->x;
y= stack->y;
count = 0;
for(m=(x-1);m<=(x+1);m++)
{
for(n=(y+1);n>=(y-1);n--)
{
if(exist(m,n)==1)
{
if(flag[m][n]==0&&!(image.at<Vec3b>(m,n).val[0]==255&&image.at<Vec3b>(m,n).val[1]==255&&image.at<Vec3b>(m,n).val[2]==255))
{
count++;
if(count==1)
{
stack=pop(stack);
parent = pop(parent);
}
flag[m][n]=1;
stack = push(stack,m,n);
parent = push(parent,x,y);
path = push(path,x,y);
if(m==xf&&n==yf)
break;
}
}
}
if(m==xf&&n==yf)
break;
}
if(m==xf&&n==yf)
break;
if(count==0)
{
parent = pop(parent);
stack=pop(stack);
path=pop(path);
while(!(parent->x == path->x && parent->y == path->y))
path=pop(path);
}
}
path = push(path,xf,yf);
return path;
}
int main(int argc, char **argv)
{
int i,j,count=0,avgxi=0,avgyi=0,avgxf=0,avgyf=0;
Node *path = new Node;
path = NULL;
for(i=0;i<image.rows;i++)
{
for(j=0;j<image.cols;j++)
{
if(image.at<Vec3b>(i,j).val[2]==237)
{
count++;
avgxi=avgxi+i;
avgyi=avgyi+j;
}
}
}
avgxi=avgxi/count;  //x coordinate of red circle
avgyi=avgyi/count;

count=0;
for(i=0;i<image.rows;i++)
{
for(j=0;j<image.cols;j++)
{
if(image.at<Vec3b>(i,j).val[1]==255&&image.at<Vec3b>(i,j).val[2]==0&&image.at<Vec3b>(i,j).val[0]==0)
{
count++;
avgxf=avgxf+i;
avgyf=avgyf+j;
}
}
}
avgxf=avgxf/count;  //x coordinate of green circle
avgyf=avgyf/count;
path = search(path,avgxi,avgyi,avgxf,avgyf);
while(path->x<200 && path->y<200)
{
i=path->x;
j=path->y;
change(i,j);
path= path->next;

 } 
namedWindow("Display Image", WINDOW_AUTOSIZE ); 
imshow( "Display Image", image );   
waitKey(0);
return 0;
}


