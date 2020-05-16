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
void writeFile(string filename, Volume &vol)
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
		//vol.FT.Fat[2] = 268435455;
		for (int i = 0; i < vol.FT.Fat.size(); i++)
			fout.write((char*)&vol.FT.Fat[i], sizeof(vol.FT.Fat[i]));
		//tao rdet cua vol
		//int pos = 0;
		//for (int i = 0; i < filename.length(); i++)
		//	vol.D.sec[0].temp[pos++] = filename[i];
		//pos = 61;
		//for (int i = 0; i < filename.length(); i++)
		//	vol.D.sec[0].temp[pos++] = filename[i];
		//pos = 111;
		//vol.D.sec[0].temp[pos++] = 0;	//loai item, 0: folder, 1: file
		//vol.D.sec[0].temp[pos++] = 2;	//cluster bat dau cua folder nay
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
void importItem(string filename, Volume& vol)
{
	string r_fol;
	printList(vol.I);
	cout << "Input folder you wanna copy file into: ";
	getline(cin, r_fol);
	int atp = 1, sDET = 0;
	for (int i = 0; i < vol.I.size(); i++)
	{
		if (r_fol == vol.I[i].name)
		{
			sDET = i;
		}
		if (vol.I[i].name == filename)
		{
			int pos = filename.find('.');
			string ext = filename.substr(pos);
			filename.erase(filename.begin() + pos, filename.end());
			filename += "_" + to_string(atp) + ext;
			atp++;
		}
	}

	Item new_File;
	new_File.name = filename;
	new_File.folder = r_fol;
	new_File.file = 1;

	vector<Sector> tmp;	//luu noi dung file dc copy vao vol
	ifstream fin(filename, ios::binary);
	if (!fin.is_open())
	{
		cout << "Can not open file.";
		return;
	}
	else
	{
		fin.seekg(0, ios::end);
		int32 fsize = fin.tellg();
		fin.seekg(0, ios::beg);
		new_File.n_cluster = ceilf(ceilf(fsize / vol.BS.BperSector) / vol.BS.SperCluster);
		do
		{
			Sector t = { 0 };
			fin.read((char*)&t, sizeof(Sector));
			tmp.push_back(t);
		} while (!fin.eof());
		fin.close();
	}
	int pos = 2;
	for (; pos < vol.FT.Fat.size() - new_File.n_cluster; pos++)
	{
		bool check = true;	//kt co cho trong trong fat vua du kich thuoc file ko
		if (vol.FT.Fat[pos] == 0)
		{
			for (int i = pos + 1; i < new_File.n_cluster - 1; i++)
			{
				if (vol.FT.Fat[i] != 0)
					check = false;
			}
			if (check)
				break;
		}
	}
	//ghi FAT
	bool type;
	int* fat_arr = new int[new_File.n_cluster];
	if (pos < vol.FT.Fat.size() - new_File.n_cluster)	//TH khong bi phan manh
	{
		type = false;
		new_File.start_cluster = pos;
		for (int i = 0; i < new_File.n_cluster - 1; i++)
		{
			vol.FT.Fat[pos] = pos + 1;
			pos++;
		}
		vol.FT.Fat[pos] = 268435455;	//0FFF FFFFh 
		vol.BS.FAT_empty = pos + 1;
	}
	else	//TH bi phan manh
	{
		pos = 2;
		int c = 0;
		type = true;
		int count = new_File.n_cluster, prev_pos;
		while (count > 0)
		{
			if (vol.FT.Fat[pos] == 0)
			{
				if (count == new_File.n_cluster)
					prev_pos = pos;
				else if (count != 1)
					vol.FT.Fat[prev_pos] = pos;
				else
					vol.FT.Fat[pos] = 268435455;	//0FFF FFFFh 
				fat_arr[c++] = pos;
				count--;
			}
			pos++;
		}
	}
	// ghi sdet
	int empty_pos = 0, sector_pos = (vol.I[sDET].start_cluster - 2) * vol.BS.SperCluster;
	for (; sector_pos < (vol.I[sDET].start_cluster - 2) * vol.BS.SperCluster + vol.BS.SperCluster; sector_pos++)
	{
		for (empty_pos = 0; empty_pos < 512; empty_pos += 128)
		{
			bool check = false;
			for (int k = empty_pos; k < empty_pos + 128; k++)
			{
				if (vol.D.sec[sector_pos].s[k] != 0)
					check = true;
			}
			if (check)
				continue;
			goto Import_SDET;
		}
	}
Import_SDET:
	import_SDET(vol, empty_pos, sector_pos, new_File);
	int32 data_pos = (new_File.start_cluster - 2) * vol.BS.SperCluster;
	//ghi noi dung file
	if (!type)
	{
		for (int i = 0; i < tmp.size(); i++)
			vol.D.sec[data_pos++] = tmp[i];
	}
	else
	{
		int j = 0;
		for (int i = 0; i < new_File.n_cluster; i++)
		{
			int count = 0;
			int32 data_pos = (fat_arr[i] - 2) * vol.BS.SperCluster;
			for (; j < tmp.size(); j++)
			{
				vol.D.sec[data_pos++] = tmp[i];
				count++;
				if (count == vol.BS.SperCluster)
					break;
			}
		}
	}
	delete[]fat_arr;

	//goi ham update lai vector<Item>
}
//them vao bang SDET
void import_SDET(Volume& vol, int empty_pos, int sector_pos, Item new_File)
{
	int origin_pos = empty_pos;
	for (int i = 0; i < new_File.name.length(); i++)
		vol.D.sec[sector_pos].s[empty_pos++] = new_File.name[i];
	empty_pos = origin_pos + 61;		//kich thuoc toi da cua ten file la 61 (tinh ca phan duoi mo rong)
	for (int i = 0; i < new_File.folder.length(); i++)
		vol.D.sec[sector_pos].s[empty_pos++] = new_File.folder[i];
	empty_pos = origin_pos + 111;	//kich thuoc toi da cua ten folder la 50
	vol.D.sec[sector_pos].s[empty_pos++] = new_File.file;

	unsigned char* t = convert32_to_8(new_File.start_cluster);
	unsigned char a = t[0], b = t[1], c = t[2], d = t[3];
	vol.D.sec[sector_pos].s[empty_pos++] = a;
	vol.D.sec[sector_pos].s[empty_pos++] = b;
	vol.D.sec[sector_pos].s[empty_pos++] = c;
	vol.D.sec[sector_pos].s[empty_pos++] = d;

	t = convert32_to_8(new_File.n_cluster);
	a = t[0]; b = t[1]; c = t[2]; d = t[3];
	vol.D.sec[sector_pos].s[empty_pos++] = a;
	vol.D.sec[sector_pos].s[empty_pos++] = b;
	vol.D.sec[sector_pos].s[empty_pos++] = c;
	vol.D.sec[sector_pos].s[empty_pos++] = d;

	for (int i = 0; i < new_File.password.length(); i++)
		vol.D.sec[sector_pos].s[empty_pos++] = new_File.password[i];
}
//in ra danh sach cac file
void printList(vector<Item>I)
{
	for (int i = 0; i < I.size(); i++)
	{
		cout << "- " << I[i].folder << " " << I[i].name << endl;
	}
}
//convert int32, int16 thanh int8
unsigned char* convert32_to_8(int32 a) {
	unsigned char* c = (unsigned char*)(&a);
	(c[3] << 24 | c[2] << 16 | c[1] << 8 | c[0] << 0);
	return c;
}
unsigned char* convert16_to_8(int16 a) {
	unsigned char* c = (unsigned char*)(&a);
	(c[1] << 8 | c[0] << 0);
	return c;
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