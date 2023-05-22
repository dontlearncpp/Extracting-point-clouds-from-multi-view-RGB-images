# Extracting-point-clouds-from-multi-view-RGB-images
This is an official implementation of a paper titled "Extracting cow point clouds from multi-view RGB images with an improved YOLACT++ instance segmentation"

All codes and data will be published here.


![image](https://github.com/dontlearncpp/Extracting-point-clouds-from-multi-view-RGB-images/assets/103402250/9b12f530-ca77-4482-bff7-9c81fb809130)

1.CPP

  (1)用于解析XML文件，输出相机外参(用于去除隐藏点)
  
  (2)并根据分割边界分割动物点云，只保留边界内的点
  
  INPUT:
  
  line45   string xml_file = "E://niutifenge//xml//" + gongcheng + ".xml";//读取xml文件(相机参数)!!! 89.xml is obtained from contextcapuure 
  
  line258  string srcPath = "E:\\niutifenge\\yvcetxt\\" +gongcheng+"\\";//读取边界点文件夹!!! 
  
  OUTPUT:
  
  line252   ofstream f("E:\\niutifenge\\waican\\"+gongcheng +".txt", ios::app);//写出相机外参，用于去除隐藏点!!! 
  
  line480   string pcd ="E:\\niutifenge\\bufendianyun\\" +gongcheng+"\\" +src_name[numi] + ".pcd";//输出边界点内部点云!!!
  
  
  
