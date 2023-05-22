#include <iostream>
#include <fstream>
#include "json.h"
#include "reader.h"
#include <reader.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <pcl/io/pcd_io.h>
#include <string>
#include <Eigen/Dense>
#include <math.h>
#include <io.h>
#include <windows.h>
#include <vector>
#include "tinystr.h"
#include "tinyxml.h"
#include <shlwapi.h>
#include<stdio.h>
#include<sstream>
#include<vector>
#include <stdlib.h>
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace std;
using namespace cv;
using namespace Eigen;
#define Pi 3.14159265358979323846
vector<cv::Point3f> Generate3DPoints();
int dx = 0;
string gongcheng = "89";//!!!!!
vector <float> Omega;
vector <float> Cenx;


void displayxml(const char* dbname)
{
    TiXmlDocument xmlDocument;
    string xml_file = "E://niutifenge//xml//" + gongcheng + ".xml";//!!!
    if (!xmlDocument.LoadFile("E://niutifenge//xml//89.xml")) // 需要解析的xml文件!!!!!
    {
        return;
    }

    TiXmlElement* xmlRoot = xmlDocument.RootElement(); // 找根节点BlocksExchange

    // 通过根节点开始遍历下面的所有 名为dbname的子节点，xmlRoot->FirstChildElement();不加参数表示遍历根节点下所有子节点
    // 加参数表示遍历名为dbname的子节点 elem = elem->NextSiblingElement()每次向后找下一个
    for (TiXmlElement* elem = xmlRoot->FirstChildElement(dbname); elem != NULL; elem = elem->NextSiblingElement())
    {
        //const char* arr = elem->Attribute("name"); // 取出子节点的属性（rtdb的name属性）
        ////const char* node1Name = elem->GetText();  // 取出标签中间的值，我的xml文件用不到
        //cout << "rtdb-name属性:" << arr << endl;

        // 通过rtdb继续找他下面的子节点
        // 查找方式和上面的一样 （要是下面还有属性可以继续通过这种方式继续找）
        for (TiXmlElement* elem1 = elem->FirstChildElement("Photogroups"); elem1 != NULL; elem1 = elem1->NextSiblingElement())
        {
            //const char* name = elem1->Attribute("name"); // data节点下的属性
            //const char* type = elem1->Attribute("type"); // data节点下的属性
            //const char* init = elem1->Attribute("init"); // data节点下的属性
            //const char* length = elem1->Attribute("length"); // data节点下的属性
            //cout << name << " " << type << " " << init << " " << length << endl; // 加上空格
            for (TiXmlElement* elem2 = elem1->FirstChildElement("Photogroup"); elem2 != NULL; elem2 = elem2->NextSiblingElement())
            {
                for (TiXmlElement* elem3 = elem2->FirstChildElement("Photo"); elem3 != NULL; elem3 = elem3->NextSiblingElement())
                {
                    for (TiXmlElement* elem4 = elem3->FirstChildElement("Pose"); elem4 != NULL; elem4 = elem4->NextSiblingElement())
                    {
                        for (TiXmlElement* elem5 = elem4->FirstChildElement("Rotation"); elem5 != NULL; elem5 = elem5->NextSiblingElement())
                        {

                            for (TiXmlElement* elem6 = elem5->FirstChildElement("Omega"); elem6 != NULL; elem6 = elem6->NextSiblingElement())
                            {

                                TiXmlNode* rawdata_node = NULL;

                                const char* Omegabuf = elem6->ToElement()->GetText();
                                cout << "Omega" << Omegabuf << endl;
                                float f = atof(Omegabuf);
                                Omega.push_back(f);
                            }

                        }
                        for (TiXmlElement* elem5 = elem4->FirstChildElement("Center"); elem5 != NULL; elem5 = elem5->NextSiblingElement())
                        {
                            for (TiXmlElement* elem6 = elem5->FirstChildElement(); elem6 != NULL; elem6 = elem6->NextSiblingElement())
                            {

                                TiXmlNode* rawdata_node1 = NULL;

                                const char* Cenxbuf = elem6->ToElement()->GetText();
                                cout << "x" << Cenxbuf << endl;
                                float f = atof(Cenxbuf);
                                Cenx.push_back(f);
                            }

                        }
                    }
                }
            }


        }
    }
}
/* 函数功能: 判断点(x, y)是否在有ploy_sides个顶点的多边形内 */
/* 参数: poly_sides	测试多边形的顶点数
**		 poly_x	测试多边形的各个顶点的X轴坐标
**		 poly_y	测试多边形的各个顶点的Y轴坐标
**		 x	测试点的X轴坐标
**		 Y 	测试点的Y轴坐标 */
/* 返回值: 返回0 表示不在多边形内部，返回1 表示在多边形内部 */
/* 说明: 在多边形各边上的点默认不在多边形内部 */
int inOrNot(int poly_sides, float* poly_X, float* poly_Y, float x, float y)
{

    int i, j;

    j = poly_sides - 1;

    int res = 0;

    for (i = 0; i < poly_sides; i++)
    {
        if ((poly_Y[i] < y && poly_Y[j] >= y || poly_Y[j] < y && poly_Y[i] >= y) && (poly_X[i] <= x || poly_X[j] <= x))
        {
            res ^= ((poly_X[i] + (y - poly_Y[i]) / (poly_Y[j] - poly_Y[i]) * (poly_X[j] - poly_X[i])) < x);
        }

        j = i;
    }

    return res;
}



/*角度转换*/
inline float itod(float o)
{
    return Pi / 180 * o;
}

struct Shapes {
    std::string label;
    int group_id;
    std::vector<cv::Point> points;
};
static std::string readInputTestFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file)
        return std::string("");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    std::string text;
    char* buffer = new char[size + 1];
    buffer[size] = 0;
    if (fread(buffer, 1, size, file) == (unsigned long)size)
        text = buffer;
    fclose(file);
    delete[] buffer;
    return text;
}
void readFileJson(std::string& jsonfile, std::vector<Shapes>& shapes) {
    shapes.clear();

    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());

    Json::Value root;//Define the root node
    Json::String input = readInputTestFile(jsonfile.c_str());
    if (input.empty())
    {
        std::cerr << "Invalid input file: " << jsonfile << std::endl;
    }

    JSONCPP_STRING errs;
    if (reader->parse(input.c_str(), input.c_str() + std::strlen(input.c_str()), &root, &errs))
    {
        Json::Value::Members members(root.getMemberNames());
        //std::sort(members.begin(), members.end());
        for (const auto& name : members)//curly braces
        {
            Json::Value root1 = root[name];
            if (name == "shapes")//Find shapes
            {
                Json::ArrayIndex size1 = root1.size();
                for (Json::ArrayIndex index1 = 0; index1 < size1; ++index1)//square brackets
                {
                    Shapes shapesTmp;
                    std::vector<cv::Point2f> pointsVec;
                    Json::Value root2 = root1[index1];
                    Json::Value::Members members2(root2.getMemberNames());
                    for (const auto& name2 : members2)//Braces, each outline information
                    {
                        Json::Value root3 = root2[name2];
                        if (name2 == "label")
                        {
                            shapesTmp.label = root2[name2].asCString();
                            //std::cout << shapesTmp.label << std::endl;
                        }
                        else if (name2 == "group_id")
                        {
                            shapesTmp.group_id = root2[name2].asInt();
                            //std::cout << shapesTmp.group_id << std::endl;
                        }
                        else if (name2 == "points")//Find shapes
                        {
                            Json::ArrayIndex size3 = root3.size();
                            for (Json::ArrayIndex index3 = 0; index3 < size3; ++index3)//square brackets, points
                            {
                                int x = root3[index3][0].asFloat();
                                int y = root3[index3][1].asFloat();
                                pointsVec.push_back(cv::Point(x, y));
                            }
                            shapesTmp.points.assign(pointsVec.begin(), pointsVec.end());
                        }
                        else
                        {
                            continue;
                        }
                    }
                    shapes.push_back(shapesTmp);
                }
            }
        }
    }

    delete reader;
}






int main()
{
    /*读取相机外参数*/
    char r[] = "Block";
    displayxml(r);
    int a = 0;

    ofstream f("E:\\niutifenge\\waican\\"+gongcheng +".txt", ios::app);//写出相机外参，用于去除隐藏点!!!
    for (int jj = 0; jj < Cenx.size(); ++jj)
    {
        f << Cenx[jj] << '\n';
    }
    f.close();
    string srcPath = "E:\\niutifenge\\yvcetxt\\" +gongcheng+"\\";//读取txt，指定标注文件的文件夹!!!
    //string srcPath = "E:\\niutifenge\\yvcetxt\\44\\";//读取txt，指定标注文件的文件夹!!!
    vector <string> file_vec;//整个文件名包括路径
    vector <string> src;//整个文件名
    vector <string> src_name;//只有文件名，没有后缀
    glob(srcPath + "/*.txt", file_vec, false);
    for (string file_name : file_vec)//src读取文件夹下的所有文件名
    {
        int length = strlen(file_name.c_str()) - strlen(srcPath.c_str());
        string c = file_name.substr (strlen(srcPath.c_str())+1, length);
        src_name.push_back( c.erase(strlen(c.c_str())-4));
        cout << file_name << endl;

        src.push_back(file_name);
    }
    for (int numi = 0; numi < src.size(); ++numi)
    {
        pcl::PointCloud<pcl::PointXYZ> inside;
        pcl::PointCloud<pcl::PointXYZ> all;

        std::string file = src[numi];//逐个文件读取分割信息

        ifstream fp;
        vector<float> pp1;
        fp.open(file, ios::in);
        while (!fp.eof()) {
            string str1;
            getline(fp, str1);
            pp1.push_back(atoi(str1.c_str()));
            //cout << str1 << " " << str1.length() << endl;
        }

        //std::vector<Shapes> q;
        //readFileJson(file, q);

        //cout << "point " << q[0].points[0].x << endl;//先访问迭代器，在访问数据成员,再访问迭代器

        int n;
        n = (pp1.size()-1);
        float bian_x[10000];
        float bian_y[10000];


        for (int i = 0; i < (n/2); i++)
        {
            bian_x[i] = pp1[(i*2)];
            bian_y[i] = pp1[(i*2+1)];
        }

        int tmp = 0;
        int tmp1 = 0;
        int tmp2 = 0;
        // Read 3D points
        vector<cv::Point3f> objectPoints = Generate3DPoints();//读取三维点
        vector<cv::Point2f> imagePoints;
        cv::Mat intrisicMat(3, 3, cv::DataType<float>::type); // Intrisic matrix相机内参数矩阵!!!
        /*华为p20拍照参数*/
        intrisicMat.at<float>(0, 0) = 3223.005412748038;
        intrisicMat.at<float>(1, 0) = 0;
        intrisicMat.at<float>(2, 0) = 0;
        intrisicMat.at<float>(0, 1) = 0;
        intrisicMat.at<float>(1, 1) = 3224.726381635694;
        intrisicMat.at<float>(2, 1) = 0;
        //intrisicMat.at<float>(0, 2) = 1986.08631044703;
        intrisicMat.at<float>(0, 2) = 1986.08631044703;
        intrisicMat.at<float>(1, 2) = 1484.27464851322;
        intrisicMat.at<float>(1, 2) = 1484.27464851322;
        intrisicMat.at<float>(2, 2) = 1;
        /*小米视频0.6参数*/
        //intrisicMat.at<float>(0, 0) = 1016.836002388055;
        //intrisicMat.at<float>(1, 0) = 0;
        //intrisicMat.at<float>(2, 0) = 0;
        //intrisicMat.at<float>(0, 1) = 0;
        //intrisicMat.at<float>(1, 1) = 1027.625112266645;
        //intrisicMat.at<float>(2, 1) = 0;
        //intrisicMat.at<float>(0, 2) = 955.7328324461899;
        //intrisicMat.at<float>(1, 2) = 528.423488727248;
        //intrisicMat.at<float>(2, 2) = 1;
        /*旋转向量转旋转矩阵*/
        cv::Mat rVec;

        /**定义旋转矩阵R**/
        float o = Omega[numi*3];
        float p = Omega[numi*3+1];
        float k = Omega[numi*3+2];//o=omega w ,p=phi sita ,k=kappa k

        //p = itod(p);


        float m00, m01, m02, m10, m11, m12, m20, m21, m22;
        m00 = cos(itod(p)) * cos(itod(k));
        m01 = cos(itod(o)) * sin(itod(k)) + sin(itod(o)) * sin(itod(p)) * cos((itod(k)));
        m02 = sin(itod(o)) * sin(itod(k)) - cos(itod(o)) * sin(itod(p)) * cos(itod(k));
        m10 = -(cos(itod(p)) * sin(itod(k)));
        m11 = cos(itod(o)) * cos(itod(k)) - sin(itod(o)) * sin(itod(p)) * sin(itod(k));
        m12 = sin(itod(o)) * cos(itod(k)) + cos(itod(o)) * sin(itod(p)) * sin(itod(k));
        m20 = sin(itod(p));
        m21 = -sin(itod(o)) * cos(itod(p));
        m22 = cos(itod(o)) * cos(itod(p));



        cv::Mat rotm = (cv::Mat_<float>(3, 3) << m00, m01, m02, m10, m11, m12, m20, m21, m22);

        cv::Rodrigues(rotm, rVec);//将旋转矩阵转为旋转向量

        cv::Mat tVecc(3, 1, cv::DataType<float>::type); // Translation vector
        cv::Mat tVec(3, 1, cv::DataType<float>::type); // Translation vector


        /*t=-Rc*/

        tVecc.at<float>(0) = Cenx[numi * 3];
        tVecc.at<float>(1) = Cenx[numi * 3+1];

        tVecc.at<float>(2) = Cenx[numi * 3+2];
        tVec = -(rotm * tVecc);

        cv::Mat distCoeffs(5, 1, cv::DataType<float>::type);   // Distortion vector
        //distCoeffs.at<float>(0) = 0;
        //distCoeffs.at<float>(1) = 0;
        //distCoeffs.at<float>(2) = 0;
        //distCoeffs.at<float>(3) = 0;
        //distCoeffs.at<float>(4) = 0;
        distCoeffs.at<float>(0) = -0.06;
        distCoeffs.at<float>(1) = -0.1;
        distCoeffs.at<float>(2) = 0.03;
        distCoeffs.at<float>(3) = 0;
        distCoeffs.at<float>(4) = 0;

        //distCoeffs.at<float>(0) = 0.0003820978274277789;
        //distCoeffs.at<float>(1) = 0.0007662464889711536;
        //distCoeffs.at<float>(2) = 0.00007662003032086063;
        //distCoeffs.at<float>(3) = 0.0002471716204810778;
        //distCoeffs.at<float>(4) = -0.4066531505655897;

        /*小米视频0.6参数*/
        //distCoeffs.at<float>(0) = 0.0358839968692888;
        //distCoeffs.at<float>(1) = -0.4514010897863991;
        //distCoeffs.at<float>(2) = 0.0004571028911467427;
        //distCoeffs.at<float>(3) = -0.00165703558061161;
        //distCoeffs.at<float>(4) = 1.364721769787012;

        cout << "rotm " << rotm << endl;
        cout << "Intrisic matrix: " << intrisicMat << endl << endl;
        cout << "Rotation vector: " << rVec << endl << endl;
        cout << "Translation vector: " << tVec << endl << endl;
        cout << "Distortion coef: " << distCoeffs << endl << endl;
        std::vector<cv::Point2f> projectedPoints;
        cv::projectPoints(objectPoints, rVec, tVec, intrisicMat, distCoeffs, projectedPoints);
        Mat img = Mat::zeros(Size(3968 , 2976), CV_8UC3);//分割后的图像//!!!
        Mat imgqian = Mat::zeros(Size(3968, 2976), CV_8UC3);//分割前的图像
        
        /*设置图像背景为白色*/
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                if (img.channels() == 1)
                {
                    img.at<uchar>(i, j) = img.at<uchar>(i, j) + 100;
                }
                else if (img.channels() == 3)
                {
                    img.at<Vec3b>(i, j)[0] = 255;
                    img.at<Vec3b>(i, j)[1] = 255;
                    img.at<Vec3b>(i, j)[2] = 255;

                    imgqian.at<Vec3b>(i, j)[0] = 255;
                    imgqian.at<Vec3b>(i, j)[1] = 255;
                    imgqian.at<Vec3b>(i, j)[2] = 255;
                }
            }
        }

        //写图像
        //imwrite("F:\\pcl_test\\test9.13\\quantu\\111.jpg", imgqian);
        //imwrite("F:\\pcl_test\\test9.13\\quantu\\112.jpg", img);


        //int image_cols = 1920;//!!!
        //int image_rows = 1080;

        int image_cols = 3968;//!!!
        int image_rows = 2976;
        std::vector<cv::Point2f> pts_2d;
        for (unsigned int i = 0; i < projectedPoints.size(); ++i)
        {
            cv::Point2f point_2d = projectedPoints[i];
            if (point_2d.x < 0 || point_2d.x > image_cols || point_2d.y < 0 || point_2d.y > image_rows)
            {
                tmp2++;//图像外的点
                continue;
            }
            else
            {
                all.push_back(pcl::PointXYZ(objectPoints[i].x, objectPoints[i].y, objectPoints[i].z));
                tmp++;//图像内的点
                circle(imgqian, projectedPoints[i], 2, Scalar(0, 255, 0), -1);  // 画半径为5的圆(画点）
                if (inOrNot((n/2), bian_x, bian_y, point_2d.x, point_2d.y))//返回为1表示在多边形内部
                {
                    circle(imgqian, projectedPoints[i], 2, Scalar(0, 0, 255), -1);  // 画半径为5的圆(画点）
                    inside.push_back(pcl::PointXYZ(objectPoints[i].x, objectPoints[i].y, objectPoints[i].z));
                    tmp1++;//图像和框内的点
                }
            }
        }

        cout << "tmp: " << tmp << endl;
        cout << "tmp1: " << tmp1 << endl;
        cout << "tmp2: " << tmp2 << endl;
        //cv::imshow("origin image", img);
        //char ad[128] = { 0 };
        string ad1 = "E:\\niutifenge\\dianyuntuxiang\\" +gongcheng+"\\" + src_name[numi] + ".jpg";//!!!
        string ad = "F:\\pcl_test\\test9.13\\quantubai_jiaozheng\\" +gongcheng+"\\" + src_name[numi] + ".jpg";//!!!
        //sprintf_s(ad, "F:\\pcl_test\\test9.13\\quantubai_jiaozheng\\test%d.jpg", ++dx);

        //写图像
        imwrite(ad, imgqian);
        imwrite(ad1, img);
        pcl::PCDWriter writer;
        string command;
        string pcd ="E:\\niutifenge\\bufendianyun\\" +gongcheng+"\\" +src_name[numi] + ".pcd";//!!!
        string wenjianjiacpd = "E:\\niutifenge\\bufendianyun\\" + gongcheng;
        if (_access(wenjianjiacpd.c_str(), 0) == -1)
        {
            command = "mkdir -p " + wenjianjiacpd;
            system(command.c_str());
        }


        cout << "pcd: " << pcd << endl;
        pcl::io::savePCDFileASCII(pcd, inside);

        string allpcd = "F:\\pcl_test\\test9.13\\dianyunall\\ " + src_name[numi] + ".pcd";
        //pcl::io::savePCDFileASCII(allpcd, all);

        
    }
   
     return 0;
}
vector<cv::Point3f> Generate3DPoints()
{
    //创建一个PointCloud<PointXYZ> boost 共享指针并进行实例化
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    vector<cv::Point3f> points;
    float x, y, z;
    //pcd文件路径
    std::string pcd_in = "E://niutifenge//model//"+gongcheng+".pcd";//!!!
    if (pcl::io::loadPCDFile<pcl::PointXYZ>(pcd_in, *cloud) == -1)
        //打开点云文件
    {
        PCL_ERROR("Couldn't read file test_pcd.pcd\n");
    }
    for (int nIndex = 0; nIndex < cloud->points.size(); nIndex++)
    {
        cloud->points[nIndex].x;
        cloud->points[nIndex].y;
        cloud->points[nIndex].z;
        points.push_back(cv::Point3f(cloud->points[nIndex].x, cloud->points[nIndex].y, cloud->points[nIndex].z));
    }
    /* for (unsigned int i = 0; i < points.size(); ++i)
     {
         cout << points[i] << endl << endl;
     }*/
    return points;
}
