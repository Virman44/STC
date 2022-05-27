#include <stdio.h> //������������ ��� ������ ����������
#include <string.h>
#include <stdlib.h>
#include <math.h>


int findmax(unsigned long long numbers[]) { //������� ��� ����� 4� ����� � ���������� �������������
	unsigned short max = 0;
	for (short i = 0; i < 4; i++) {
		scanf_s("%llu", &numbers[i]);
		if (numbers[i] > max)
			max = numbers[i];
	}
	return max;
}

unsigned long long findopred(unsigned long long numbers[]) { //������� ������������ �������
	unsigned long long matrix[3][3] = { {numbers[0], numbers[1], 1}, {numbers[1], numbers[2], 1}, {numbers[2], numbers[3], 1} };
	return llabs(matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] + matrix[1][0] * matrix[2][1] * matrix[0][2] - matrix[0][2] * matrix[1][1] * matrix[2][0] - matrix[0][1] * matrix[1][0] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1]);
}



int main(void) {
	unsigned long long numbers[4]; //������ ��� �������� ����� �1, �2, �3, �4
	unsigned short max, check = 0, stop = 0, j = 1; //max - ����������� �������� �� ��������� �����, check - �������� �� ��������������, stop - ����� �� ����� ��� ���������� j > 100, j - ���������� ��������� �������
	int lastNum; // ����� �5
	unsigned long long opred; //������������ �������
	printf("Enter the first 4 numbers: ");
	max = findmax(numbers); //������ ����� � ������� ������������
	printf("Result:\n");
	opred = findopred(numbers); //������� ������������ �������
	for (unsigned long long m = max + 1; m <= 65535; m++) { // ����� m ����� ������ max �� 1
		if (opred % m == 0) { //������������ ����� ������� ����� m, ����� ������� �� ������� ����������� ����������
			for (unsigned long long a = 0; a < m; a++) { // ���������� � � � ��������� ���������, ��� ��� �������� ����� m
				for (unsigned long long c = 0; c < m; c++) {
					for (short i = 0; i < 3; i++) { //�������� �� ��������������
						if (numbers[i + 1] == (a * numbers[i] + c) % m) { //������� ��������������
							check++; //������� ��� ����������� ������������ � � �
						}
						else { //���� ������� �������������� �����, �������� ������� � ��������� � �������� ���� � � �
							check = 0;
							break;
						}
						if (check == 3) { //���� ��� 3 ��������� ��������
							lastNum = (a * numbers[3] + c) % m;//������ ��������� �����
							printf("Case %3d: a = %5llu c = %5llu m = %5llu nextNumber = %5d\n", j, a, c, m, lastNum);//������� ����� ��������, ����� �, �, m � �5
							j++;//������� ���������� ��������� �������
							if (j > 100) {//���� ���������� ��������� ������� ��������� 100, ������� �� �����, ��� ���������� ��� ��� �������, ����� ������������ ������� ����� 0, �������� 7 6 9 0, ����� �� �������� ��������� ������� ��� 10<=m<=65535
								stop = 1;
							}

						}

					}
					if (stop == 1) break;
				}
				if (stop == 1) break;
			}
			if (stop == 1) break;
		}
	}
	return 0;
}