/*���������� 200604*/


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
#define VOLWAV 9
#define WAV 10
#define BMP 11

#define STAGEFILE 12
#define VIDEOFILE 13
#define EXCEPTION 95
#define ERROR 99

int gottenToken;
char buffer[256] = { 0, };

string header[16]; //��������� ������, ������ �ε����� ������ ����
int isHeader[16] = { 0 }; //�ش��ε����� �����Ͱ� �ִ��� Ȯ��
char headerBuffer[30];

stringstream noteData;

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
	/*--------------------------------------------*/
		else if (isalpha(line[1])) {//ù ���ڰ� �������� ���(=��� �κ��� ���)
			if(line.find(" ") != string::npos){ //�߰��� ������ �ִ� ��츸
				string temp = line.substr(1, line.find(" ") - 1);//���� �պκ�
				transform(temp.begin(), temp.end(), temp.begin(), ::tolower);//����κ� Ȯ���� ���� �ҹ��ڷ� ����
				switch (headertype(temp.c_str())) { //�ؽð����� ���Ѵ�(=���������� ��ȯ)
				case headertype("player"):
					return PLAYER;
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
				case headertype("wav"):
					return WAV;
					break;
				case headertype("bmp"):
					return BMP;
					break;
				case headertype("stagefile"):
					return STAGEFILE;
					break;
				case headertype("bpmfield"):
					return VIDEOFILE;
					break;
				}

			}
			else {
				cout << "�����߻�" << endl;
				// �ش� ���ο� ������ ���� ��� ���������߻�
				return ERROR;
			}
		}
	}	
	
	return EXCEPTION;
	
}

void testOutput(stringstream& line) { //�׽�Ʈ�� ����Լ� 
	string a;
	while ((line >> a)) {
		cout << a << endl;
	}
}




int main() {


	ifstream test;
	test.open("angelic7.bme"); //�Ľ� �׽�Ʈ�� ������ �д´�

	if (test.is_open()) { //������ ���������� �о����� ���
		while (test.getline(buffer, sizeof(buffer))) { //�� ������ �� ���ξ� �о� ���ۿ� �����Ѵ�
			string lineBuffer = buffer;
			if (lineBuffer.find("#") != string::npos) // '#'���� �����ϴ� ������ ��츸 �д´�
			{
				gottenToken = getToken(buffer);


				if (gottenToken == DATA) { //������ �κ�
					noteData << lineBuffer.substr(1); //���� ������ ������ ��Ʈ���� �Է��Ѵ�
				}


				else if (gottenToken >= 1 || gottenToken <= 13) {//����κ�
					if (isHeader[gottenToken] == 0) { //����κп� �����Ͱ� ���°�쿡
						
						header[gottenToken] = lineBuffer.substr(lineBuffer.find(" ") + 1); // ':' ���� ��������͸� �����Ѵ�
						isHeader[gottenToken] = 1;

					}
					else
						cout << "�����ߺ� �߻�" << endl; 
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

	testOutput(noteData);



	while (1) {}

	return 0;
}