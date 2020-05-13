#include "Header.h"


int main() {

	Volume vol;
	/*string str;
	cout << "Input file name you want to set password:";
	getline(cin, str);
	createPass(str, vol);*/
	
	int cluster;
	
	cout << "Enter the cluster -th: ";
	cin >> cluster;
	initVol(vol);
	createInfor(vol);
	cout << vol.BS.BootSector << endl;
	int32 sector_th = ClusterToSector(cluster, vol.BS);
	cout << "Cluster " << cluster << " ~ sector " << sector_th << endl;
	system("pause");
	return 0;
}