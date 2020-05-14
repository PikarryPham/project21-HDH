#include "Header.h"

//khoi tao vol
void initVol(Volume &vol) //default: 2GB
{
	vol.BS.BperSector = 512;
	vol.BS.FAT_empty = 2;
	vol.BS.FAT_size = 1024;
	vol.BS.BootSector = int16(8);
	vol.BS.numFAT = 1;
	vol.BS.RDET_cluster = 2;
	vol.BS.SperCluster = 32;
	vol.BS.vol_size = 4194304;
	vol.FT.Fat.push_back(268435448);
	vol.FT.Fat.push_back(4294967295);
	for (int i = 2; i < 131072; i++)
		vol.FT.Fat.push_back(0);
	Sector temp = { 0 };
	for (int i = 0; i < 4193272; i++)
		vol.D.sec.push_back(temp);
}
//doc vol
void readFile(string filename, Volume &vol)
{
	ifstream fin(filename, ios::binary);
	if (!fin.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		fin.read((char*)&vol.BS.BperSector, sizeof(vol.BS.BperSector));
		fin.read((char*)&vol.BS.SperCluster, sizeof(vol.BS.SperCluster));
		fin.read((char*)&vol.BS.BootSector, sizeof(vol.BS.BootSector));
		fin.read((char*)&vol.BS.numFAT, sizeof(vol.BS.numFAT));
		fin.read((char*)&vol.BS.vol_size, sizeof(vol.BS.vol_size));
		fin.read((char*)&vol.BS.FAT_size, sizeof(vol.BS.FAT_size));
		fin.read((char*)&vol.BS.RDET_cluster, sizeof(vol.BS.RDET_cluster));
		fin.read((char*)&vol.BS.FAT_empty, sizeof(vol.BS.FAT_empty));
		fin.read((char*)&vol.BS.raw, sizeof(vol.BS.raw));
		int start_FAT = 512 * (vol.BS.BootSector - 1);
		fin.seekg(start_FAT, ios::cur);
		for (int i = 0; i < vol.BS.numFAT * vol.BS.FAT_size; i++)
		{
			int32 temp;
			fin.read((char*)&temp, sizeof(temp));
			vol.FT.Fat.push_back(temp);
		}
		int32 data_sec = vol.BS.vol_size - vol.BS.BootSector - vol.BS.numFAT * vol.BS.FAT_size;
		for (int i = 0; i < data_sec; i++)
		{
			Sector temp;
			fin.read((char*)&temp, sizeof(temp));
			vol.D.sec.push_back(temp);
		}
		fin.close();
	}
}
//ghi vol
void writeFile(string filename, Volume vol)
{
	ofstream fout(filename, ios::binary);
	if (!fout.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		fout.write((char*)&vol.BS.BperSector, sizeof(vol.BS.BperSector));
		fout.write((char*)&vol.BS.SperCluster, sizeof(vol.BS.SperCluster));
		fout.write((char*)&vol.BS.BootSector, sizeof(vol.BS.BootSector));
		fout.write((char*)&vol.BS.numFAT, sizeof(vol.BS.numFAT));
		fout.write((char*)&vol.BS.vol_size, sizeof(vol.BS.vol_size));
		fout.write((char*)&vol.BS.FAT_size, sizeof(vol.BS.FAT_size));
		fout.write((char*)&vol.BS.RDET_cluster, sizeof(vol.BS.RDET_cluster));
		fout.write((char*)&vol.BS.FAT_empty, sizeof(vol.BS.FAT_empty));
		fout.write((char*)&vol.BS.raw, sizeof(vol.BS.raw));
		int8 temp[512] = { 0 };
		for (int i = 0; i < vol.BS.BootSector - 1; i++)
			fout.write((char*)&temp, sizeof(temp));
		for (int i = 0; i < vol.FT.Fat.size(); i++)
			fout.write((char*)&vol.FT.Fat[i], sizeof(vol.FT.Fat[i]));
		for (int i = 0; i < vol.D.sec.size(); i++)
			fout.write((char*)&vol.D.sec[i], sizeof(vol.D.sec[i]));
		fout.close();
	}
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