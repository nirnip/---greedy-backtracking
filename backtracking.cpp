#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <psapi.h>
using namespace std;
using namespace chrono;

class Stuff {
private:
	int name;
	int weight;
	int price;
	double cost = 0;
public:
	Stuff(int weight = 0, int price = 0, int name = 0) {
		this->weight = weight;
		this->price = price;
		this->name = name;
	}
	void setName(int i) {
		name = i;
	}
	void setWeight(int weight) {
		this->weight = weight;
		cost = (double)price / weight;
	}
	void setPrice(int price) {
		this->price = price;
		cost = (double)price / weight;
	}
	int getName() { return name; }
	int getWeight() { return weight; }
	int getPrice() { return price; }
	double getCost() { return cost; }
};

void quick_sort(Stuff* stList, int start, int end);
void greedy(Stuff* stList, int length, int size, double* x, double& price);
bool promising(Stuff* stList, int length, int index, int size, int weight, int profit, int maxprofit);
void backtrack(Stuff* stList, int length, int index, int size, int profit, int weight, int* tmp, int* x, int& maxprofit);

int main() {
	srand((unsigned int)time(NULL));

	Stuff* stList = NULL;
	int enter = 0;
	int W;

	double* xg = NULL;
	double gmaxprofit;

	int* xb = NULL;
	int* tmp = NULL;
	int bmaxprofit;

	int* tgreedy;
	int* tback;

	double* backTime;

	clock_t cstart, cend;
	system_clock::time_point start, end;
	nanoseconds nano;

	/* ������ ������ �賶�� ũ�� �޾� ���� */
	cout << "������ ������ �Է��ϼ��� : ";
	cin >> enter;
	if (enter <= 0) { cout << "0������ ���ڸ� �Է��߽��ϴ�. ���α׷��� �����մϴ�.\n"; }

	Stuff* list = new Stuff[enter];
	if (!list) { cout << "���� ����Ʈ�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }

	tgreedy = new int[10]{ 0 };
	if (!tgreedy) { cout << "tgreedy�迭�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }
	tback = new int[10]{ 0 };
	if (!tback) { cout << "tback�迭�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }
	backTime = new double[10]{ 0 };
	if (!tback) { cout << "backTkme�迭�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }

	for (int j = 0; j < 10; j++) {
		W = rand();														// �賶 ũ�� ���
		cout << "\n�賶�� ũ�� : " << W << endl;

		for (int i = 0; i < enter; i++) {								// ������ ����, ���� ���
			list[i].setName(i);
			list[i].setPrice(rand() % 50 + 1);
			list[i].setWeight(rand() % 50 + 1);
			//cout << i + 1 << "��° ������ price : " << list[i].getPrice() << "\tweight : " << list[i].getWeight() << endl;
		}
		cout << endl;

		xg = new double[enter] {0};
		if (!xg) { cout << "xg�迭�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }
		xb = new int[enter] {0};
		if (!xb) { cout << "xb�迭�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }
		tmp = new int[enter] {0};
		if (!tmp) { cout << "tmp�迭�� �����Ҵ翡 �����߽��ϴ�."; exit(1); }

		/* ������ �������� ���� ���� */
		quick_sort(list, 0, enter - 1);

		/* greedy */
		gmaxprofit = 0;
		start = system_clock::now();
		greedy(list, enter, W, xg, gmaxprofit);
		end = system_clock::now();

		int i = 0;
		cout << "\n----------greedy ���(fractional)----------\n�賶�� �� ������ ����\n";
		/*while ((xg[i] != 0) && (i < enter)) {
			cout << list[i].getName() + 1 << "��° ���� : " << xg[i] * list[i].getPrice() << "(price)\t" << xg[i] * list[i].getWeight() << "(weight)\n";
			i++;
		}*/
		nano = end - start;
		tgreedy[j] = nano.count();
		cout << "\nmax profit : " << gmaxprofit << "\ttime : " << nano.count() << " nano seconds\n\n";

		/* backtracking */
		bmaxprofit = 0;
		if (enter < 128) {
			start = system_clock::now();
			backtrack(list, enter, -1, W, 0, 0, tmp, xb, bmaxprofit);
			end = system_clock::now();
		}
		else {
			cstart = clock();
			backtrack(list, enter, -1, W, 0, 0, tmp, xb, bmaxprofit);
			cend = clock();
		}

		i = 0;
		cout << "\n----------backtracking ���(0/1)----------\n�賶�� �� ������ ����\n";
		/*for (int i = 0; i < enter; i++) {
			if (xb[i] == 1) {
				cout << list[i].getName() + 1 << "��° ���� : " << "\t" << list[i].getPrice() << "(price)\t" << list[i].getWeight() << "(weight)\n";
			}
		}*/


		if (enter < 128) {
			nano = end - start;
			tback[j] = nano.count();
			cout << "\nmax profit : " << bmaxprofit << "\ttime : " << tback[j] << " nano seconds\n";
		}
		else {
			backTime[j] = (double)cend - cstart;
			cout << "\nmax profit : " << bmaxprofit << "\ttime : " << backTime[j] << " micro seconds\n";
		}
	}

	double gAvg = 0, bAvg = 0;
	

	if (enter < 128) {
		for (int j = 0; j < 10; j++) {
			gAvg += tgreedy[j];		
			bAvg += tback[j];
		}
		gAvg /= 10;
		bAvg /= 10;
		cout << "\ngreedy�� ��� �ð� : " << gAvg << " nano seconds\nbacktracking�� ��� �ð� : " << bAvg << " nano seconds";
	}
	else {
		for (int j = 0; j < 10; j++) {
			gAvg += tgreedy[j];
			bAvg += backTime[j];
		}
		gAvg /= 10;
		bAvg /= 10;
		cout << "\ngreedy�� ��� �ð� : " << gAvg << " nano seconds\nbacktracking�� ��� �ð� : " << bAvg << " micro seconds";
	}
}

void quick_sort(Stuff* stList, int start, int end) {
	if (start >= end) { // ���Ұ� 1���� ��� 
		return;
	}
	int pivot = start;
	int i = pivot + 1; // ���� ��� ���� 
	int j = end; // ������ ��� ���� 
	Stuff temp;
	while (i <= j) { // �����Ͱ� ������������ �ݺ� 
		while (i <= end && stList[i].getCost() >= stList[pivot].getCost()) { i++; }
		while (j > start && stList[j].getCost() <= stList[pivot].getCost()) { j--; }
		if (i > j) { // ������ 
			temp = stList[j];
			stList[j] = stList[pivot];
			stList[pivot] = temp;
		}
		else { // i��°�� j��°�� ���� 
			temp = stList[i];
			stList[i] = stList[j];
			stList[j] = temp;
		}
	}
	// ���� ��� 
	quick_sort(stList, start, j - 1);
	quick_sort(stList, j + 1, end);
}

void greedy(Stuff* stList, int length, int size, double* x, double& price) {
	price = 0;
	int i = 0;
	while ((i < length) && (stList[i].getWeight() <= size)) {
		price += stList[i].getPrice();
		size -= stList[i].getWeight();
		x[i] = 1;
		i++;
	}
	if (i < length) {
		x[i] = (double)size / stList[i].getWeight();
		price += x[i] * stList[i].getPrice();
	}
}

void backtrack(Stuff* stList, int length, int index, int size, int profit, int weight, int* tmp, int* x, int& maxprofit) {
	if (weight <= size && profit > maxprofit) {
		maxprofit = profit;
		copy(tmp, tmp + length, x);
	}

	if (promising(stList, length, index, size, weight, profit, maxprofit) && (index + 1 < length)) {
		tmp[index + 1] = 1;
		backtrack(stList, length, index + 1, size,
			profit + stList[index + 1].getPrice(), weight + stList[index + 1].getWeight(), tmp, x, maxprofit);
		tmp[index + 1] = 0;
		backtrack(stList, length, index + 1, size, profit, weight, tmp, x, maxprofit);
	}
}
bool promising(Stuff* stList, int length, int index, int size, int weight, int profit, int maxprofit) {
	double price = profit;
	
	if (weight >= size)
		return false;
	else {
		size -= weight;
		int i = index;
		while (i < length && stList[i].getWeight() <= size) {
			price += stList[i].getPrice();
			size -= stList[i].getWeight();
			i++;
		}
		if (i < length)
			price += (double)size * stList[i].getCost();
		return (price > maxprofit);
	}
}