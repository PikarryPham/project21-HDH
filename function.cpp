#include "Header.h"

//khoi tao vol
void initVol(Volume &vol)
{

}
//doc vol
void readFile(string filename, Volume &vol)
{

}
//ghi vol
void writeFile(string filename, Volume vol)
{

}
//ham chuyen doi tu cluster sang sector (ko tinh phan sector trong vung boot + fat)
int32 ClusterToSector(int cluster, BootSector BS)
{

}
//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol)
{

}
//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
void exportItem(string filename, Volume& vol)
{

}
//copy 1 file tu ngoai vao vol
void importItem(string filename, Volume &vol)
{

}
//xoa 1 file hoac 1 folder
void deleteItem(string filename, Volume &vol)
{

}
//tao thong so phu hop cho cac bien cua boot sector
void createInfor(Volume &vol)
{

}
//ham tao password, hoi ng dung pass
void createPass(string filename, Volume &vol)
{

}