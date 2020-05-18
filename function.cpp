#include "Header.h"

//khoi tao vol
void initVol(Volume &vol) //default: 2GB
{
	vol.BS.BperSector = 512;
	vol.BS.FAT_empty = 2;
	vol.BS.FAT_size = 16;
	vol.BS.BootSector = int16(8);
	vol.BS.numFAT = 1;
	vol.BS.RDET_cluster = 2;
	vol.BS.SperCluster = 32;
	vol.BS.vol_size = 65536;
	vol.FT.Fat.push_back(268435448);
	vol.FT.Fat.push_back(4294967295);
	for (int i = 2; i < 2048; i++)
		vol.FT.Fat.push_back(0);
	Sector temp = { 0 };
	for (int i = 0; i < 65512; i++)
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

		for (int i = 0; i < vol.BS.numFAT * vol.BS.FAT_size*vol.BS.BperSector/4; i++)
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
		vol.FT.Fat[2] = 268435455;
		for (int i = 0; i < vol.FT.Fat.size(); i++)
			fout.write((char*)&vol.FT.Fat[i], sizeof(vol.FT.Fat[i]));
		/*tao rdet cua vol*/
		int pos = 0;
		for (int i = 0; i < filename.length(); i++)
			vol.D.sec[0].s[pos++] = filename[i];
		pos = 61;
		for (int i = 0; i < filename.length(); i++)
			vol.D.sec[0].s[pos++] = filename[i];
		pos = 111;
		vol.D.sec[0].s[pos++] = 0;	//loai item, 0: folder, 1: file
		vol.D.sec[0].s[pos++] = 2;	//cluster bat dau cua folder nay
		
		for (int i = 0; i < vol.D.sec.size(); i++)
			fout.write((char*)&vol.D.sec[i], sizeof(vol.D.sec[i]));
		fout.close();
	}
}

//ham chuyen doi tu cluster sang sector (ko tinh phan sector trong vung boot + fat)
int32 ClusterToSector(int cluster, BootSector BS)
{
	//dùng công thức liên hệ giữa sector thứ i và cluster thứ k
	int32 sector = 0;
	//cong thuc tinh cluster thu k tuong duong vs sector thu i la
	sector = BS.BootSector + BS.numFAT * BS.FAT_size + (cluster - BS.RDET_cluster) * BS.SperCluster;
	return sector;
}
Item readItem(Volume vol, Sector aSector,int currentByte) {
		Item item;
		int countByte = currentByte;
		for (int i = countByte; i < countByte+61; i++)
			item.name += aSector.s[i];
		countByte += 61;
		int pos = item.name.find('\0');
		item.name = item.name.substr(0, pos);
		for (int i = countByte; i < countByte + 50; i++)
			item.folder += aSector.s[i];
		pos = item.folder.find('\0');
		item.folder = item.folder.substr(0, pos);
		countByte += 50;
		item.file = aSector.s[countByte++];
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
		for (int i = countByte; i < countByte + 8; i++)
			item.password +=aSector.s[i];
		pos = item.password.find('\0');
		item.password = item.password.substr(0, pos);
		countByte += 8;
		return item;
}
//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol)
{
	vector<Item> it = vol.I;
	int countSec = 0;
	int RDET_pos = 0;
	Sector aSector = vol.D.sec[RDET_pos];
	//read RDET
	int push = 0;
	int countByte = 0;
	do {
		Item item = readItem(vol, aSector, countByte);
		if (item.file != 0 && item.file != 1)
			break;
		if (item.start_cluster == 0 && item.n_cluster == 0)
			break;
		it.push_back(item);
		push++;
		countByte += 128;
		if (countByte == 512)
		{
			aSector = vol.D.sec[++RDET_pos];
			countByte = 0;
		}
	} while (1);

	//read SDET
	int pop = 0;
	do {
		Item item = it[pop];
		pop++;
		int countByte = 0;
		if(item.file==0)
		do {
			int childFolder_sector = item.start_cluster*vol.BS.SperCluster;
			Sector aSector = vol.D.sec[childFolder_sector];
			item = readItem(vol,aSector,countByte);
			if (item.file != 0 && item.file != 1)
				break;
			if (item.start_cluster == 0 && item.n_cluster == 0)
				break;
			it.push_back(item);
			push++;
			countByte += 128;
			if (countByte == 512)
			{
				aSector = vol.D.sec[++RDET_pos];
				countByte = 0;
			}
		} while (1);
	} while (pop<push);
	return it;
}

Item FindFile(string name,Volume vol) {
	for (int i = 0; i < sizeof(vol.I); i++)
	{
		if (name == vol.I[i].name)
			return vol.I[i];
	}
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
		int32 current_cluster= item.start_cluster;
		do {
			for (int i = 0; i < vol.BS.SperCluster; i++) {
				buffer = vol.D.sec[(current_cluster*vol.BS.SperCluster) + i];
				fout.write((char *)&buffer, sizeof(buffer));
			}
			current_cluster = vol.FT.Fat[current_cluster];
		} while (current_cluster!= 268435455);
	}
	fout.close();
}
//copy 1 file tu ngoai vao vol
//void importItem(string filename, Volume& vol)
////{
//	string name=filename;
//	for (int i = filename.length(); i >= 0; i--) {
//		if (filename[i] == '\\')
//		{
//			name.erase(0,i+1);
//			break;
//		}
//	}
//	ifstream fin(filename, ios::binary);
//	if (!fin.is_open())
//	{
//		cout << "Can not open volume.";
//		return;
//	}
//	else
//	{
//		fin.seekg(0, fin.end);
//		int length = fin.tellg();
//		fin.seekg(0, fin.beg);
//		int clusterNumber = (length / (vol.BS.BperSector*vol.BS.SperCluster));
//		if (length - (clusterNumber*vol.BS.BperSector*vol.BS.SperCluster) > 0)
//			clusterNumber++;
//		Item temp;
//		temp.name = name;
//		temp.folder = name;
//		temp.start_cluster = 2;
//		temp.n_cluster = clusterNumber;
//		temp.file = 1;
//		vol.I.push_back(temp);
//		//can kiem tra du byte trong
//		int first_cluster = vol.I[0].start_cluster;
//		for (int i = 0; i < clusterNumber - 1; i++)
//		{
//			vol.FT.Fat[first_cluster] = first_cluster+1;
//			first_cluster++;
//		}
//		vol.FT.Fat[first_cluster] = 268435455;
//		int sectorNumber = vol.BS.SperCluster*temp.start_cluster;
//		for (int i = 0; i < clusterNumber*vol.BS.SperCluster; i++)
//		{
//			Sector buffer = { 0 };
//			fin.read((char*)&buffer, sizeof(buffer));
//			vol.D.sec[sectorNumber] = buffer;
//			sectorNumber++;
//		}
//	}
//	fin.close();
//}
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
//xoa 1 file hoac 1 folder
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
	int16 temp = x*256+y;
	return temp;
}

int32 convert8_to_32(unsigned char *a) {
	unsigned char x = a[3], y = a[2],z=a[1],t=a[0];
	int32 temp = x * 256*256*256 + y * 256 * 256+z*256+t;
	return temp;
}

//chua chay
void deleteItem(string filename, Volume &vol,string volName)
{
	int32 rdet_sector = vol.BS.BootSector + vol.BS.numFAT * vol.BS.FAT_size;
	int32 start_sector;
	Item item = FindFile(filename, vol);
	if (item.folder == volName)
	{
		start_sector = rdet_sector;
	}
	else {
		Item Folder = FindFile(item.folder, vol);
		start_sector = (Folder.start_cluster-2)*vol.BS.SperCluster + rdet_sector;
	}
	//doc de tim vi tri file trong rdet/sdet
	int32 fileRdet;
	ifstream fin(volName, ios::binary);
	if (!fin.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		fin.seekg(start_sector*512, ios::beg);
		Item buffer;
		do {
			int32 current_pos = fin.tellg();
			fin.read((char *)&buffer,sizeof(buffer));
			int pos = buffer.name.find('\0');
			buffer.name = buffer.name.substr(0, pos);
			if (buffer.name == filename)
			{
				fileRdet = current_pos;
				break;
			}
		} while (1);
	}
	//sua bang fat va doi E5 trong entry
	ofstream fout(volName, ios::binary|ios::app);
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
	// các thông số hợp lý cho FAT32
	int size;
	int option;
	string typeSize;
	long long bytes = 0; //Maximum value for a variable of type long long	9223372036854775807


	//input so bytes cho volume mong muon (1)
	cout << "What kind of type of volume'size you want to create volume: " << endl;
	cout << "1.MB" << endl;
	cout << "2.GB" << endl;
	cout << "0.bytes (default) " << endl;
	cin >> option;
	cout << "How many for size of the volume?: ";
	cin >> size;
	switch (option) {
	case 1: { //trong TH nguoi dung nhap MB --> bytes
		bytes = size * pow(1024, 2);
		cout << "The volume has " << size << "MB ~ " << bytes << " bytes" << endl;
		break;
	}
	case 2: {//trong TH nguoi dung nhap GB --> bytes
		bytes = size * pow(1024, 3);
		cout << "The volume has " << size << "GB ~ " << bytes << " bytes" << endl;
		break;
	}
	default: //mac dinh la bytes
		bytes = size;
		cout << "The volume has " << bytes << " bytes" << endl;
	}

	//input so bytes per sector mong muon (2) =-> KHONG CAN HAM NAY, default = 512
	/*int16 BperSector = 0;
	do {
		cout << "How many number of sectors do you want for per sector (512,1024,....): ";
		cin >> BperSector;
		if (BperSector % 2 != 0 || BperSector < 512) cout << "Please enter again" << endl;
	} while (BperSector % 2 != 0 || BperSector < 512);*/

	//input so sector mong muon cho vung bootsector (neu k phai la boi so cua 2 thi yeu cau nhap lai) (3)
	int16 SectorofBootSector = 0;
	do {
		cout << "How many sectors you want for Boot Sector:  ";
		cin >> SectorofBootSector;
		if (SectorofBootSector % 2 != 0 || SectorofBootSector < 2) cout << "Please enter again" << endl;
	} while (SectorofBootSector % 2 != 0 || SectorofBootSector < 2);

	//input số sector per cluster (neu < 1 thi yeu cau nhap lai) (4)
	uint16_t sectorPerCluster = 0;
	do {
		cout << "How many sectors you want for per cluster:  ";
		cin >> sectorPerCluster;
		if (sectorPerCluster < 1) cout << "Please enter again" << endl;
	} while (sectorPerCluster < 1);


	/*
	Tu (1),(2),(3), (4) ==> cac thong so cho boot sector
	*/
	vol.BS.BperSector = 512;
	vol.BS.BootSector = SectorofBootSector;
	vol.BS.vol_size = bytes / vol.BS.BperSector;
	vol.BS.SperCluster = sectorPerCluster;
	cout << "Size of volume (sector) is " << vol.BS.vol_size << endl;
	//==> từ các thông số trên ta phải suy ra được số sector cho FAT = SF
	//FAT32 ==> kích thước mỗi phần tử trong bảng FAT là 4 bytes 
	//công thức tính SF:
	double tmp_FATsize = 0.0;
	tmp_FATsize = (4 * vol.BS.vol_size - 4 * vol.BS.BootSector + 8 * vol.BS.SperCluster) / (vol.BS.BperSector * vol.BS.SperCluster + 4 * vol.BS.numFAT);
	vol.BS.FAT_size = ceilf(tmp_FATsize); //hàm làm tròn lên
}
//ham tao password, hoi ng dung pass
void createPass(string filename, Volume& vol)
{
	// muốn đặt password cho file nào?
	//khi nhập tên file --> dò xem file có trong vector<Item> I hay khong tu danh sach cac item co tu ham createList ==> neu co thi moi cho tao pass ==> khong thi out

	vol.I = createList(vol);
	//vector<Item> I cua volume trong TH nay se chua danh sach cac item duoc tao tu ham createList

	int len = vol.I.size();
	for (int i = 0; i < len; i++)
	{
		if (filename == vol.I[i].name) //do xem trong vector<Item>I co I[i].name nao == filename hay khong ? chuyen qua phan nhap pass : bao "khong co" & out
		{
			int option;
			do {
				cout << "Do you want to set password for " << filename << " Yes: 1. No: 0 ";
				cin >> option;
				if (option == 1) {
					//password ít nhất 6 ký tự
					string password;
					do {
						cout << "Enter your password (at least 6 characters): ";
						cin >> password;
						if (password.length() < 6) {
							cout << "Please enter password again!" << endl;
						}
					} while (password.length() < 6);
					//cout << toHex(taoPass(password)) << endl;
					//them password vao trong cai item I co chua filename do
					vol.I[i].password = toHex(taoPass(password));
					cout << "Have set password " << password << " for " << filename << endl;
				}
				else if (option == 0) {
					cout << "Not set password" << endl;
					return;
				}
				else if (option != 0 && option != 1) {
					cout << "Please enter again" << endl;
				}
			} while (option != 0 && option != 1);
		}
		else {
			cout << "Can't find " << filename << "in the list of items" << endl;
			return;
		}
	}
}
string toHex(unsigned int input) { //hàm convert thành hex từ biến hash unsigned int
	//hàm này có sử dụng class stringstream để ghép nối chuỗi thật nhanh chóng & tiện lợi, thao tác nhanh hơn vs string --> có sử dụng #include <sstream>
	string hexhash;
	stringstream hexstream;
	hexstream << hex << input; //Biến số input thành 1 chuỗi số và ghép vào sau chuỗi đang có //ví dụ nếu input = 17 thì nó sẽ là "17"
	hexhash = hexstream.str(); //lấy content của hextstream
	std::transform(hexhash.begin(), hexhash.end(), hexhash.begin(), ::toupper); //toUpper chuỗi hexhash
	return hexhash; //đây chính là password sau khi được mã hóa
}
unsigned int taoPass(string pass)
{
	unsigned int init = 123321456654789987;
	//unsigned int magic = 7891234;
	int magic = rand() % 100;
	int magic2 = rand();
	unsigned int hash = 0;
	for (int i = 0; i < pass.length(); i++) {
		hash = hash << i; //dùng phép dời bit phải để ra một hash mới
		hash = hash ^ (pass[i]); // dùng phép xor để làm thay đổi các giá trị bit
		hash = hash * magic + magic2;
	}
	return hash;
}