#include <bits/stdc++.h>
#define Rand() ((rand() * 65536) + rand())
int main(){
	freopen("input.txt", "w", stdout);
	FILE *json = fopen("EsriNapervilleElectricNetwork.json", "w");
	int N = 1000000, M = 1000030, ST = 2, ED = 2;
	for (int i = 0; i < 10000000; i ++ );
	srand(unsigned(time(NULL) + clock()));
	/*
	printf("%d %d\n", N, M);
	for (int i = 1; i <= N; i ++ )
		printf("%d\n", i);
	for (int j = 1; j <= M; j ++ ){
		int x = Rand() % N + 1, y = Rand() % N + 1;
		for (; x == y; y = Rand() % N + 1);
		// make all point connected
		if (j < N){ x = Rand() % j + 1; y = j + 1; }
		printf("%d %d e%d\n", x, y, j);
	}
	printf("%d\n", ST);
	for (; ST -- ; )
		printf("%d ", Rand() % N + 1);
	printf("\n%d\n", ED);
	for (; ED -- ; )
		printf("%d ", Rand() % N + 1);
	*/
	fprintf(json, "{\"type\":\"tabular\",\"sourceMapping\":{\"1\":\"ConnectivityAssociation\",\"6\":\"QS10085.UNADMIN.ElectricDistributionDevice\",\"7\":\"QS10085.UNADMIN.ElectricDistributionLine\",\"9\":\"QS10085.UNADMIN.ElectricDistributionJunction\"},\"rows\":[");
	char onerow[] = "{\"viaNetworkSourceId\":7,\"viaGlobalId\":\"E%d\",\"viaAssetGroupName\":\"Low Voltage\",\"viaGeometry\":{\"hasZ\":true,\"hasM\":true,\"paths\":[[[6815682.885900899768,1843065.821362793446,0,null],[6815770.9437956512,1843051.783333137631,0,null]]]},\"fromNetworkSourceId\":9,\"fromGlobalId\":\"%d\",\"fromAssetGroupName\":\"Connection Point\",\"fromTerminalId\":1,\"fromGeometry\":{\"x\":6815682.885900899768,\"y\":1843065.821362793446,\"z\":0,\"m\":null},\"toNetworkSourceId\":9,\"toGlobalId\":\"%d\",\"toAssetGroupName\":\"Connection Point\",\"toTerminalId\":1,\"toGeometry\":{\"x\":6815770.9437956512,\"y\":1843051.783333137631,\"z\":0,\"m\":null}}";
	printf("%d %d\n", N, M);
	for (int i = 1; i <= N; i ++ )
		printf("%d\n", i);
	for (int i = 1; i <= M; i ++ ){
		int x = Rand() % N + 1, y = Rand() % N + 1;
		for (; x == y; y = Rand() % N + 1);
		// make all point connected
		if (i < N){ x = Rand() % i + 1; y = i + 1; }
		fprintf(json, onerow, i, x, y);
		printf("%d %d E%d\n", x, y, i);
		if (i != M) fprintf(json, ",");
	}
	fprintf(json, "],\"controllers\":[");
	char control[] = "{\"networkSourceId\":6,\"globalId\":\"%d\",\"assetGroupName\":\"Transformer\",\"terminalId\":5,\"geometry\":{\"x\":6815946.0382617265,\"y\":1843672.782419219613,\"z\":0,\"m\":null}}";
	printf("%d\n", ST);
	for (; ST -- ; ){
		int t = Rand() % N + 1;
		fprintf(json, control, t);
		printf("%d ", t);
		if (ST != 0) fprintf(json, ",");
	}
	fprintf(json, "]}");
	//fclose(stdout);
	printf("\n%d\n", ED);
	FILE *start = fopen("startingpoints.txt", "w");
	for (; ED -- ; ){
		//int r = Rand() % (N + M) + 1;
		//if (r <= N) printf("%d\n", r);
		//else printf("E%d\n", r - N);
		int r = Rand() % N + 1;
		printf("%d ", r);
		fprintf(start, "%d\n", r);
	}
}
