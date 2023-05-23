# Extracting-point-clouds-from-multi-view-RGB-images
This is an official implementation of a paper titled "Extracting cow point clouds from multi-view RGB images with an improved YOLACT++ instance segmentation"

All codes and data will be published here.



![image](https://github.com/dontlearncpp/Extracting-point-clouds-from-multi-view-RGB-images/assets/103402250/9b12f530-ca77-4482-bff7-9c81fb809130)

1.Point clouds projection based on SFM
1.CPP

  (1)Used to parse XML files and output camera extrinsic parameters (used to remove hidden points)
  
  (2)Segment the scene point cloud according to the segmentation boundary, and only keep the points within the boundary
  
  INPUT:
  
  line45   string xml_file = "E://niutifenge//xml//" + gongcheng + ".xml";//Read xml file (camera parameters)!!! 89.xml is obtained from contextcapuure 
  
  line258  string srcPath = "E:\\niutifenge\\yvcetxt\\" +gongcheng+"\\";//read boundary point folder!!! 
  
  OUTPUT:
  
  line252   ofstream f("E:\\niutifenge\\waican\\"+gongcheng +".txt", ios::app);//Write out the camera extrinsic parameters to remove hidden points!!! 
  
  line480   string pcd ="E:\\niutifenge\\bufendianyun\\" +gongcheng+"\\" +src_name[numi] + ".pcd";//Internal point cloud of boundary points was output!!!
  
2.Cow body segmentation based on improved YOLACT++

For the implementation of YOLACT++, please refer to the official model, and the evaluation part was modified to output boundary point information
  
  
  
