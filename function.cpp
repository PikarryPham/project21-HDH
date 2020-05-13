#include "Header.h"

//khoi tao vol
void initVol(Volume& vol)
{
	vol.BS.BperSector = 512;
	vol.BS.FAT_empty = 2;
	vol.BS.FAT_size = 1024;
	vol.BS.BootSector = 8;
	vol.BS.numFAT = 1;
	vol.BS.RDET_cluster = 2;
	vol.BS.SperCluster = 32;
	vol.BS.vol_size = 4194304;
	vol.FT.Fat.push_back(268435448);
	vol.FT.Fat.push_back(4294967295);
	for (int i = 2; i < 131072; i++)
		vol.FT.Fat.push_back(0);
	int8 temp[512] = { 0 };
	/*for (int i = 0; i < 4193272; i++)
		vol.D.sec.push_back(temp);*/
}
//doc vol
void readFile(string filename, Volume vol)
{

}
//ghi vol
void writeFile(string filename, Volume vol)
{

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
//tao danh sach cac file + thu muc co trong vol
vector<Item> createList(Volume vol)
{
	vector<Item> it = vol.I;
	
	return it;
}
//chep 1 file tu vol ra ngoai (check xem file co pass hay ko, neu co thi yeu cau nhap pass)
void exportItem(string filename, vector<Item> IT, Data D)
{

}
//copy 1 file tu ngoai vao vol
void importItem(string filename, vector<Item> IT, Volume vol)
{

}
//xoa 1 file hoac 1 folder
void deleteItem(string filename, vector<Item> IT, Volume vol)
{

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
	cout << "3.TB" << endl;
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
	case 3: {//trong TH nguoi dung nhap TB --> bytes
		bytes = size * pow(1024, 4);
		cout << "The volume has " << size << "TB ~ " << bytes << " bytes" << endl;
		break;
	}
	default: //mac dinh la bytes
		bytes = size;
		cout << "The volume has " << bytes << " bytes" << endl;
	}

	//input so bytes per sector mong muon (2)
	int16 BperSector = 0;
	do {
		cout << "How many number of sectors do you want for per sector (512,1024,....): ";
		cin >> BperSector;

		if (BperSector % 2 != 0 || BperSector < 512) cout << "Please enter again" << endl;
	} while (BperSector % 2 != 0 || BperSector < 512);

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
	vol.BS.BperSector = BperSector;
	vol.BS.BootSector = SectorofBootSector;
	vol.BS.vol_size = bytes / BperSector;
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
	//hàm này có sử dụng class stringstream để ghép nối chuỗi thật nhanh chóng & tiện lợi, thao tác nhanh hơn vs string --> có sử dụng
	string hexhash;
	stringstream hexstream;
	hexstream << hex << input; //Biến số input thành 1 chuỗi số và ghép vào sau chuỗi đang có //ví dụ nếu input = 17 thì nó sẽ là "17"
	hexhash = hexstream.str(); //lấy content của hextstream
	std::transform(hexhash.begin(), hexhash.end(), hexhash.begin(), std::toupper); //toUpper chuỗi hexhash
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
