# Extracting-point-clouds-from-multi-view-RGB-images
This is an official implementation of a paper titled "Extracting cow point clouds from multi-view RGB images with an improved YOLACT++ instance segmentation"

All codes and data will be published here.

![MG_20211126_080519_5](https://github.com/dontlearncpp/Extracting-point-clouds-from-multi-view-RGB-images/assets/103402250/b3f4fbfb-e15a-49b6-a9a2-f653fc901c48)

![MG_20211126_080519_6](https://github.com/dontlearncpp/Extracting-point-clouds-from-multi-view-RGB-images/assets/103402250/a8f638b3-f592-49c6-8e52-1933eaa61eef)


![image](https://github.com/dontlearncpp/Extracting-point-clouds-from-multi-view-RGB-images/assets/103402250/9b12f530-ca77-4482-bff7-9c81fb809130)

1.Point clouds projection based on SFM
1.CPP

  (1)Used to parse XML files and output camera extrinsic parameters (used to remove hidden points)
  
  (2)Segment the scene point cloud according to the segmentation boundary, and only keep the points within the boundary
  
  INPUT:
  
  line45   string xml_file = "E://niutifenge//xml//" + gongcheng + ".xml";//Read xml file (camera parameters)!!! 89.xml is obtained from contextcapuure 
  
  line258  string srcPath = "E:\\niutifenge\\yvcetxt\\" +gongcheng+"\\";//read **boundary points** folder!!! 
  
  OUTPUT:
  
  line252   ofstream f("E:\\niutifenge\\waican\\"+gongcheng +".txt", ios::app);//Write out the **camera extrinsic parameters** to remove hidden points!!! 
  
  line480   string pcd ="E:\\niutifenge\\bufendianyun\\" +gongcheng+"\\" +src_name[numi] + ".pcd";//**Internal point cloud** of boundary points was output!!!
  
2.Cow body segmentation based on improved YOLACT++

For the implementation of YOLACT++, please refer to the official model, and the evaluation part was modified to output boundary point information

OUTPUT:

  line148 zuizhong_path = "/media/xingshixu/367a0adf-4bec-4c64-b23d-945aacb28ba5/yang/yolact++_resnet_ecam_cafpn_res/yolact/yolact-master/black_white/" + str(path[1]) #MASK
  
  line149 zuizhong_txt = "/media/xingshixu/367a0adf-4bec-4c64-b23d-945aacb28ba5/yang/yolact++_resnet_ecam_cafpn_res/yolact/yolact-master/txt/" + str(path[1]) #  **boundary points** of MASK(txt)
  
  
  
  
