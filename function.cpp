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
Item readItem(Volume vol, Sector aSector, int currentByte) {
	Item item;
	int countByte = currentByte;
	for (int i = 0; i < 61; i++)
		item.name[i] = (char)aSector.s[i];
	countByte += 61;
	for (int i = countByte; i < countByte + 50; i++)
		item.folder[i] = (char)aSector.s[i];
	countByte += 50;
	unsigned char a[4];
	a[0] = aSector.s[countByte++];
	a[1] = aSector.s[countByte++];
	a[2] = aSector.s[countByte++];
	a[3] = aSector.s[countByte++];
	item.start_cluster = convert8_to_32(a);
	a[0] = aSector.s[countByte++];
	a[1] = aSector.s[countByte++];
	a[2] = aSector.s[countByte++];
	a[3] = aSector.s[countByte++];
	item.n_cluster = convert8_to_32(a);
	item.file = aSector.s[countByte++];
	for (int i = countByte; i < countByte + 8; i++)
		item.password[i] = (char)aSector.s[i];
	countByte += 8;
}
//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol)
{
	vector<Item> it = vol.I;
	int countSec = 0;
	int RDET_pos = vol.BS.RDET_cluster*vol.BS.SperCluster + vol.BS.BootSector + vol.BS.numFAT * vol.BS.FAT_size;
	Sector aSector = vol.D.sec[RDET_pos];
	//read RDET
	int push = 0;
	do {
		int countByte = 0;
		Item item = readItem(vol, aSector, countByte);
		if (item.file != 0 && item.file != 1)
			break;
		if (item.start_cluster == 0 && item.n_cluster == 0)
			break;
		it.push_back(item);
		push++;
		countByte += 128;
		if (countByte == 512)
			aSector = vol.D.sec[++RDET_pos];
	} while (1);

	//read SDET
	int pop = 0;
	do {
		Item item = it[pop];
		pop++;
		if (item.file == 0)
			do {
				int countByte = 0;
				int childFolder_sector = item.start_cluster*vol.BS.SperCluster;
				Sector aSector = vol.D.sec[childFolder_sector];
				item = readItem(vol, aSector, countByte);
				if (item.file != 0 && item.file != 1)
					break;
				if (item.start_cluster == 0 && item.n_cluster == 0)
					break;
				it.push_back(item);
				push++;
				countByte += 128;
				if (countByte == 512)
					aSector = vol.D.sec[++RDET_pos];
			} while (1);
	} while (pop < push);
	return it;
}

Item FindFile(string name, Volume vol) {
	for (int i = 0; i < sizeof(vol.I); i++)
		if (vol.I[i].name == name)
			return vol.I[i];
	cout << "Cannot find " << name << " file" << endl;
	return { 0 };
}

//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
void exportItem(string filename, Volume vol)
{
	//chưa hoi pass
	//fat va rdet can xoa
	string filePath;
	cout << "Example path is H:\\NewFolder\\" << endl;
	cout << "Enter path to export: ";
	getline(cin, filePath);
	filePath += filename;
	Item item = FindFile(filename, vol);
	Sector buffer;
	ofstream fout(filePath, ios::binary);
	if (!fout.is_open())
	{
		cout << "Can not create File.";
		return;
	}
	else {
		//đọc và kiểm tra phân mảnh
		int32 current_cluster = item.start_cluster;
		do {
			for (int i = 0; i < vol.BS.SperCluster; i++) {
				buffer = vol.D.sec[(current_cluster*vol.BS.SperCluster) + i];
				fout.write((char *)&buffer, sizeof(buffer));
			}
			current_cluster = vol.FT.Fat[current_cluster];
		} while (current_cluster != 268435455);
	}
	fout.close();
}
//copy 1 file tu ngoai vao vol
void importItem(string filename, Volume &vol)
{

}
unsigned char* convert32_to_8(int32 a) {
	unsigned char* c = (unsigned char *)(&a);
	(c[3] << 24 | c[2] << 16 | c[1] << 8 | c[0] << 0);
	//cout << (int)c[0] << " " << (int)c[1] << " " << (int)c[2] << " " << (int)c[3];
	return c;
}
unsigned char* convert16_to_8(int16 a) {
	unsigned char* c = (unsigned char *)(&a);
	(c[1] << 8 | c[0] << 0);
	return c;
}

int16 convert8_to_16(unsigned char *a) {
	unsigned char x = a[1], y = a[0];
	int16 temp = x * 256 + y;
	return temp;
}

int32 convert8_to_32(unsigned char *a) {
	unsigned char x = a[3], y = a[2], z = a[1], t = a[0];
	int32 temp = x * 256 * 256 * 256 + y * 256 * 256 + z * 256 + t;
	return temp;
}
void deleteItem(string filename, Volume &vol, string volName)
{
	for (int i = volName.length(); i >= 0; i--) {
		if (volName[i] == '\\')
		{
			volName.erase(0, i + 1);
			break;
		}
	}
	int32 rdet_sector = vol.BS.BootSector + vol.BS.numFAT * vol.BS.FAT_size;
	int32 start_sector;
	Item item = FindFile(filename, vol);
	if (item.folder == volName)
	{
		start_sector = rdet_sector;
	}
	else {
		Item Folder = FindFile(item.folder, vol);
		start_sector = Folder.start_cluster*vol.BS.SperCluster + rdet_sector;
	}
	//doc de tim vi tri file trong rdet/sdet
	int32 fileRdet;
	ifstream fin(filename, ios::binary);
	if (!fin.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		fin.seekg(start_sector, ios::beg);
		Item buffer;
		do {
			int32 current_pos = fin.tellg();
			fin.read((char *)&buffer, sizeof(buffer));
			if (buffer.name == filename)
			{
				fileRdet = current_pos;
				break;
			}
		} while (1);
	}
	//sua bang fat va doi E5 trong entry
	ofstream fout(volName, ios::binary);
	if (!fout.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		int start_FAT = 512 * (vol.BS.BootSector);
		int start_Position = start_FAT + item.start_cluster * 4;
		fout.seekp(start_Position, ios::beg);
		for (int i = 0; i < item.n_cluster; i++)
			fout.write((char*)&vol.FT.Fat[start_Position + i], sizeof(vol.FT.Fat[i]));

		int8 firstByte = 229;
		item.name[0] = (char)firstByte;
		fout.seekp(fileRdet, ios::beg);
		fout.write((char *)&item, sizeof(item));
	}

	fout.close();
}
//tao thong so phu hop cho cac bien cua boot sector
void createInfor(Volume &vol)
{

}
//ham tao password, hoi ng dung pass
void createPass(string filename, Volume &vol)
{

}