/*최종수정일 200604*/


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

string header[16]; //헤더정보를 저장함, 각각의 인덱스로 정보를 구분
int isHeader[16] = { 0 }; //해당인덱스에 데이터가 있는지 확인
char headerBuffer[30];

stringstream noteData;

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
	/*--------------------------------------------*/
		else if (isalpha(line[1])) {//첫 문자가 영문자인 경우(=헤더 부분인 경우)
			if(line.find(" ") != string::npos){ //중간에 공백이 있는 경우만
				string temp = line.substr(1, line.find(" ") - 1);//공백 앞부분
				transform(temp.begin(), temp.end(), temp.begin(), ::tolower);//헤더부분 확인을 위한 소문자로 변경
				switch (headertype(temp.c_str())) { //해시값으로 비교한다(=정수형으로 변환)
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
				cout << "오류발생" << endl;
				// 해당 라인에 공백이 없는 경우 구문오류발생
				return ERROR;
			}
		}
	}	
	
	return EXCEPTION;
	
}

void testOutput(stringstream& line) { //테스트용 출력함수 
	string a;
	while ((line >> a)) {
		cout << a << endl;
	}
}




int main() {


	ifstream test;
	test.open("angelic7.bme"); //파싱 테스트용 파일을 읽는다

	if (test.is_open()) { //파일이 정상적으로 읽어졌을 경우
		while (test.getline(buffer, sizeof(buffer))) { //연 파일을 한 라인씩 읽어 버퍼에 저장한다
			string lineBuffer = buffer;
			if (lineBuffer.find("#") != string::npos) // '#'으로 시작하는 라인일 경우만 읽는다
			{
				gottenToken = getToken(buffer);


				if (gottenToken == DATA) { //데이터 부분
					noteData << lineBuffer.substr(1); //현재 라인을 데이터 스트림에 입력한다
				}


				else if (gottenToken >= 1 || gottenToken <= 13) {//헤더부분
					if (isHeader[gottenToken] == 0) { //헤더부분에 데이터가 없는경우에
						
						header[gottenToken] = lineBuffer.substr(lineBuffer.find(" ") + 1); // ':' 뒤의 헤더데이터를 저장한다
						isHeader[gottenToken] = 1;

					}
					else
						cout << "구문중복 발생" << endl; 
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

	testOutput(noteData);



	while (1) {}

	return 0;
}