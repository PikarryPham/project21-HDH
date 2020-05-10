#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>
#define int8 uint8_t
using namespace std;

struct BootSector {
	int8 BperSector[2];	//so byte tren sector
	int8 SperCluster;	//so sector tren cluster
	int8 BootSector[2];	//so sector thuoc vung Bootsector
	int8 numFAT;	//so bang FAT
	int8 vol_size[4];	//kick thuoc vol (sector)
	int8 FAT_size[4];	//kich thuoc 1 bang FAT (sector)
	int8 RDET_cluster[4];	//cluster bat dau cua RDET
	int8 S_minor[2];	//sector chua thong tin phu
	int8 S_backup[2];	//sector chua ban luu cua boot sector
	int8 raw[490];
};
struct FAT_table {
	vector<int8[4]> Fat;
};
struct Data {
	vector<int8[512]> sec;
};
struct Item {
	vector<int8> name;
	uint16_t start_cluster;	//cluster bat dau cua file
	uint16_t n_cluster;
	int8 file;	//0: folder, 1:file
	vector<int8> password;
};
struct Volume {
	BootSector BS;
	FAT_table FT;
	Data D;
	vector<Item> I;
};

//khoi tao vol
void initVol(Volume vol);
//doc vol
void readFile(string filename, Volume vol);
//ghi vol
void writeFile(string filename, Volume vol);
//ham chuyen doi tu cluster sang sector (ko tinh phan sector trong vung boot + fat)
uint32_t ClusterToSector(int cluster, BootSector BS);
//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol);
//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
void exportItem(string filename, vector<Item> IT, Data D);
//copy 1 file tu ngoai vao vol
void importItem(string filename, vector<Item> IT, Volume vol);
//xoa 1 file hoac 1 folder
void deleteItem(string filename, vector<Item> IT, Volume vol);
//tao thong so phu hop cho cac bien cua boot sector
void createInfor(Volume vol);
//ham convert tu LE sang so TN hop ly
int convert(int8* val);
//ham tao password, hoi ng dung pass
void createPass(string filename, Volume vol);

/*co the se thiet ke them bien luu lai vi tri cluster bat dau 
cua vung trong trong phan du lieu, de khoi phai duyet lai,
bien luu vi tri trong trong RDET + FAT nua.
*** hien tai chua co, do anh chua biet nen them o dau.
*/