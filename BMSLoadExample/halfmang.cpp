/*최종수정일 200612*/


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

string header[20]; //헤더정보를 저장함, 각각의 인덱스로 정보를 구분
int isHeader[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //해당인덱스에 데이터가 있는지 확인
char headerBuffer[30];

stringstream noteData;
stringstream wavData;
stringstream bmpData;

constexpr unsigned int headertype(const char* str)
{
	return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * headertype(str + 1) : 8603; //문자열을 해시값으로 변경
}



int getToken(string line) { //토큰 단위 분석

	if (isdigit(line[1])) { //첫 문자가 숫자인 경우(=데이터 부분인 경우)
		if (line[6] == ':') {// :의 앞부분은 시간을, 뒷부분은 데이터를 나타냄, 구문의 오류가 없는지 검사
			if (isdigit(line[2]) && isdigit(line[3]) && isxdigit(line[4]) && isxdigit(line[5]))
			{
				return DATA;
			}
			else
			{
				cout << "오류발생" << endl;
				// 해당 라인에 :가 없는경우 구문오류발생
				return ERROR;
			}
		}
	}
	/*--------------------------------------------*/
	else if (isalpha(line[1])) {//첫 문자가 영문자인 경우(=헤더 부분인 경우)
		if (line.find(" ") != string::npos) { //중간에 공백이 있는 경우만
			string temp = line.substr(1, line.find(" ") - 1);//공백 앞부분
			transform(temp.begin(), temp.end(), temp.begin(), ::tolower);//헤더부분 확인을 위한 소문자로 변경


			//헤더가 bmp로 시작하거나 wav로 시작할 경우
			if (temp.length() == 5 && strcmp(temp.substr(0, 3).c_str(), "bmp") == 0 || strcmp(temp.substr(0, 3).c_str(), "wav") == 0) {
				if ( (isdigit(temp[3])||isalpha(temp[3])) && (isdigit(temp[4])||isalpha(temp[4]))) {//3,4번 인덱스가 숫자거나 영문자인지 확인
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
					cout << "WAV또는 BMP오류 발생" << endl;
					return ERROR;
				}
			}

			else {//bmp나 wav가 아닌 헤더인 경우
				switch (headertype(temp.c_str())) { //해시값으로 비교한다(=정수형으로 변환)
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
					cout << "없는 토큰 문자" << temp <<endl;
					return ERROR;
					break;
				}
			}
		}


		else {
			cout << "오류발생" << endl;
			// 해당 라인에 공백이 없는 경우 구문오류발생
			return ERROR;
		}
	}

	else
		return EXCEPTION;

}

void noteOutput(stringstream& line) { //노트데이터 출력 
	string a;
	ofstream out("note");
	while ((line >> a)) {
		out << a << endl;
	}
	out.close();
}


void wavOutput(stringstream& line) { //WAV와 BMP데이터 출력
	string a;
	ofstream out("wav");
	while ((line >> a)) {
		out << a << endl;
	}
	out.close();
}

void bmpOutput(stringstream& line) { //WAV와 BMP데이터 출력
	string a;
	ofstream out("bmp");
	while ((line >> a)) {
		out << a << endl;
	}
	out.close();
}


void headerOutput() { //헤더데이터 출력
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
	test.open(k); //파싱 테스트용 파일을 읽는다

	if (test.is_open()) { //파일이 정상적으로 읽어졌을 경우
		while (test.getline(buffer, sizeof(buffer))) { //연 파일을 한 라인씩 읽어 버퍼에 저장한다
			string lineBuffer = buffer;
			if (lineBuffer.find("#") != string::npos) // '#'으로 시작하는 라인일 경우만 읽는다
			{
				gottenToken = getToken(buffer);
				if (gottenToken == DATA) { //데이터 부분
					noteData << lineBuffer.substr(1) << " "; //현재 라인을 데이터 스트림에 입력한다
				}
				else if (gottenToken >= 1 && gottenToken <= 17) {//헤더부분

					if (gottenToken == WAV || gottenToken == BMP) {//토큰이 wav나 bmp인 경우

						if (gottenToken == WAV) {
							wavData << lineBuffer.substr(4,2)<<":"<< lineBuffer.substr(7)<<" ";  //사운드 데이터에 저장
						}
						else {
							bmpData << lineBuffer.substr(4,2)<<":"<< lineBuffer.substr(7)<<" "; //그림 데이터에 저장
						}


					}

					else if (isHeader[gottenToken] == 0) { //헤더부분에 데이터가 없는경우에
						header[gottenToken] = lineBuffer.replace(lineBuffer.find(" "), 1, ":").substr(1); // 공백을 :로 바꾼뒤 저장한다.
						isHeader[gottenToken] = 1; //헤더 유무 
					}
					else {
						cout << "구문중복 발생" << endl;
					}
				}


				else if (gottenToken == ERROR) {
					cout << "구문오류 발생" << endl;
				}


				else
					cout << "예외상황 발생" << endl;
			}
			else continue;// '#'으로 시작하지 않는 라인은 건너뛴다

		}
		test.close();
	}
	cout << "헤더 데이터" << endl;
	cout << endl;
	headerOutput();
	cout << endl;
	cout << endl;
	cout << "키음 데이터" << endl;
	cout << endl;
	wavOutput(wavData);
	cout << endl;
	cout << endl;
	cout << "배경 데이터" << endl;
	cout << endl;
	bmpOutput(bmpData);
	cout << endl;
	cout << endl;
	cout << "노트 데이터" << endl;
	cout << endl;
	noteOutput(noteData);

	}
	else {
		cout << "BMS File이 없습니다." << endl;
	}

	while (1) {}

	return 0;
}