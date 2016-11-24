#include "MattingDef.h"



using namespace std;

// 暂时没用到
// ----------------------------------------------------------------------------------------
void colorgrow(cv::Mat &face,cv::Mat &mask,cv::Mat &refermask,cv::Mat &referedge,SeedLoc *seedloc,int seednum,cv::Rect faceloc)
{
	int width=face.cols;
	int height=face.rows;
	int i,j;
	int flag;
	int *pointloc=(int*)malloc(width*height*5*sizeof(int));
	memset(pointloc,0,width*height*5*sizeof(int));
	int pointcount=0;
	int b1,r1,g1,alpha1;
	int b0,r0,g0;
	int bs,rs,gs;
	int roundnum=0;
	int left,right,top,bottom;
	int absdifthr,absdifthg,absdifthb;
	int curdifthr,curdifthg,curdifthb;
	int refer;
	int edge;
	cv::Mat hsvmat(height,width,CV_8UC3,CvScalar(0));
	cv::cvtColor(face,hsvmat,CV_BGR2HSV);
	int referh,curh,hth;
	hth=0;

	while(1)
	{	
		top=seedloc[roundnum].y1;
		bottom=seedloc[roundnum].y2;
		left=seedloc[roundnum].x1;
		right=seedloc[roundnum].x2;

		if(seednum-1==roundnum)
			hth=10;

		for(int x=top;x<bottom;x++){
			for(int y=left;y<right;y++){
				i=x;
				j=y;
				edge=referedge.at<uchar>(i,j);
				if(0!=edge)
					continue;
				bs=b0=face.at<cv::Vec3b>(i,j)[0];
				gs=g0=face.at<cv::Vec3b>(i,j)[1];
				rs=r0=face.at<cv::Vec3b>(i,j)[2];
				absdifthb=min(max(bs/3,15),30);
				absdifthg=min(max(gs/3,15),30);
				absdifthr=min(max(rs/3,15),30);
				referh=hsvmat.at<cv::Vec3b>(i,j)[0];
				pointcount=0;
				mask.at<uchar>(i,j)=255;
				while(pointcount>=0){

					//if(x>faceloc.y&&x<faceloc.y+faceloc.height
					//	&&y>faceloc.x&&y<faceloc.x+faceloc.width
					//	&&(i>faceloc.y+faceloc.height||i<faceloc.y
					//	||j<faceloc.x||j>faceloc.x+faceloc.width))
					//if(x<faceloc.y+faceloc.height
					//	&&(i>faceloc.y+faceloc.height||i<faceloc.y
					//	||j<faceloc.x||j>faceloc.x+faceloc.width))
					//{
					//	pointcount--;
					//	if(pointcount<0)
					//		break;
					//	i=*(pointloc+pointcount*5);
					//	j=*(pointloc+pointcount*5+1);
					//	b0=*(pointloc+pointcount*5+2);
					//	g0=*(pointloc+pointcount*5+3);
					//	r0=*(pointloc+pointcount*5+4);
					//	continue;
					//}
					
					curdifthb=min(max(b0/4,15),25);
					curdifthg=min(max(g0/4,15),25);
					curdifthr=min(max(r0/4,15),25);
	/*				if(i-1>=0&&j-1>=0)
					{
						flag=mask.at<uchar>(i-1,j-1);
						refer=refermask.at<uchar>(i-1,j-1);
						edge=referedge.at<uchar>(i-1,j-1);
						b1=face.at<cv::Vec3b>(i-1,j-1)[0];
						g1=face.at<cv::Vec3b>(i-1,j-1)[1];
						r1=face.at<cv::Vec3b>(i-1,j-1)[2];
						if(255!=flag&&0!=refer&&0==edge
							&&abs(b1-b0)<curdifthb
							&&abs(g1-g0)<curdifthg
							&&abs(r1-r0)<curdifthr
							&&abs(b1-bs)<absdifthb
							&&abs(g1-gs)<absdifthg
							&&abs(r1-rs)<absdifthr){
								mask.at<uchar>(i-1,j-1)=255;
								*(pointloc+pointcount*5)=i-1;
								*(pointloc+pointcount*5+1)=j-1;
								*(pointloc+pointcount*5+2)=b1;
								*(pointloc+pointcount*5+3)=g1;
								*(pointloc+pointcount*5+4)=r1;
								pointcount++;
						}
					}*/
					if(i-1>=0)
					{
						flag=mask.at<uchar>(i-1,j);
						refer=refermask.at<uchar>(i-1,j);
						edge=referedge.at<uchar>(i-1,j);
						curh = hsvmat.at<cv::Vec3b>(i-1,j)[0];
						b1=face.at<cv::Vec3b>(i-1,j)[0];
						g1=face.at<cv::Vec3b>(i-1,j)[1];
						r1=face.at<cv::Vec3b>(i-1,j)[2];
						if(255!=flag&&0!=refer&&255!=edge
							&&abs(b1-b0)<curdifthb
							&&abs(g1-g0)<curdifthg
							&&abs(r1-r0)<curdifthr
							&&((abs(b1-bs)<absdifthb
							&&abs(g1-gs)<absdifthg
							&&abs(r1-rs)<absdifthr)
							||(abs(curh-referh)<hth))){
								mask.at<uchar>(i-1,j)=255;
								*(pointloc+pointcount*5)=i-1;
								*(pointloc+pointcount*5+1)=j;
								*(pointloc+pointcount*5+2)=b1;
								*(pointloc+pointcount*5+3)=g1;
								*(pointloc+pointcount*5+4)=r1;
								pointcount++;
						}
					}
					//if(i-1>=0&&j+1<width)
					//{
					//	flag=mask.at<uchar>(i-1,j+1);
					//	refer=refermask.at<uchar>(i-1,j+1);
					//	edge=referedge.at<uchar>(i-1,j+1);
					//	b1=face.at<cv::Vec3b>(i-1,j+1)[0];
					//	g1=face.at<cv::Vec3b>(i-1,j+1)[1];
					//	r1=face.at<cv::Vec3b>(i-1,j+1)[2];
					//	if(255!=flag&&0!=refer&&0==edge
					//		&&abs(b1-b0)<curdifthb
					//		&&abs(g1-g0)<curdifthg
					//		&&abs(r1-r0)<curdifthr
					//		&&abs(b1-bs)<absdifthb
					//		&&abs(g1-gs)<absdifthg
					//		&&abs(r1-rs)<absdifthr){
					//			mask.at<uchar>(i-1,j+1)=255;
					//			*(pointloc+pointcount*5)=i-1;
					//			*(pointloc+pointcount*5+1)=j+1;
					//			*(pointloc+pointcount*5+2)=b1;
					//			*(pointloc+pointcount*5+3)=g1;
					//			*(pointloc+pointcount*5+4)=r1;
					//			pointcount++;
					//	}
					//}
					if(j-1>=0)
					{
						flag=mask.at<uchar>(i,j-1);
						refer=refermask.at<uchar>(i,j-1);
						edge=referedge.at<uchar>(i,j-1);
						curh=hsvmat.at<cv::Vec3b>(i,j-1)[0];
						b1=face.at<cv::Vec3b>(i,j-1)[0];
						g1=face.at<cv::Vec3b>(i,j-1)[1];
						r1=face.at<cv::Vec3b>(i,j-1)[2];
						if(255!=flag&&0!=refer&&255!=edge
							&&abs(b1-b0)<curdifthb
							&&abs(g1-g0)<curdifthg
							&&abs(r1-r0)<curdifthr
							&&((abs(b1-bs)<absdifthb
							&&abs(g1-gs)<absdifthg
							&&abs(r1-rs)<absdifthr)
							||(abs(curh-referh)<hth))){
								mask.at<uchar>(i,j-1)=255;
								*(pointloc+pointcount*5)=i;
								*(pointloc+pointcount*5+1)=j-1;
								*(pointloc+pointcount*5+2)=b1;
								*(pointloc+pointcount*5+3)=g1;
								*(pointloc+pointcount*5+4)=r1;
								pointcount++;
						}
					}
					if(j+1<width)
					{
						flag=mask.at<uchar>(i,j+1);
						refer=refermask.at<uchar>(i,j+1);
						edge=referedge.at<uchar>(i,j+1);
						curh=hsvmat.at<cv::Vec3b>(i,j+1)[0];
						b1=face.at<cv::Vec3b>(i,j+1)[0];
						g1=face.at<cv::Vec3b>(i,j+1)[1];
						r1=face.at<cv::Vec3b>(i,j+1)[2];
						if(255!=flag&&0!=refer&&255!=edge
							&&abs(b1-b0)<curdifthb
							&&abs(g1-g0)<curdifthg
							&&abs(r1-r0)<curdifthr
							&&((abs(b1-bs)<absdifthb
							&&abs(g1-gs)<absdifthg
							&&abs(r1-rs)<absdifthr)
							||(abs(curh-referh)<hth))){
								mask.at<uchar>(i,j+1)=255;
								*(pointloc+pointcount*5)=i;
								*(pointloc+pointcount*5+1)=j+1;
								*(pointloc+pointcount*5+2)=b1;
								*(pointloc+pointcount*5+3)=g1;
								*(pointloc+pointcount*5+4)=r1;
								pointcount++;
						}
					}
					//if(i+1<height&&j-1>=0)
					//{
					//	flag=mask.at<uchar>(i+1,j-1);
					//	refer=refermask.at<uchar>(i+1,j-1);
					//	edge=referedge.at<uchar>(i+1,j-1);
					//	b1=face.at<cv::Vec3b>(i+1,j-1)[0];
					//	g1=face.at<cv::Vec3b>(i+1,j-1)[1];
					//	r1=face.at<cv::Vec3b>(i+1,j-1)[2];
					//	if(255!=flag&&0!=refer&&0==edge
					//		&&abs(b1-b0)<curdifthb
					//		&&abs(g1-g0)<curdifthg
					//		&&abs(r1-r0)<curdifthr
					//		&&abs(b1-bs)<absdifthb
					//		&&abs(g1-gs)<absdifthg
					//		&&abs(r1-rs)<absdifthr){
					//			mask.at<uchar>(i+1,j-1)=255;
					//			*(pointloc+pointcount*5)=i+1;
					//			*(pointloc+pointcount*5+1)=j-1;
					//			*(pointloc+pointcount*5+2)=b1;
					//			*(pointloc+pointcount*5+3)=g1;
					//			*(pointloc+pointcount*5+4)=r1;
					//			pointcount++;
					//	}

					//}
					if(i+1<height)
					{
						flag=mask.at<uchar>(i+1,j);
						refer=refermask.at<uchar>(i+1,j);
						edge=referedge.at<uchar>(i+1,j);
						curh=hsvmat.at<cv::Vec3b>(i+1,j)[0];
						b1=face.at<cv::Vec3b>(i+1,j)[0];
						g1=face.at<cv::Vec3b>(i+1,j)[1];
						r1=face.at<cv::Vec3b>(i+1,j)[2];
						if(255!=flag&&0!=refer&&255!=edge
							&&abs(b1-b0)<curdifthb
							&&abs(g1-g0)<curdifthg
							&&abs(r1-r0)<curdifthr
							&&((abs(b1-bs)<absdifthb
							&&abs(g1-gs)<absdifthg
							&&abs(r1-rs)<absdifthr)
							||(abs(curh-referh)<hth))){
								mask.at<uchar>(i+1,j)=255;
								*(pointloc+pointcount*5)=i+1;
								*(pointloc+pointcount*5+1)=j;
								*(pointloc+pointcount*5+2)=b1;
								*(pointloc+pointcount*5+3)=g1;
								*(pointloc+pointcount*5+4)=r1;
								pointcount++;
						}
					}
					//if(i+1<height&&j+1<width)
					//{
					//	flag=mask.at<uchar>(i+1,j+1);
					//	refer=refermask.at<uchar>(i+1,j+1);
					//	edge=referedge.at<uchar>(i+1,j+1);
					//	b1=face.at<cv::Vec3b>(i+1,j+1)[0];
					//	g1=face.at<cv::Vec3b>(i+1,j+1)[1];
					//	r1=face.at<cv::Vec3b>(i+1,j+1)[2];
					//	if(255!=flag&&0!=refer&&0==edge
					//		&&abs(b1-b0)<curdifthb
					//		&&abs(g1-g0)<curdifthg
					//		&&abs(r1-r0)<curdifthr
					//		&&abs(b1-bs)<absdifthb
					//		&&abs(g1-gs)<absdifthg
					//		&&abs(r1-rs)<absdifthr){
					//			mask.at<uchar>(i+1,j+1)=255;
					//			*(pointloc+pointcount*5)=i+1;
					//			*(pointloc+pointcount*5+1)=j+1;
					//			*(pointloc+pointcount*5+2)=b1;
					//			*(pointloc+pointcount*5+3)=g1;
					//			*(pointloc+pointcount*5+4)=r1;
					//			pointcount++;
					//	}
					//}
					pointcount--;
					if(pointcount<0)
						break;
					i=*(pointloc+pointcount*5);
					j=*(pointloc+pointcount*5+1);
					b0=*(pointloc+pointcount*5+2);
					g0=*(pointloc+pointcount*5+3);
					r0=*(pointloc+pointcount*5+4);
				}
			}
		}
		roundnum++;
		if(seednum==roundnum)
			break;
	}
	free(pointloc);
}
void roundRectangle(cv::Mat &image, CvPoint lefttop, CvPoint rightbottom,int radius,   CvScalar color,int thickness=1, int line_type=8, int shift=0) 
{  	
	int temp;  
	if(lefttop.x>rightbottom.x)  	
	{  		
		temp=lefttop.x;  		
		lefttop.x=rightbottom.x;  		
		rightbottom.x=temp;  	
	}  
	if(lefttop.y>rightbottom.y)  	
	{  		
		temp=lefttop.y;  		
		lefttop.y=rightbottom.y;  		
		rightbottom.y=temp;  	
	}  	
	if(rightbottom.x-lefttop.x<2*radius || rightbottom.y-lefttop.y<2*radius)  	
	{  		
		radius=min((rightbottom.x-lefttop.x)/2,(rightbottom.y-lefttop.y)/2);  	
	}  	
	int radius1=30;
	CvPoint center;  	
	//if(thickness>0)  	
	//{  		
	//	center=cvPoint(rightbottom.x-radius,lefttop.y+radius);  		
	//	ellipse(image,center,cvSize(radius,radius),0,270,360,color,1,line_type,shift);  		
	//	center=cvPoint(lefttop.x+radius,lefttop.y+radius);  		
	//	ellipse(image,center,cvSize(radius,radius),0,180,270,color,1,line_type,shift);  		
	//	center=cvPoint(lefttop.x+radius,rightbottom.y-radius);  		
	//	ellipse(image,center,cvSize(radius,radius),0,90,180,color,1,line_type,shift);  		
	//	center=cvPoint(rightbottom.x-radius,rightbottom.y-radius);  		
	//	ellipse(image,center,cvSize(radius,radius),0,0,90,color,1,line_type,shift);  		
	//	line(image,cvPoint(lefttop.x+radius,lefttop.y),cvPoint(rightbottom.x-radius,lefttop.y),color,1,line_type,shift);  		
	//	line(image,cvPoint(lefttop.x+radius,rightbottom.y),cvPoint(rightbottom.x-radius,rightbottom.y),color,1,line_type,shift);  		
	//	line(image,cvPoint(lefttop.x,lefttop.y+radius),cvPoint(lefttop.x,rightbottom.y-radius),color);  		
	//	line(image,cvPoint(rightbottom.x,lefttop.y+radius),cvPoint(rightbottom.x,rightbottom.y-radius),color,1,line_type,shift); 	
	//}  	
	//else   	
	{  		
		center=cvPoint(rightbottom.x-radius,lefttop.y+radius);  		
		ellipse(image,center,cvSize(radius,radius),0,270,360,color,-1,line_type,shift);  		
		center=cvPoint(lefttop.x+radius,lefttop.y+radius);  		
		ellipse(image,center,cvSize(radius,radius),0,180,270,color,-1,line_type,shift);  		
		center=cvPoint(lefttop.x+radius1,rightbottom.y-radius1);  		
		ellipse(image,center,cvSize(radius1,radius1),0,90,180,color,-1,line_type,shift);  		
		center=cvPoint(rightbottom.x-radius1,rightbottom.y-radius1);  		
		ellipse(image,center,cvSize(radius1,radius1),0,0,90,color,-1,line_type,shift);  		
		rectangle(image,cvPoint(lefttop.x+radius,lefttop.y),cvPoint(rightbottom.x-radius,lefttop.y+radius),color,-1,line_type,shift);  		
		rectangle(image,cvPoint(lefttop.x+radius1,rightbottom.y-radius1),cvPoint(rightbottom.x-radius1,rightbottom.y),color,-1,line_type,shift);  		
		rectangle(image,cvPoint(lefttop.x,lefttop.y+radius),cvPoint(lefttop.x+radius1,rightbottom.y-radius1),color,-1,line_type,shift);  		
		rectangle(image,cvPoint(rightbottom.x-radius1,lefttop.y+radius),cvPoint(rightbottom.x,rightbottom.y-radius1),color,-1,line_type,shift);  		
		rectangle(image,cvPoint(lefttop.x+radius1,lefttop.y+radius),cvPoint(rightbottom.x-radius1,rightbottom.y-radius1),color,-1,line_type,shift);  	
	} 
}
void drawbody(cv::Mat &mask,int bodystarty,int bodymidy)
{
	int imgheight=mask.rows;
	int imgwidth=mask.cols;
	cv::Mat ele1=cv::getStructuringElement(cv::MORPH_ELLIPSE, CvSize(8,8),cv::Point(0,0));
		cv::dilate(mask,mask,ele1);
		//cv::dilate(mask,mask,ele1);
		//cv::dilate(mask,mask,ele1); 
		vector<vector<cv::Point> > contours; 
		vector<cv::Vec4i> hierarchy;  
		cv::Mat bodymat;
		mask.copyTo(bodymat);
		for(int i=0;i<bodystarty;i++)
		{
			memset(bodymat.data+i*bodymat.step,0,bodymat.step);
		}
		for(int i=bodymidy;i<imgheight;i++)
		{
			memset(bodymat.data+i*bodymat.step,0,bodymat.step);
		}
		int left,right,top,bottom;
		bool flag=false;
		cv::Point leftp(imgwidth-1,imgheight-1);
		for(int j=0;j<imgwidth;j++)
		{
			for(int i=0;i<imgheight;i++)
			{
				if(bodymat.at<uchar>(i,j))
				{
					leftp.x=j;
					leftp.y=i;
					flag=true;
					break;
				}
			}
			if(flag)
				break;
		}
		cv::Point rightp(imgwidth-1,imgheight-1);
		flag=false;
		for(int j=imgwidth-1;j>=0;j--)
		{
			for(int i=0;i<imgheight;i++)
			{
				if(bodymat.at<uchar>(i,j))
				{
					rightp.x=j;
					rightp.y=i;
					flag=true;
					break;
				}
			}
			if(flag)
				break;
		}
		int mid=(leftp.x+rightp.x)/2;
		//for(int i=leftp.y+1;i<bodymidy;i++)
		//{
		//	for(int j=leftp.x;j<=mid;j++)
		//		bodymat.at<uchar>(i,j)=0;
		//}
		//for(int i=rightp.y+1;i<bodymidy;i++)
		//{
		//	for(int j=mid;j<=rightp.x;j++)
		//		bodymat.at<uchar>(i,j)=0;
		//}
		//imshow("body",bodymat);
		//cvWaitKey(0);
		cv::findContours( bodymat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));	
		vector<vector<cv::Point> >hull( contours.size() );
		 for( int i = 0; i < contours.size(); i++ )
		 {  convexHull( cv::Mat(contours[i]), hull[i], false ); }

		 /// �����������͹��
		 cv::Mat drawing(imgheight,imgwidth,CV_8UC1,cvScalar(0));
		 for( int i = 0; i< contours.size(); i++ )
		 {
			 drawContours( drawing, hull, i, 255, -1, 8, vector<cv::Vec4i>(), 0, cv::Point() );
			 //drawContours( mask, hull, i, 255, -1, 8, vector<cv::Vec4i>(), 0, cv::Point() );
		 }
		for(int i=bodystarty;i<leftp.y+1;i++)
		{
			for(int j=leftp.x;j<=mid;j++)
				mask.at<uchar>(i,j)=drawing.at<uchar>(i,j);
		}
		for(int i=bodystarty;i<rightp.y+1;i++)
		{
			for(int j=mid;j<=rightp.x;j++)
				mask.at<uchar>(i,j)=drawing.at<uchar>(i,j);
		}
}
void drawhead(cv::Mat &mask,int headend)
{
	int imgheight=mask.rows;
	int imgwidth=mask.cols;
	vector<vector<cv::Point> > contours; 
	vector<cv::Vec4i> hierarchy;  
	cv::Mat bodymat;
	mask.copyTo(bodymat);
	for(int i=headend;i<=imgheight-1;i++)
	{
		memset(bodymat.data+i*bodymat.step,0,bodymat.step);
	}

	//imshow("head",bodymat);
	//cvWaitKey(0);
	cv::findContours( bodymat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));	
	vector<vector<cv::Point> >hull( contours.size() );
	 for( int i = 0; i < contours.size(); i++ )
	{  convexHull( cv::Mat(contours[i]), hull[i], false ); }

	 for( int i = 0; i< contours.size(); i++ )
	{
		// drawContours( drawing, hull, i, 255, 1, 8, vector<cv::Vec4i>(), 0, cv::Point() );
			drawContours( mask, hull, i, 255, -1, 8, vector<cv::Vec4i>(), 0, cv::Point() );
	}
}
void drawfacerect(cv::Mat face,std::vector<cv::Rect> faces,cv::Mat mask)
{
	int facenum=faces.size();
	int imgwidth=face.cols;
	int imgheight=face.rows;
	int width,height;
	int hist[1024]={0};
	cv::Mat grayface(imgheight,imgwidth,CV_8UC1,cvScalar(0));
	cv::cvtColor(face,grayface,CV_BGR2GRAY);
	CvPoint rightbottom,lefttop;
	SeedLoc seedloc[4];
	cv::Mat refermask(imgheight,imgwidth,CV_8UC1,cvScalar(0));
	cv::Mat tempmask(imgheight,imgwidth,CV_8UC1,cvScalar(0));
	int face1height=0;

	//cv::GaussianBlur(grayface,grayface,cv::Size(3,3),0,0);
	cv::Mat edge(imgheight,imgwidth,CV_8UC1,cvScalar(0));
	cv::Canny(grayface,edge,30,150,3); 

	for(int k=0;k<facenum;k++)
	{
		if(face1height>min(imgwidth,imgheight)/4)
			continue;
		int facexs=faces[k].x;
		int faceys=faces[k].y;
		int facexe=facexs+faces[k].width;
		int faceye=faceys+faces[k].height;
		int facewidth=faces[k].width;
		int faceheight=faces[k].height;
		int noskincount=0;
		for(int i=faceys;i<faceye;i++)
		{
			for(int j=facexs;j<facexe;j++)
			{
				if(face.at<cv::Vec3b>(i,j)[0]>face.at<cv::Vec3b>(i,j)[2])
					noskincount++;
			}
		}
		if(noskincount>facewidth*faceheight/3)
			continue;

		for(int i=faceys;i<faceye;i++)
		{
			for(int j=facexs;j<facexe;j++)
			{
				if(face.at<cv::Vec3b>(i,j)[2]>face.at<cv::Vec3b>(i,j)[0]&&face.at<cv::Vec3b>(i,j)[2]>face.at<cv::Vec3b>(i,j)[1])
					noskincount++;
			}
		}
		if(noskincount<facewidth*faceheight/3)
			continue;

		face1height=faceheight;

		memset(tempmask.data,0,tempmask.step*tempmask.rows);

		//headinfo
		int headys=max(faceys-min(faceheight/2,60),0);
		int headye=min(faceye+faceheight/2,imgheight-1);
		int headheight=headye-headys+1;;
		int headxe=min(facexe+facewidth/3,imgwidth-1);
		int headxs=max(facexs-facewidth/3,0);
		int headwidth=headxe-headxs+1;
		int headcenterx=(headxs+headxe)/2;
		int headcentery=(headys+headye)/2;

		//bodyinfo
		int bodyxe=headxe+facewidth>imgwidth-1?imgwidth-1:headxe+facewidth;
		int bodyye=headye+headheight*6>imgheight-1?imgheight-1:headye+headheight*6;
		int bodyxs=headxs-facewidth<0?0:headxs-facewidth;
		int bodyys=faceye;
		rightbottom = cvPoint(bodyxe,bodyye);		
		lefttop = cvPoint(bodyxs,bodyys);

		ellipse(refermask,cvPoint(headcenterx,headcentery),cvSize(headwidth/2,headheight/2),0,0,360,255,-1);
		roundRectangle(refermask,lefttop,rightbottom,(bodyxe-bodyxs)/3,cvScalar(255,0),-1,8,0); 
		//refermask.copyTo(mask);
		//continue;

	/*	cv::Mat ele=cv::getStructuringElement(1, CvSize(2,2),cv::Point(0,0));
		cv::dilate(edge,edge,ele);

		for(int i=0;i<imgheight;i++)
		{
			for(int j=0;j<imgwidth;j++)
			{
				if(0==refermask.at<uchar>(i,j))
					edge.at<uchar>(i,j)=0;
			}
		}*/

		imshow("refermask",refermask);
		imshow("edge",edge);


	/*	cv::Mat ele=cv::getStructuringElement(1, CvSize(3,3),cv::Point(0,0));
		cv::Mat revsedge;
		revsedge=255-edge;

		cv::erode(revsedge,revsedge,ele);
		imshow("reverseedge",revsedge);
		cvWaitKey(0);*/

		////colorgrowseedinfo
		int seed1xs=facexs+facewidth/3;
		int seed1xe=facexe-facewidth/3;
		int seed1ys=faceys-min(faceheight/4,60)<0?0:faceys-min(faceheight/4,60);
		//int seed1ys=faceys;
		int seed1ye=faceye+faceheight*2>imgheight-1?imgheight-1:faceye+faceheight*2;
		//int seed2ys=min(headye+min(faceheight/3,30),imgheight-1);
		int seed2ys=min(headye+min(faceheight/5,30),imgheight-1);;
		int seed2ye=min(headye+min(faceheight*2/3,60),imgheight-1);
		int seed2xs=max(facexs-min(facewidth*2/5,40),0);
		int seed2xe=min(facexe+min(facewidth*2/5,40),imgwidth-1);
		int seed3ys=min(headye+faceheight,imgheight-1);
		int seed3ye=min(headye+faceheight*3/2,imgheight-1);
		int seed3xs=max(facexs-min(facewidth*2/5,30),0);
		int seed3xe=min(facexe+min(facewidth*2/5,30),imgwidth-1);
		int seed4xs=facexs;
		int seed4xe=facexe;
		int seed4ys=faceye+faceheight*2>imgheight-1?imgheight-1:faceye+faceheight*2;
		int seed4ye=faceye+faceheight*4>imgheight-1?imgheight-1:faceye+faceheight*4;


		seedloc[0].x1=seed1xs;
		seedloc[0].x2=seed1xe;
		seedloc[0].y1=seed1ys;
		seedloc[0].y2=seed1ye;
		seedloc[1].x1=seed2xs;
		seedloc[1].x2=seed2xe;
		seedloc[1].y1=seed2ys;
		seedloc[1].y2=seed2ye;
		seedloc[2].x1=seed3xs;
		seedloc[2].x2=seed3xe;
		seedloc[2].y1=seed3ys;
		seedloc[2].y2=seed3ye;
		seedloc[3].x1=seed4xs;
		seedloc[3].x2=seed4xe;
		seedloc[3].y1=seed4ys;
		seedloc[3].y2=seed4ye;


		////colorgrowseedinfo
		//int seed1xs=facexs;
		//int seed1xe=facexe;
		//int seed1ys=faceye+faceheight/2>imgheight-1?imgheight-1:faceye+faceheight/2;
		//int seed1ye=faceye+faceheight*4>imgheight-1?imgheight-1:faceye+faceheight*4;
		//int seed2ys=min(headye+min(faceheight/3,30),imgheight-1);
		//int seed2ye=min(headye+min(faceheight*2/3,60),imgheight-1);
		//int seed2xs=max(facexs-facewidth*2/3,0);
		//int seed2xe=min(facexe+facewidth*2/3,imgwidth-1);
		//int seed3ys=min(headye+faceheight,imgheight-1);
		//int seed3ye=min(headye+faceheight*3/2,imgheight-1);
		//int seed3xs=max(facexs-min(facewidth*2/5,30),0);
		//int seed3xe=min(facexe+min(facewidth*2/5,30),imgwidth-1);


		//seedloc[0].x1=seed1xs;
		//seedloc[0].x2=seed1xe;
		//seedloc[0].y1=seed1ys;
		//seedloc[0].y2=seed1ye;
		//seedloc[1].x1=seed2xs;
		//seedloc[1].x2=seed2xe;
		//seedloc[1].y1=seed2ys;
		//seedloc[1].y2=seed2ye;
		//seedloc[2].x1=seed3xs;
		//seedloc[2].x2=seed3xe;
		//seedloc[2].y1=seed3ys;
		//seedloc[2].y2=seed3ye;
		for(int i=0;i<4;i++)
		{
			line(grayface,cvPoint(seedloc[i].x1,seedloc[i].y1),cvPoint(seedloc[i].x2,seedloc[i].y1),255,1);  
			line(grayface,cvPoint(seedloc[i].x1,seedloc[i].y1),cvPoint(seedloc[i].x1,seedloc[i].y2),255,1);  
			line(grayface,cvPoint(seedloc[i].x1,seedloc[i].y2),cvPoint(seedloc[i].x2,seedloc[i].y2),255,1);  
			line(grayface,cvPoint(seedloc[i].x2,seedloc[i].y1),cvPoint(seedloc[i].x2,seedloc[i].y2),255,1);  
		}
		imshow("seedloc",grayface);
		//imwrite("d:\\seedloc.jpg",grayface);
		//cvWaitKey(0);

		colorgrow(face,tempmask,refermask,edge,seedloc,4,faces[k]);

		imshow("contour1",tempmask);
		cvWaitKey(0);

		//smallerheadinfo
		int smallerheadys=max(faceys-faceheight/3,0);
		int smallerheadye=min(faceye+faceheight/3,imgheight-1);
		int smallerheadxs=max(facexs-facewidth/4,0);
		int smallerheadxe=min(facexe+facewidth/4,imgwidth-1);
		int smallerheadwidth=smallerheadxe-smallerheadxs+1;
		int smallerheadheight=smallerheadye-smallerheadys+1;
		int smallerheadcenterx=(smallerheadxe+smallerheadxs)/2;
		int smallerheadcentery=(smallerheadye+smallerheadys)/2;
		//ellipse(mask,cvPoint(smallerheadcenterx,smallerheadcentery),cvSize(smallerheadwidth/2,smallerheadheight/2),0,0,360,255,-1);

		//imshow("colorgraymask",mask);
		//cvWaitKey(0);


		//��һ�滭�Ĵ��Է�Χ���������ˣ�����~~
		//dilate(mask,mask,NULL);
		//dilate(mask,mask,NULL);
		//dilate(mask,mask,NULL);


		//CvPoint leftshoulder=cvPoint(bodyxs,bodyys);
		//CvPoint rightshoulder=cvPoint(bodyxe,bodyys);
		//bool flag=false;
		//for(int j=bodyxs;j<bodyxe;j++)
		//{
		//	for(int i=bodyys;i<bodyye;i++)
		//	{
		//		if(255==mask.at<uchar>(i,j)&&i>bodyys)
		//		{
		//			leftshoulder.x=j;
		//			leftshoulder.y=i;
		//			flag=true;
		//			break;
		//		}
		//	}
		//	if(flag)
		//		break;
		//}
		//flag=false;
		//for(int j=bodyxe;j>bodyxs;j--)
		//{
		//	for(int i=bodyys;i<bodyye;i++)
		//	{
		//		if(255==mask.at<uchar>(i,j)&&i>bodyys)
		//		{
		//			rightshoulder.x=j;
		//			rightshoulder.y=i;
		//			flag=true;
		//			break;

		//		}
		//	}
		//	if(flag)
		//		break;
		//}

		//int drawleftcenterx=smallerheadcenterx;
		//int drawleftcentery=leftshoulder.y;
		//int drawleftwidth=max(smallerheadcenterx-leftshoulder.x,0);
		//int drawleftheight=max(leftshoulder.y-(faceye+faceys)/2,0);
		//ellipse(mask,cvPoint(drawleftcenterx,drawleftcentery),cvSize(drawleftwidth,drawleftheight),0,180,270,255,-1);

		//int drawrightcenterx=smallerheadcenterx;
		//int drawrightcentery=rightshoulder.y;
		//int drawrightwidth=max(rightshoulder.x-smallerheadcenterx,0);
		//int drawrightheight=max(rightshoulder.y-(faceye+faceys)/2,0);
		//ellipse(mask,cvPoint(drawrightcenterx,drawrightcentery),cvSize(drawrightwidth,drawrightheight),0,270,360,255,-1);

		//rectangle( mask, cvPoint(drawrightcenterx,drawrightcentery), cvPoint(rightshoulder.x, bodyye), 255, -1, 4, 0 ); 
		//rectangle( mask, cvPoint(leftshoulder.x,drawleftcentery), cvPoint(drawleftcenterx, bodyye), 255, -1, 4, 0 ); 
		//��һ�滭�Ĵ��Է�Χ���������ˣ�����~~


		//�������з�0����������
		/*vector<vector<cv::Point> > contours; 
		vector<cv::Vec4i> hierarchy;  
		cv::findContours( mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));	
		for( int i = 0; i < contours.size(); i++ )
		{
			drawContours(mask, contours, i, 255, -1, 8, hierarchy, 0, cv::Point()); 
		}

		cv::Mat ele=cv::getStructuringElement(2, CvSize(3,3),cv::Point(0,0));
		cv::erode(mask,mask,ele);
		cv::Mat ele1=cv::getStructuringElement(2, CvSize(5,5),cv::Point(0,0));
		cv::dilate(mask,mask,ele1);
		cv::dilate(mask,mask,ele1);
		cv::dilate(mask,mask,ele1);*/

		drawbody(tempmask,smallerheadcentery+faceheight/3,min(faceye+faceheight*3,imgheight-1));

		drawhead(tempmask,smallerheadcentery+faceheight/2);
		//drawhead(mask,faceye);

		//imshow("contour",mask);
		//cvWaitKey(0);

	//cv::Mat ele1=cv::getStructuringElement(cv::MORPH_ELLIPSE, CvSize(15,15),cv::Point(0,0));
	//	cv::erode(mask,mask,ele1);
	//	cv::erode(mask,mask,ele1);
	//	cv::dilate(mask,mask,ele1);
	//	cv::dilate(mask,mask,ele1);
	//	cv::dilate(mask,mask,ele1);


	//	// imshow("outline",drawing);

	//	imshow("contour",mask);
	//	cvWaitKey(0);
	
	//�������з�0����������
		vector<vector<cv::Point> > contours; 
		vector<cv::Vec4i> hierarchy;  
		cv::findContours( tempmask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));	
		for( int i = 0; i < contours.size(); i++ )
		{
			drawContours(mask, contours, i, 255, -1, 8, hierarchy, 0, cv::Point()); 
		}



		//cvReleaseMemStorage(&storage);
		//cvReleaseImage(&maxareaimage);
	}

		//imshow("contour2",mask);
		//imwrite("d:\\contour2.jpg",mask);
		//cv::Mat ele=cv::getStructuringElement(2, CvSize(3,3),cv::Point(0,0));
		//cv::erode(mask,mask,ele);
		//cv::Mat ele1=cv::getStructuringElement(1, CvSize(8,8),cv::Point(0,0));
		//cv::erode(mask,mask,ele1);
		//cv::erode(mask,mask,ele1);
		//cv::erode(mask,mask,ele1);
		imshow("contour",mask);
		//imwrite("d:\\contour.jpg",mask);
		//cvWaitKey(0);

}
void find_face2(String face_cascade_name, cv::Mat &face,std::vector<cv::Rect> faces){
//	string face_cascade_name = "E:\\soft\\opencv-3.0.0\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
//	//����ʹ�õ�LBP��������ٶȽ�haar������ٶȿ죬û�еĻ�ʹ��opencv�Դ���haar���������Ҳ����
	cv::CascadeClassifier face_cascade;
	face_cascade.load(face_cascade_name);

	//cv_image<bgr_pixel> img(face);
	cv::Mat face_gray;
	cvtColor( face, face_gray, CV_BGR2GRAY );  //rgb����ת��Ϊ�Ҷ�����
	cv::equalizeHist( face_gray, face_gray );   //ֱ��ͼ���⻯
	face_cascade.detectMultiScale(face_gray,faces,1.2,2,0|CV_HAAR_FIND_BIGGEST_OBJECT,cv::Size(20,20));
}

void find_face( cv::CascadeClassifier &face_cascade, cv::Mat &face,cv::Mat &mask){
	//cv_image<bgr_pixel> img(face);
	cv::Mat face_gray;
	std::vector<cv::Rect> faces;
    cvtColor( face, face_gray, CV_BGR2GRAY );  //rgb����ת��Ϊ�Ҷ�����
    cv::equalizeHist( face_gray, face_gray );   //ֱ��ͼ���⻯
    face_cascade.detectMultiScale(face_gray,faces,1.2,2,0|CV_HAAR_FIND_BIGGEST_OBJECT,cv::Size(20,20));	
	drawfacerect(face,faces,mask);
}
void gaussianbur(cv::Mat &mat,cv::Mat &mask,int minsize=5)
{
	int imgwidth=mat.cols;
	int imgheight=mat.rows;
	//cv::Mat mask0(imgheight,imgwidth,CV_8UC1,CvScalar(0));
	//int smallsize=min(imgheight,imgwidth);
	//ellipse(mask0,cvPoint(imgwidth/2,imgheight/2),cvSize(imgwidth/6,imgheight/6),0,0,360,255,-1);
	//cv::Mat matblur(imgheight,imgwidth,CV_8UC3,CvScalar(0));
	//cv::GaussianBlur(mat,matblur,cv::Size(minsize,minsize),0,0);
	//for(int i=0;i<imgheight;i++)
	//{
	//	for(int j=0;j<imgwidth;j++)
	//	{	
	//		if(0!=mask0.at<uchar>(i,j))
	//			mat.at<cv::Vec3b>(i,j)=matblur.at<cv::Vec3b>(i,j);
	//	}
	//}
	//cv::Mat mask1(imgheight,imgwidth,CV_8UC1,CvScalar(0));
	//ellipse(mask1,cvPoint(imgwidth/2,imgheight/2),cvSize(imgwidth/3,imgheight/3),0,0,360,255,-1);
	//cv::GaussianBlur(mat,matblur,cv::Size(minsize*2+1,minsize*2+1),0,0);
	//for(int i=0;i<imgheight;i++)
	//{
	//	for(int j=0;j<imgwidth;j++)
	//	{	
	//		if(0==mask0.at<uchar>(i,j)&&0!=mask1.at<uchar>(i,j))
	//			mat.at<cv::Vec3b>(i,j)=matblur.at<cv::Vec3b>(i,j);
	//	}
	//}
	//cv::GaussianBlur(mat,matblur,cv::Size(minsize*3,minsize*3),0,0);
	//for(int i=0;i<imgheight;i++)
	//{
	//	for(int j=0;j<imgwidth;j++)
	//	{	
	//		if(0==mask1.at<uchar>(i,j))
	//			mat.at<cv::Vec3b>(i,j)=matblur.at<cv::Vec3b>(i,j);
	//	}
	//}
	//cv::blur(matblur,matblur,cv::Size(size,size));
	// medianBlur(matblur,matblur,size);
	//imshow("blurback",mat);
	//cvWaitKey(0);
	cv::Mat ele1=cv::getStructuringElement(cv::MORPH_ELLIPSE, CvSize(3,3),cv::Point(0,0));
	//erode(mask,mask,ele1);
	//imshow("immat1",mat);
	cv::Mat mask1;
	int sh,sw;
	cv::Size dsize;
	cv::Mat smat;
	if(imgheight>500&&imgwidth>500)
	{
		sh=imgheight/2;
		sw=imgwidth/2;
		dsize = cv::Size(sw,sh);
		mask1=cv::Mat(dsize,CV_8UC1);
		resize(mask, mask1,dsize);
		smat= cv::Mat(dsize,CV_8UC3);
		resize(mat, smat,dsize);
	}
	else
	{
		sh=imgheight;
		sw=imgwidth;
		dsize = cv::Size(sw,sh);
		mask1=cv::Mat(dsize,CV_8UC1);
		resize(mask, mask1,dsize);
		smat= cv::Mat(dsize,CV_8UC3);
		resize(mat, smat,dsize);
	}
	erode(mask1,mask1,ele1);
	int blurround=0;
	while(1)
	{
		cv::Mat matblur(sh,sw,CV_8UC3,CvScalar(0));
		cv::GaussianBlur(smat,matblur,cv::Size(3,3),0,0);
		for(int i=0;i<sh;i++)
		{
			for(int j=0;j<sw;j++)
			{	
				if(0==mask1.at<uchar>(i,j))
					smat.at<cv::Vec3b>(i,j)=matblur.at<cv::Vec3b>(i,j);
			}
		}
		dilate(mask1,mask1,ele1);
		blurround++;
		if(8==blurround)
			break;
	}
	resize(smat, mat,cv::Size(imgwidth,imgheight));

	//while(1)
	//{
	//	cv::Mat matblur(imgheight,imgwidth,CV_8UC3,CvScalar(0));
	//	cv::GaussianBlur(mat,matblur,cv::Size(15,15),0,0);
	//	for(int i=0;i<imgheight;i++)
	//	{
	//		for(int j=0;j<imgwidth;j++)
	//		{	
	//			if(0==mask1.at<uchar>(i,j))
	//				mat.at<cv::Vec3b>(i,j)=matblur.at<cv::Vec3b>(i,j);
	//		}
	//	}
		//dilate(mask1,mask1,ele1);
		//blurround++;
		//if(3==blurround)
			//break;
	//}
	//imshow("img",mat);
	//cvWaitKey(0);
}
void imgmix(cv::Mat &img,cv::Mat &back,cv::Mat &mask)
{
	int imgheight=img.rows;
	int imgwidth=img.cols;
	for(int i=0;i<imgheight;i++)
	{
		for(int j=0;j<imgwidth;j++)
		{	
			if(0==mask.at<uchar>(i,j))
				img.at<cv::Vec3b>(i,j)=back.at<cv::Vec3b>(i,j);
		}
	}
}

void backblur(cv::CascadeClassifier &face_cascade,cv::Mat &imgori)
{
	cv::Mat img;
	imgori.copyTo(img);

	cv::Mat mask(img.rows,img.cols,CV_8UC1,CvScalar(0));
	find_face(face_cascade,img,mask);

	//cv::Mat imgblur;
	//imgori.copyTo(imgblur);
	gaussianbur(imgori,mask);

	int imgheight=imgori.rows;
	int imgwidth=imgori.cols;

	for(int i=0;i<imgheight;i++)
	{
		for(int j=0;j<imgwidth;j++)
		{
			if(0!=mask.at<uchar>(i,j))
			{
				img.at<cv::Vec3b>(i,j)[2]=255;
			}
		}
	}

	//cv::Mat ele1=cv::getStructuringElement(cv::MORPH_ELLIPSE, CvSize(5,5),cv::Point(0,0));
	//erode(mask,mask,ele1);
	//imgmix(imgori,imgblur,mask);
	//imgblur.copyTo(imgori);

	imshow("blur",imgori);
	cv::imshow("tagface",img);
	//cv::imshow("mask",mask);
	cvWaitKey(0);

}
int main(int argc, char** argv)
{
    string face_cascade_name = "E:\\soft\\opencv-3.0.0\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
    //����ʹ�õ�LBP��������ٶȽ�haar������ٶȿ죬û�еĻ�ʹ��opencv�Դ���haar���������Ҳ����
    cv::CascadeClassifier face_cascade;
    face_cascade.load(face_cascade_name);


	cv::Mat imgori=cv::imread("D:\\1.jpg");
	int imgwidth=imgori.cols;
	int imgheight=imgori.rows;
	//imshow("img",imgori);
	int smallwidth=imgwidth;
	int smallheight=imgheight;
	while(smallwidth>1500&&smallwidth>1500)
	{
		smallwidth/=2;
		smallheight/=2;
	}

		cv::Size dsize = cv::Size(smallwidth,smallheight);
		cv::Mat img=cv::Mat(dsize,CV_8UC1);
		resize(imgori, img,dsize);
		cv::Mat(dsize,CV_8UC3);

	imshow("img",img);

	backblur(face_cascade,img);

	return 0;
}