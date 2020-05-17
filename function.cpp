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
		//dò xem trong vector<string> có chứa name các volume đó có tên volume nãy mình vừa nhập hay không, nếu không thì bắt nhập lại hoặc out
		for (int i = 0; i < str.size(); i++) {
			tmp = str[i];
			if(nme == str[i]){
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
		initVol(vol);
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
void initVol(Volume & vol) 
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
		/*for (int i = 0; i < 4193272; i++)
			vol.D.sec.push_back(temp);*/
	};
	//doc vol
void readFile(string filename, Volume & vol) {
	ifstream fin(filename, ios::binary);
	if (!fin.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		fin.read((char*)& vol.BS.BperSector, sizeof(vol.BS.BperSector));
		fin.read((char*)& vol.BS.SperCluster, sizeof(vol.BS.SperCluster));
		fin.read((char*)& vol.BS.BootSector, sizeof(vol.BS.BootSector));
		fin.read((char*)& vol.BS.numFAT, sizeof(vol.BS.numFAT));
		fin.read((char*)& vol.BS.vol_size, sizeof(vol.BS.vol_size));
		fin.read((char*)& vol.BS.FAT_size, sizeof(vol.BS.FAT_size));
		fin.read((char*)& vol.BS.RDET_cluster, sizeof(vol.BS.RDET_cluster));
		fin.read((char*)& vol.BS.FAT_empty, sizeof(vol.BS.FAT_empty));
		fin.read((char*)& vol.BS.raw, sizeof(vol.BS.raw));

		int start_FAT = 512 * (vol.BS.BootSector - 1);
		fin.seekg(start_FAT, ios::cur);

		for (int i = 0; i < vol.BS.numFAT * vol.BS.FAT_size; i++)
		{
			int32 temp;
			fin.read((char*)& temp, sizeof(temp));
			vol.FT.Fat.push_back(temp);
		}

		int32 data_sec = vol.BS.vol_size - vol.BS.BootSector - vol.BS.numFAT * vol.BS.FAT_size;

		for (int i = 0; i < data_sec; i++)
		{
			Sector temp;
			fin.read((char*)& temp, sizeof(temp));
			vol.D.sec.push_back(temp);
		}

		fin.close();
	}
	}
	//ghi vol
void writeFile(string filename, Volume & vol) {
	ofstream fout(filename, ios::binary);
	if (!fout.is_open())
	{
		cout << "Can not open volume.";
		return;
	}
	else
	{
		fout.write((char*)& vol.BS.BperSector, sizeof(vol.BS.BperSector));
		fout.write((char*)& vol.BS.SperCluster, sizeof(vol.BS.SperCluster));
		fout.write((char*)& vol.BS.BootSector, sizeof(vol.BS.BootSector));
		fout.write((char*)& vol.BS.numFAT, sizeof(vol.BS.numFAT));
		fout.write((char*)& vol.BS.vol_size, sizeof(vol.BS.vol_size));
		fout.write((char*)& vol.BS.FAT_size, sizeof(vol.BS.FAT_size));
		fout.write((char*)& vol.BS.RDET_cluster, sizeof(vol.BS.RDET_cluster));
		fout.write((char*)& vol.BS.FAT_empty, sizeof(vol.BS.FAT_empty));
		fout.write((char*)& vol.BS.raw, sizeof(vol.BS.raw));

		int8 temp[512] = { 0 };
		for (int i = 0; i < vol.BS.BootSector - 1; i++)
			fout.write((char*)& temp, sizeof(temp));
		//vol.FT.Fat[2] = 268435455;
		for (int i = 0; i < vol.FT.Fat.size(); i++)
			fout.write((char*)& vol.FT.Fat[i], sizeof(vol.FT.Fat[i]));
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
			fout.write((char*)& vol.D.sec[i], sizeof(vol.D.sec[i]));
		fout.close();
	}
	}
	//ham chuyen doi tu cluster sang sector (ko tinh phan sector trong vung boot + fat)
int32 ClusterToSector(int cluster, BootSector BS) {
	int32 sector = 0;
	//cong thuc tinh cluster thu k tuong duong vs sector thu i la
	sector =  (cluster - BS.RDET_cluster) * BS.SperCluster;
	return sector;
};
	//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol){
		//do nthing
	vector<Item> I;
		return I;
};
	//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
void exportItem(string filename, Volume& vol) {
	//do nthing;
}
	//copy 1 file tu ngoai vao vol
void importItem(string filename, Volume & vol) {
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
			fin.read((char*)& t, sizeof(Sector));
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
void printList(vector<Item> I)
{
	for (int i = 0; i < I.size(); i++)
	{
		cout << "- " << I[i].folder << " " << I[i].name << endl;
	}
}
//xoa 1 file hoac 1 folder
void deleteItem(string filename, Volume & vol) {
		//do nthing
	}
	//tao thong so phu hop cho cac bien cua boot sector
void createInfor(Volume& vol) {
	
	//phần mới được thêm vào
	vector<string> list = listNameOfVolume("file.txt");
	cout << "Enter your volume's name: ";
	cin >> vol.nameVol;
	string tmp;
	//cap nhat ten volume vao trong file, neu trung thi keu dat ten lai
	for (int i = 0; i < list.size(); i++) {
		tmp = list[i];
		if (vol.nameVol == tmp)
		{
			cout << "Your new volume name has the same name with another one in file" << endl;
			cout << "Enter your volume's name again: ";
			cin >> vol.nameVol;
		}
	}
	if (vol.nameVol != tmp) {
		passNameOfVolumeToFile("file.txt", vol);
	}
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


	//cập nhật thêm ==> cho số nhỏ thôi nếu không hư máy 
	
	Sector temp = { 0 };
	int8 s[512] = { 0 }; 
	for (int i = 0; i < (vol.BS.vol_size - vol.BS.BootSector - vol.BS.FAT_size); i++)
			vol.D.sec.push_back(temp);
	vol.FT.Fat[2] = 268435455;

	
	// cái này con không chắc chắn lắm @@ qua làm hơi sảng :(( nội coi lại nếu lỗi fix giùm con lun nha :((
	for (int i = 0; i < vol.I.size(); i++) {
		vol.I.push_back(vol.I[0]);
		vol.I[0].name = vol.nameVol;
		vol.I[0].folder = vol.nameVol; //tên của volume sẽ là tên của chính item (volume) đó. Vì nó là volume nên tên của folder cha cũng chính là tên của nó
	}
	
	cout << vol.I[0].name << endl;
	//tao entry dau tien cho sector[0]
	
	int pos = 0;
	for (int i = 0; i < vol.FT.Fat.size(); i++) {
		vol.D.sec[0].s[pos++] = vol.nameVol[i]; //string out of range
	}
	pos = 61;
	for (int i = 0; i < vol.FT.Fat.size(); i++) {
		vol.D.sec[0].s[pos++] = vol.nameVol[i];
	}
	pos = 111;
	vol.D.sec[0].s[pos++] = 0; //loai item: 0: folder, 1: file
	vol.D.sec[0].s[pos++] = 2; //cluster bat dau cua folder Volume

}
unsigned int taoPass(string pass) {
	
	/*Hạn chế gõ pass có DẤU */
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
	};
string toHex(unsigned int input) {
		string hexhash;
		stringstream hexstream;
		hexstream << hex << input; //Biến số input thành 1 chuỗi số và ghép vào sau chuỗi đang có //ví dụ nếu input = 17 thì nó sẽ là "17"
		hexhash = hexstream.str(); //lấy content của hextstream
		std::transform(hexhash.begin(), hexhash.end(), hexhash.begin(), std::toupper); //toUpper chuỗi hexhash
		return hexhash; //đây chính là password sau khi được mã hóa
	};

void createPass(string filename, Volume & vol) {
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
				cout << "		      0. Exit						 " << endl;
				cin >> option;
				if (option < 0 || option > 6) cout << "Please enter again!" << "\n";
			} while (option < 0 || option > 6);

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
				exportItem(fname, vol);
				break;
			}
			case 4: {
				cout << "--> You choose option " << option << " IMPORT FILE" << endl;
				string fname;
				cout << "Enter the name of file you want to import: " << endl;
				cin >> fname;
				importItem(fname, vol);
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
void passNameOfVolumeToFile(string fname, Volume vol)
{
	std::ofstream  fout;
	fout.open(fname, std::ios_base::app);
	fout << "\n"; 
	fout << vol.nameVol;
	fout.close();
}

