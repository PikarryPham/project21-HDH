#include "Header.h"
int main() {

	Volume vol;
	/*string str;
	cout << "Input file name you want to set password:";
	getline(cin, str);
	createPass(str, vol);*/
	int cluster;
	//cout << "Enter the cluster -th: ";
	//cin >> cluster;

	/*initVol(vol);
	writeFile("Do_an.iso", vol);
	string filename = "Do_an.iso";
	Item t;
	t.file = 0;
	t.folder = filename;
	t.name = filename;
	t.start_cluster = 2;
	vol.I.push_back(t);
	importItem("F4.txt", vol);
	importItem("F2.txt", vol);
	importItem("F3.txt", vol);
	writeFile("Do_an.iso", vol);*/
	readFile("Do_an.iso",vol);
	vol.I=createList(vol);
	exportItem("F4.txt", vol,"Do_an.iso");

	//exportItem("F4.txt", vol);*/

	

	/*createInfor(vol);
	cout << vol.BS.BootSector << endl;
	int32 sector_th = ClusterToSector(cluster, vol.BS);
	cout << "Cluster " << cluster << " ~ sector " << sector_th << endl;*/
	system("pause");
	return 0;
}