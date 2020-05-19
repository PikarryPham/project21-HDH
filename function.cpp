#include "Header.h"
/*
********CAC HAM CHO GIAO DIEN CONSOLE****
*/
void gotoXY(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void textColor(WORD color) {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0; wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void fixConsoleWindow() // khoa man hinh console
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void stopScrolling()
{
	HWND scroll = GetConsoleWindow();
	SetScrollRange(scroll, SB_VERT, 1, 29, true);
}
void drawBorder() {
	textColor(3);
	//Ve border tren & duoi
	for (int i = 1; i < 120; i++)
	{
		gotoXY(i, 0);
		std::cout << char(205);
		gotoXY(i, 29);
		std::cout << char(205);
	}
	//Ghi title cua do an
	gotoXY(44, 0);
	std::cout << "  OPERATING SYSTEMS - 18CLC1  ";

}
void authors() {
	gotoXY(56, 11);
	std::cout << "AUTHORS";
	gotoXY(45, 14);
	std::cout << "PHAM NGOC THUY TRANG - 18127022";
	gotoXY(45, 16);
	std::cout << "LAM NGOC PHUONG ANH  - 18127039";
	gotoXY(45, 18);
	std::cout << "NGUYEN THI ANH DAO	  - 18127272";
}
void setCursor(bool visible, DWORD size)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (size == 0)
	{
		size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}
int inputKey()
{
	if (_kbhit())
	{
		int key = _getch();
		if (key == 224)	// special key
		{
			key = _getch();
			return key + 1000;
		}
		if (key == 13) //Enter key
		{
			return key + 1000;
		}
		return key;
	}
	else return -1;
	return -1;
}
void mainMenu(Volume vol)
{

	for (int i = 1; i < 120; i++)
	{
		gotoXY(i, 0);
		std::cout << char(205);
		gotoXY(i, 29);
		std::cout << char(205);
	}
	//Ghi title
	gotoXY(50, 0);
	std::cout << " MAIN MENU  ";
	string s = "";
	int optionMenu;
	while (true)
	{
		setCursor(0, 0); //Hide cursor de man hinh dep hon 
		s = "                             ";
		gotoXY(25, 8);
		std::cout << "==================== Please choose your option ===================\n";
		//co 2 lua chon: mo file co san trong txt, hoac tao file moi
		textColor(7);
		gotoXY(25, 11);
		std::cout << "                     1. Open existed volume \n";
		gotoXY(25, 14);
		std::cout << "                     2. Create new volume \n";
		do {
			gotoXY(15, 20);
			std::cout << "--> Enter your option: ";
			cin >> optionMenu;
			gotoXY(15, 22);
			std::cout << "--> You choose option: " << optionMenu << endl;
			if (optionMenu != 1 && optionMenu != 2) std::cout << "Please enter again." << endl;
			//else if (optionMenu == 1 || optionMenu == 2)
		} while (optionMenu != 1 && optionMenu != 2);
		Sleep(600);
		break;
	}
	if (optionMenu == 1) {
		system("cls");
		setCursor(1, 10);
		//initVol(vol);
		std::cout << "---> What's volume name of the list you want to work with? <--- " << endl;
		string nme, tmp;
		cin >> nme;
		vector<string> str;
		str = listNameOfVolume("file.txt");
		//dò xem trong vector<string> có ch?a name các volume ?ó có tên volume nãy mình v?a nh?p hay không, n?u không thì b?t nh?p l?i ho?c out
		for (int i = 0; i < str.size(); i++) {
			tmp = str[i];
			if (nme == str[i]) {
				readFile(nme, vol);
				listMenu(vol);
				break;
			}
		}
		if (nme != tmp) {
			cout << "Volume's name you want to work doesn't exist" << endl;
			Sleep(1200);
			system("cls");
			textColor(6);
			nameVolMenu(vol);
		}
	}
	else if (optionMenu == 2) {
		system("cls");
		setCursor(1, 10);
		//initVol(vol);
		std::cout << "---> Please create new volume by enter value <--- " << endl;
		createInfor(vol);
		listMenu(vol);
	}
}

void smallMenu(Volume vol) {
	drawBorder();
	authors();
	string s = "";
	while (true)
	{
		setCursor(0, 0); //Hide cursor de man hinh dep hon 
		s = "                             ";
		gotoXY(45, 24);
		std::cout << s;
		s = "PRESS ENTER TO GO TO THE MENU";
		Sleep(200);
		gotoXY(45, 24);
		textColor(9);
		std::cout << s;
		Sleep(600);

		int a = inputKey();
		if (a == 1013) //1013 == enter
		{
			textColor(6); //set mau gold
			break;
		}
	}
	system("cls");
	nameVolMenu(vol);
}

void nameVolMenu(Volume vol) {
	for (int i = 1; i < 120; i++)
	{
		gotoXY(i, 0);
		std::cout << char(205);
		gotoXY(i, 29);
		std::cout << char(205);
	}
	gotoXY(50, 0);
	std::cout << " LIST EXISTED VOLUMES  ";
	string s = "";
	vector<string> str;
	str = listNameOfVolume("file.txt");
	int k = 10; //vi tri cua toa do y 
	int count = 0; //de dem do dai cua vector, neu count == size cua vector thi dung
	bool flag = true;
	while (flag) {
		for (int i = 0; i < str.size(); i++) {
			gotoXY(52, k);
			cout << i + 1 << " . " << str[i] << endl;
			count++;
			k++;
		}
		if (count == str.size()) flag = false;
	}
	while (true) {
		setCursor(0, 0); //Hide cursor de man hinh dep hon 
		s = "                             ";
		gotoXY(45, 24);
		std::cout << s;
		s = "PRESS ENTER TO GO TO THE MENU";
		Sleep(200);
		gotoXY(45, 24);
		textColor(9);
		std::cout << s;
		Sleep(600);

		int a = inputKey();
		if (a == 1013) //1013 == enter
		{
			textColor(6); //set mau gold
			break;
		}
	}
	system("cls");
	mainMenu(vol);
}

/*
************CAC HAM CHO PHAN YEU CAU************
*/

vector<string> listNameOfVolume(string file) {

	string str;
	vector<string> listName;
	ifstream fin;
	fin.open(file, ios_base::in);

	if (fin.fail()) {
		cout << "File doest exist!" << endl;
		return listName;
	}
	else {
		while (!fin.eof()) { getline(fin, str); fin >> str; listName.push_back(str); }
	}
	fin.close();
	return listName;
}
void initVol(Volume& vol)
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
};
//doc vol
void readFile(string filename, Volume& vol) {
	ifstream fin(filename, ios::binary);
	if (!fin.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		vol.nameVol = filename;
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

		for (int i = 0; i < vol.BS.numFAT * vol.BS.FAT_size * 512 / 4; i++)
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
	vol.I = createList(vol);
}
//ghi vol
void writeFile(string filename, Volume& vol) {
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
int32 ClusterToSector(int cluster, BootSector BS) {
	int32 sector = 0;
	//cong thuc tinh cluster thu k tuong duong vs sector thu i la
	sector = (cluster - BS.RDET_cluster) * BS.SperCluster;
	return sector;
};
//tao danh sach cac file + thu muc co trong vol
Item readItem(Volume vol, Sector aSector, int currentByte) {
	Item item;
	int countByte = currentByte;
	for (int i = countByte; i < countByte + 61; i++)
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
		item.password += aSector.s[i];
	pos = item.password.find('\0');
	item.password = item.password.substr(0, pos);
	countByte += 8;
	return item;
}
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
		if (item.name[0] != (char)229)
		{
			it.push_back(item);
			push++;
		}
		countByte += 128;
		if (countByte == 512)
		{
			aSector = vol.D.sec[++RDET_pos];
			countByte = 0;
		}
	} while (1);

	//read SDET
	int pop = 0;
	while (pop < push) {
		Item item = it[pop];
		pop++;
		int countByte = 0;
		if (item.file == 0)
			do {
				int childFolder_sector = ClusterToSector(item.start_cluster, vol.BS);//item.start_cluster * vol.BS.SperCluster;
				Sector aSector = vol.D.sec[childFolder_sector];
				item = readItem(vol, aSector, countByte);
				if (item.file != 0 && item.file != 1)
					break;
				if (item.start_cluster == 0 && item.n_cluster == 0)
					break;
				if (item.name[0] != (char)229)
				{
					it.push_back(item);
					push++;
				}
				countByte += 128;
				if (countByte == 512)
				{
					aSector = vol.D.sec[++RDET_pos];
					countByte = 0;
				}
			} while (1);
	};
	return it;
}
//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
Item FindFile(string name, Volume vol) {
	int Isize = vol.I.size();
	Item item;
	for (int i = 0; i < Isize; i++)
	{
		if (name == vol.I[i].name)
			return vol.I[i];
	}
	item.name = "cannotfind";
	return item;
}
void deleteItem(string filename, Volume& vol, string volName)
{
	int32 rdet_sector = 0;
	int32 start_sector;
	Item item = FindFile(filename, vol);
	if (item.name == "cannotfind")
	{
		cout << "Already erase/move " << filename << " file" << endl;
	}
	else {
		if (item.folder == volName)
		{
			start_sector = rdet_sector;
		}
		else {
			Item Folder = FindFile(item.folder, vol);
			start_sector = ClusterToSector(Folder.start_cluster, vol.BS);
		}
		//doc de tim vi tri file trong rdet/sdet
		int32 fileRdet = 0;
		int file_D = 0;
		Sector aSector = vol.D.sec[start_sector];
		int countByte = 0;
		do {
			Item item = readItem(vol, aSector, countByte);
			if (item.file != 0 && item.file != 1)
				break;
			if (item.start_cluster == 0 && item.n_cluster == 0)
				break;
			if (item.name == filename)
			{
				file_D = start_sector;
				fileRdet = (start_sector + vol.BS.FAT_size * vol.BS.numFAT + vol.BS.BootSector) * 512 + countByte;
				break;
			}
			countByte += 128;
			if (countByte == 512)
			{
				aSector = vol.D.sec[++start_sector];
				countByte = 0;
			}
		} while (1);
		//sua bang fat va doi E5 trong entry
		ofstream fout(volName, ios::binary | ios::in);
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
			{
				int32 zero = 0;
				fout.write((char*)&zero, sizeof(zero));
				vol.FT.Fat[item.start_cluster + i] = 0;
			}
			int8 firstByte = 229;
			item.name[0] = (char)firstByte;
			fout.seekp(fileRdet, ios::beg);
			vol.D.sec[file_D].s[0] = (char)229;
			for (int i = 0; i < sizeof(vol.I); i++)
			{
				if (filename == vol.I[i].name)
				{
					vol.I.erase(vol.I.begin() + i);
					break;
				}
			}
			fout.write((char*)&item.name[0], sizeof(char));
		}

		fout.close();
	}
}
void exportItem(string filename, Volume vol, string volName)
{
	//ch?a hoi pass
	string filePath;
	cout << "Example path is H:\\NewFolder\\" << endl;
	cout << "Enter path to export: ";
	getline(cin, filePath);
	filePath += filename;
	Item item = FindFile(filename, vol);
	if (item.name == "cannotfind")
	{
		cout << "Cannot find " << filename << " file" << endl;
	}
	else {
		Sector buffer;
		ofstream fout(filePath, ios::binary);
		if (!fout.is_open())
		{
			cout << "Can not create File.";
			return;
		}
		else {
			//??c và ki?m tra phân m?nh
			int32 current_cluster = item.start_cluster;
			do {
				for (int i = 0; i < vol.BS.SperCluster; i++) {
					buffer = vol.D.sec[((current_cluster - 2) * vol.BS.SperCluster) + i];
					fout.write((char*)&buffer, sizeof(buffer));
				}
				current_cluster = vol.FT.Fat[current_cluster];
			} while (current_cluster != 268435455);
		}
		fout.close();
		deleteItem(filename, vol, volName);
	}
}
//copy 1 file tu ngoai vao vol
void importItem(string filename, Volume& vol) {
	string r_fol;
	//Item tp;
	//tp.name = vol.nameVol;
	//tp.folder = vol.nameVol;
	//tp.file = 0;
	//tp.n_cluster = 0;
	//tp.start_cluster = 2;
	//vol.I.push_back(tp);
	printListFolder(vol.I);
	cout << "Input folder you wanna copy file into: ";
	cin.ignore();
	getline(cin, r_fol);
	int atp = 1, sDET = 0;
	for (int i = 0; i < vol.I.size(); i++)
	{
		if (r_fol == vol.I[i].name)
		{
			sDET = i;
		}
		if (vol.I[i].name == filename && vol.I[i].folder == r_fol)
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
	int empty_pos = 0, sector_pos = ClusterToSector(vol.I[sDET].start_cluster, vol.BS);//(vol.I[sDET].start_cluster - 2) * vol.BS.SperCluster;
	for (; sector_pos < ClusterToSector(vol.I[sDET].start_cluster, vol.BS) + vol.BS.SperCluster; sector_pos++)
	{
		for (empty_pos = 0; empty_pos < vol.BS.BperSector; empty_pos += 128)
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
	int32 data_pos = ClusterToSector(new_File.start_cluster, vol.BS); //(new_File.start_cluster - 2)* vol.BS.SperCluster;
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
			int32 data_pos = ClusterToSector(fat_arr[i], vol.BS);
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
	vol.I = createList(vol);
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
void createFolder(string filename, Volume& vol)
{
	string r_fol;
	//Item tp;
	//tp.name = vol.nameVol;
	//tp.folder = vol.nameVol;
	//tp.file = 0;
	//tp.n_cluster = 0;
	//tp.start_cluster = 2;
	//vol.I.push_back(tp);
	printListFolder(vol.I);
	cout << "Input folder you wanna create a child folder: ";
	cin.ignore();
	getline(cin, r_fol);
	int atp = 1, sDET = 0;
	//check xem folder co ton tai trong sDET chua, neu co roi thi doi ten
	for (int i = 0; i < vol.I.size(); i++)
	{
		if (r_fol == vol.I[i].name)
		{
			sDET = i;
		}
		if (vol.I[i].name == filename && vol.I[i].folder == r_fol)
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
	new_File.file = 0;
	new_File.n_cluster = 1;

	int pos = 2;
	for (; pos < vol.FT.Fat.size(); pos++)
	{
		if (vol.FT.Fat[pos] == 0)
		{
			vol.FT.Fat[pos] = 268435455;	//0FFF FFFFh 
			break;
		}
	}
	// ghi sdet
	int empty_pos = 0, sector_pos = ClusterToSector(vol.I[sDET].start_cluster, vol.BS);
	for (; sector_pos < ClusterToSector(vol.I[sDET].start_cluster, vol.BS) + vol.BS.SperCluster; sector_pos++)
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
	//goi ham update lai vector<Item>
	vol.I = createList(vol);
}

// CONVERT
unsigned char* convert32_to_8(int32 a)
{
	unsigned char* c = (unsigned char*)(&a);
	(c[3] << 24 | c[2] << 16 | c[1] << 8 | c[0] << 0);
	return c;
}
unsigned char* convert16_to_8(int16 a)
{
	unsigned char* c = (unsigned char*)(&a);
	(c[1] << 8 | c[0] << 0);
	return c;
}
int16 convert8_to_16(unsigned char* a) {
	unsigned char x = a[1], y = a[0];
	int16 temp = x * 256 + y;
	return temp;
}
int32 convert8_to_32(unsigned char* a) {
	unsigned char x = a[3], y = a[2], z = a[1], t = a[0];
	int32 temp = x * 256 * 256 * 256 + y * 256 * 256 + z * 256 + t;
	return temp;
}

// LIST
void printListFolder(vector<Item> I)
{
	for (int i = 0; i < I.size(); i++)
	{
		if (!I[i].file)
			cout << "File: " << I[i].name << endl;
	}
}
void printListFile(vector<Item> I)
{
	for (int i = 0; i < I.size(); i++)
	{
		if (I[i].file)
			cout << "File: " << I[i].name << endl;
	}
}

//xoa 1 file hoac 1 folder
void deleteItem(string filename, Volume& vol) {
	//do nthing
}
//tao thong so phu hop cho cac bien cua boot sector
void createInfor(Volume& vol) {
	cout << "Enter your volume's name: ";
	cin >> vol.nameVol;
	// các thông s? h?p lý cho FAT32
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

	//input so sector mong muon cho vung bootsector (neu k phai la boi so cua 2 thi yeu cau nhap lai) (3)
	int16 SectorofBootSector = 0;
	do {
		cout << "How many sectors you want for Boot Sector:  ";
		cin >> SectorofBootSector;
		if (SectorofBootSector % 2 != 0 || SectorofBootSector < 2) cout << "Please enter again" << endl;
	} while (SectorofBootSector % 2 != 0 || SectorofBootSector < 2);

	//input s? sector per cluster (neu < 1 thi yeu cau nhap lai) (4)
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
	vol.BS.numFAT = 1;
	cout << "Size of volume (sector) is " << vol.BS.vol_size << endl;
	//==> t? các thông s? trên ta ph?i suy ra ???c s? sector cho FAT = SF
	//FAT32 ==> kích th??c m?i ph?n t? trong b?ng FAT là 4 bytes 
	//công th?c tính SF:
	double tmp_FATsize = 0.0;
	tmp_FATsize = (vol.BS.vol_size - vol.BS.BootSector + 2 * vol.BS.SperCluster) * 1.0 / (vol.BS.BperSector * vol.BS.SperCluster / 4 + 1);
	vol.BS.FAT_size = ceilf(tmp_FATsize); //hàm làm tròn lên

	vol.FT.Fat.push_back(268435448);
	vol.FT.Fat.push_back(4294967295);
	for (int i = 2; i < vol.BS.FAT_size * vol.BS.BperSector / 4; i++)
		vol.FT.Fat.push_back(0);

	//c?p nh?t thêm ==> cho s? nh? thôi n?u không h? máy 

	Sector temp = { 0 };
	int8 s[512] = { 0 };
	for (int i = 0; i < (vol.BS.vol_size - vol.BS.BootSector - vol.BS.FAT_size); i++)
		vol.D.sec.push_back(temp);
	vol.FT.Fat[2] = 268435455;

	Item t;
	t.file = 0;
	t.folder = vol.nameVol;
	t.name = vol.nameVol;
	t.start_cluster = 2;
	vol.I.push_back(t);
	cout << vol.I[0].name << endl;
	//tao entry dau tien cho sector[0]

	int pos = 0;
	for (int i = 0; i < vol.nameVol.length(); i++) {
		vol.D.sec[0].s[pos++] = vol.nameVol[i];
	}
	pos = 61;
	for (int i = 0; i < vol.nameVol.length(); i++) {
		vol.D.sec[0].s[pos++] = vol.nameVol[i];
	}
	pos = 111;
	vol.D.sec[0].s[pos++] = 0; //loai item: 0: folder, 1: file
	vol.D.sec[0].s[pos++] = 2; //cluster bat dau cua folder Volume
	vol.I = createList(vol);
}
unsigned int taoPass(string pass) {

	/*H?n ch? gõ pass có D?U */
	unsigned int init = 123321456654789987;
	//unsigned int magic = 7891234;
	int magic = rand() % 100;
	int magic2 = rand();
	unsigned int hash = 0;
	for (int i = 0; i < pass.length(); i++) {
		hash = hash << i; //dùng phép d?i bit ph?i ?? ra m?t hash m?i
		hash = hash ^ (pass[i]); // dùng phép xor ?? làm thay ??i các giá tr? bit
		hash = hash * magic + magic2;
	}
	return hash;
};
string toHex(unsigned int input) {
	string hexhash;
	stringstream hexstream;
	hexstream << hex << input; //Bi?n s? input thành 1 chu?i s? và ghép vào sau chu?i ?ang có //ví d? n?u input = 17 thì nó s? là "17"
	hexhash = hexstream.str(); //l?y content c?a hextstream
	std::transform(hexhash.begin(), hexhash.end(), hexhash.begin(), std::toupper); //toUpper chu?i hexhash
	return hexhash; //?ây chính là password sau khi ???c mã hóa
};

void createPass(string filename, Volume& vol) {
	vol.I = createList(vol);
	//vector<Item> I cua volume trong TH nay se chua danh sach cac item duoc tao tu ham createList

	int len = vol.I.size();
	for (int i = 1; i < len; i++) //vi item thu nhat la chinh cai volume do ==> bat dau tu 1
	{
		if (filename == vol.I[i].name) //do xem trong vector<Item>I co I[i].name nao == filename hay khong ? chuyen qua phan nhap pass : bao "khong co" & out
		{
			int option;
			do {
				cout << "Do you want to set password for " << filename << " Yes: 1. No: 0 ";
				cin >> option;
				if (option == 1) {
					//password ít nh?t 6 ký t?
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
//ham nay la ham menu list ra cac option can thiet
void listMenu(Volume vol) {
	int option;
	bool flag = true;
	//tiep tuc chon option cho den khi nao nhan 0.Exit thi khi do flag == false ==> out khoi while loop
	while (flag) {
		do {
			cout << "----> CHOOSE OPTION YOU WANT TO DO WITH VOLUME " << vol.nameVol << " <----" << endl;
			cout << "		      1. Read Volume				 " << endl;
			cout << "		      2. Write Volume			     " << endl;
			cout << "		      3. Export File				 " << endl;
			cout << "		      4. Import File				 " << endl;
			cout << "		      5. Delete File				 " << endl;
			cout << "		      6. Create Password			 " << endl;
			cout << "		      7. View List File				 " << endl;
			cout << "		      0. Exit						 " << endl;
			cin >> option;
			if (option < 0 || option > 7) cout << "Please enter again!" << "\n";
		} while (option < 0 || option > 7);

		switch (option) {
		case 1: {
			cout << "--> You choose option " << option << " READ VOLUME" << endl;
			readFile(vol.nameVol, vol);
			break;
		}
		case 2: {

			cout << "--> You choose option " << option << " WRITE VOLUME" << endl;
			writeFile(vol.nameVol, vol);
			break;
		}
		case 3: {
			cout << "--> You choose option " << option << " EXPORT FILE" << endl;
			string fname;
			cout << "Enter the name of file you want to export: " << endl;
			cin >> fname;
			exportItem(fname, vol, vol.nameVol);
			break;
		}
		case 4: {
			cout << "--> You choose option " << option << " IMPORT FILE" << endl;
			string fname;
			cout << "Enter the name of file you want to import: " << endl;
			cin >> fname;
			importItem(fname, vol);
			writeFile(vol.nameVol, vol);
			break;
		}
		case 5: {
			cout << "--> You choose option " << option << " DELETE FILE" << endl;
			string fname;
			cout << "Enter the name of file you want to delete: " << endl;
			cin >> fname;
			deleteItem(fname, vol);
			break;
		}
		case 6: {
			cout << "--> You choose option " << option << " CREATE PASSWORD" << endl;
			string fname;
			cout << "Enter the name of file you want to create password: " << endl;
			cin >> fname;
			createPass(fname, vol);
			break;
		}
		case 7: {
			cout << "--> You choose option " << option << " CREATE PASSWORD" << endl;
			cout << "List of files: " << endl;
			printListFile(vol.I);
			break;
		}
		default: {
			cout << "--> You don't choose any option " << endl;
			system("cls");
			flag = false;
			//return;
		}
		}
	}
	smallMenu(vol);
}
