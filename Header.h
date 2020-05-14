#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>
#define int8 uint8_t
#define int16 uint16_t
#define int32 uint32_t
using namespace std;

struct BootSector {
	int16 BperSector;	//so byte tren sector
	int8 SperCluster;	//so sector tren cluster
	int16 BootSector;	//so sector thuoc vung Bootsector
	int8 numFAT;	//so bang FAT
	int32 vol_size;	//kick thuoc vol (sector)
	int32 FAT_size;	//kich thuoc 1 bang FAT (sector)
	int32 RDET_cluster;	//cluster bat dau cua RDET
	int32 FAT_empty;	//vi tri trong cua bang FAT
	int8 raw[490];
};
struct FAT_table {
	vector<int32> Fat;
};
struct Sector {
	int8 s[512];
};
struct Data {
	vector<Sector> sec;
};
struct Item {
	string name;
	int16 start_cluster;	//cluster bat dau cua file
	int16 n_cluster;
	int8 file;	//0: folder, 1:file
	string password;
};
struct Volume {
	BootSector BS;
	FAT_table FT;
	Data D;
	vector<Item> I;
};

//khoi tao vol
void initVol(Volume &vol);
//doc vol
void readFile(string filename, Volume &vol);
//ghi vol
void writeFile(string filename, Volume &vol);
//ham chuyen doi tu cluster sang sector (ko tinh phan sector trong vung boot + fat)
int32 ClusterToSector(int cluster, BootSector BS);
//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol);
//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
void exportItem(string filename, Volume &vol);
//copy 1 file tu ngoai vao vol
void importItem(string filename, Volume &vol);
//xoa 1 file hoac 1 folder
void deleteItem(string filename, Volume &vol);
//tao thong so phu hop cho cac bien cua boot sector
void createInfor(Volume &vol);
//ham tao password, hoi ng dung pass
void createPass(string filename, Volume &vol);

/*co the se thiet ke them bien luu lai vi tri cluster bat dau 
cua vung trong trong phan du lieu, de khoi phai duyet lai,
bien luu vi tri trong trong RDET + FAT nua.
*** hien tai chua co, do anh chua biet nen them o dau.
*/