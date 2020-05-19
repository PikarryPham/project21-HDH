#include "Header.h"
int main() {
	Volume vol;
	fixConsoleWindow();
	stopScrolling();
	smallMenu(vol);
	//hien thi con tro
	setCursor(1, 10);
	//nameVolMenu(vol);
	//createInfor(vol);
	//writeFile(vol.nameVol, vol);
	//importItem("DoAn.docx", vol);
	//writeFile("DoAn.docx", vol);
	system("pause");
	return 0;
}