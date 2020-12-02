/*���������� 200612*/


#include<iostream>
#include<stdlib.h> 
#include<fstream>
#include<string>
#include<sstream>
#include<ctype.h>
#include <algorithm>
using namespace std;

#define DATA 0
#define PLAYER 1
#define TITLE 2
#define ARTIST 3
#define GENRE 4
#define BPM 5
#define PLAYLEVEL 6
#define RANK 7
#define TOTAL 8
#define DIFFICULTY 9


#define VOLWAV 10
#define WAV 11
#define BMP 12
#define BANNER 13
#define STAGEFILE 14
#define VIDEOFILE 15
#define BACKBMP 16

#define LNOBJ 17

#define EXCEPTION 95
#define ERROR 99
int gottenToken;
char buffer[256] = { 0, };

string header[20]; //��������� ������, ������ �ε����� ������ ����
int isHeader[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //�ش��ε����� �����Ͱ� �ִ��� Ȯ��
char headerBuffer[30];

stringstream noteData;
stringstream wavData;
stringstream bmpData;

constexpr unsigned int headertype(const char* str)
{
	return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * headertype(str + 1) : 8603; //���ڿ��� �ؽð����� ����
}



int getToken(string line) { //��ū ���� �м�

	if (isdigit(line[1])) { //ù ���ڰ� ������ ���(=������ �κ��� ���)
		if (line[6] == ':') {// :�� �պκ��� �ð���, �޺κ��� �����͸� ��Ÿ��, ������ ������ ������ �˻�
			if (isdigit(line[2]) && isdigit(line[3]) && isxdigit(line[4]) && isxdigit(line[5]))
			{
				return DATA;
			}
			else
			{
				cout << "�����߻�" << endl;
				// �ش� ���ο� :�� ���°�� ���������߻�
				return ERROR;
			}
		}
	}
	/*--------------------------------------------*/
	else if (isalpha(line[1])) {//ù ���ڰ� �������� ���(=��� �κ��� ���)
		if (line.find(" ") != string::npos) { //�߰��� ������ �ִ� ��츸
			string temp = line.substr(1, line.find(" ") - 1);//���� �պκ�
			transform(temp.begin(), temp.end(), temp.begin(), ::tolower);//����κ� Ȯ���� ���� �ҹ��ڷ� ����


			//����� bmp�� �����ϰų� wav�� ������ ���
			if (temp.length() == 5 && strcmp(temp.substr(0, 3).c_str(), "bmp") == 0 || strcmp(temp.substr(0, 3).c_str(), "wav") == 0) {
				if ( (isdigit(temp[3])||isalpha(temp[3])) && (isdigit(temp[4])||isalpha(temp[4]))) {//3,4�� �ε����� ���ڰų� ���������� Ȯ��
					switch (headertype(temp.substr(0, 3).c_str())) {
					case headertype("wav"):
						return WAV;
						break;
					case headertype("bmp"):
						return BMP;
						break;
					}
				}
				else
				{
					cout << "WAV�Ǵ� BMP���� �߻�" << endl;
					return ERROR;
				}
			}

			else {//bmp�� wav�� �ƴ� ����� ���
				switch (headertype(temp.c_str())) { //�ؽð����� ���Ѵ�(=���������� ��ȯ)
				case headertype("player"):
					return PLAYER;
					break;
				case headertype("playlevel"):
					return PLAYLEVEL;
					break; 
				case headertype("title"):
					return TITLE;
					break;
				case headertype("artist"):
					return ARTIST;
					break;
				case headertype("genre"):
					return GENRE;
					break;
				case headertype("bpm"):
					return BPM;
					break;
				case headertype("rank"):
					return RANK;
					break;
				case headertype("total"):
					return TOTAL;
					break;
				case headertype("volwav"):
					return VOLWAV;
					break;
				case headertype("stagefile"):
					return STAGEFILE;
					break;
				case headertype("bpmfield"):
					return VIDEOFILE;
					break;
				case headertype("difficulty"):
					return DIFFICULTY;
					break;
				case headertype("backbmp"):
					return BACKBMP;
					break;
				default:
					cout << "���� ��ū ����" << temp <<endl;
					return ERROR;
					break;
				}
			}
		}


		else {
			cout << "�����߻�" << endl;
			// �ش� ���ο� ������ ���� ��� ���������߻�
			return ERROR;
		}
	}

	else
		return EXCEPTION;

}

void noteOutput(stringstream& line) { //��Ʈ������ ��� 
	string a;
	ofstream out("note");
	while ((line >> a)) {
		out << a << endl;
	}
	out.close();
}


void wavOutput(stringstream& line) { //WAV�� BMP������ ���
	string a;
	ofstream out("wav");
	while ((line >> a)) {
		out << a << endl;
	}
	out.close();
}

void bmpOutput(stringstream& line) { //WAV�� BMP������ ���
	string a;
	ofstream out("bmp");
	while ((line >> a)) {
		out << a << endl;
	}
	out.close();
}


void headerOutput() { //��������� ���
	string a;
	ofstream out("header");
	for (int i = 0; i < 20; i++) {
		if (isHeader[i]) {
			out << header[i] << endl;
		}
	}
}


int main(int argc, char*argv[]) {
	
	if (argc == 2){
	string k = argv[1];
	ifstream test;
	test.open(k); //�Ľ� �׽�Ʈ�� ������ �д´�

	if (test.is_open()) { //������ ���������� �о����� ���
		while (test.getline(buffer, sizeof(buffer))) { //�� ������ �� ���ξ� �о� ���ۿ� �����Ѵ�
			string lineBuffer = buffer;
			if (lineBuffer.find("#") != string::npos) // '#'���� �����ϴ� ������ ��츸 �д´�
			{
				gottenToken = getToken(buffer);
				if (gottenToken == DATA) { //������ �κ�
					noteData << lineBuffer.substr(1) << " "; //���� ������ ������ ��Ʈ���� �Է��Ѵ�
				}
				else if (gottenToken >= 1 && gottenToken <= 17) {//����κ�

					if (gottenToken == WAV || gottenToken == BMP) {//��ū�� wav�� bmp�� ���

						if (gottenToken == WAV) {
							wavData << lineBuffer.substr(4,2)<<":"<< lineBuffer.substr(7)<<" ";  //���� �����Ϳ� ����
						}
						else {
							bmpData << lineBuffer.substr(4,2)<<":"<< lineBuffer.substr(7)<<" "; //�׸� �����Ϳ� ����
						}


					}

					else if (isHeader[gottenToken] == 0) { //����κп� �����Ͱ� ���°�쿡
						header[gottenToken] = lineBuffer.replace(lineBuffer.find(" "), 1, ":").substr(1); // ������ :�� �ٲ۵� �����Ѵ�.
						isHeader[gottenToken] = 1; //��� ���� 
					}
					else {
						cout << "�����ߺ� �߻�" << endl;
					}
				}


				else if (gottenToken == ERROR) {
					cout << "�������� �߻�" << endl;
				}


				else
					cout << "���ܻ�Ȳ �߻�" << endl;
			}
			else continue;// '#'���� �������� �ʴ� ������ �ǳʶڴ�

		}
		test.close();
	}
	cout << "��� ������" << endl;
	cout << endl;
	headerOutput();
	cout << endl;
	cout << endl;
	cout << "Ű�� ������" << endl;
	cout << endl;
	wavOutput(wavData);
	cout << endl;
	cout << endl;
	cout << "��� ������" << endl;
	cout << endl;
	bmpOutput(bmpData);
	cout << endl;
	cout << endl;
	cout << "��Ʈ ������" << endl;
	cout << endl;
	noteOutput(noteData);

	}
	else {
		cout << "BMS File�� �����ϴ�." << endl;
	}

	while (1) {}

	return 0;
}