import open3d as o3d
import numpy as np
import os
import open3d_tutorial as o3dtut
if __name__ == '__main__':
    gongcheng = "3"
    path = "E:\\niutifenge\\bufendianyun\\"+str(gongcheng)  # 点云路径
    files = os.listdir(path)  # 得到文件夹下的所有文件名称
    pcdwpath = "E:\\niutifenge\\rhp\\"+str(gongcheng)+"\\"  # 点云输出路径
    if os.path.exists(pcdwpath)==False:
        os.mkdir(pcdwpath)

    txtpath = "E:\\niutifenge\\waican\\"+str(gongcheng)+".txt"#相机外参
    data = np.loadtxt(txtpath)  # 将文件中数据加载到data数组里
    num = 0
    pcdp=[]
    for file in files:
        print("Load a ply point cloud, print it, and render it")
        f = path+"/"+file
        pcd = o3d.io.read_point_cloud(f)
        #print(pcd)
        #print(np.asarray(pcd.points))
        #o3d.visualization.draw_geometries([pcd])

        print("Convert mesh to a point cloud and estimate dimensions")

        diameter = np.linalg.norm(np.asarray(pcd.get_max_bound()) - np.asarray(pcd.get_min_bound()))
        #o3d.visualization.draw_geometries([pcd])

        print("Define parameters used for hidden_point_removal")
        camera = [data[num*3],data[num*3+1], data[num*3+2]]#相机位置x,y,z
        num=num+1
        radius = diameter * 100

        print("Get all points that are visible from given view point")
        _, pt_map = pcd.hidden_point_removal(camera, 3000)

        print("Visualize result")
        pcd = pcd.select_by_index(pt_map)

        #o3d.visualization.draw_geometries([pcd])
        pcdname = pcdwpath+str(num)+".pcd"
        o3d.io.write_point_cloud(pcdname, pcd)
        if num==1:
            pcdp=np.asarray(pcd.points)
        else:
            pcdp=np.concatenate((pcdp, np.asarray(pcd.points)), axis=0)
    pcd = o3d.geometry.PointCloud()
    pcdp = np.array(list(set([tuple(t) for t in pcdp])))
    pcd.points = o3d.utility.Vector3dVector(pcdp)
    o3d.io.write_point_cloud("end.pcd", pcd)



