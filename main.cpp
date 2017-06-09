#define _USE_MATH_DEFINES

#include <math.h>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include "opencv2/video/tracking.hpp"
#define P(s) cout << s << endl
using namespace cv;
using namespace std;


class koma{
public:
	int dir;
	string type;
};

class komaDate{
public:
	Mat img;
	string type;
	int dir;
	komaDate(Mat i, string t,int d){
		img = i;
		type = t;
		dir = d;
	}
};

int sente;
int turnDir;
int boardWidth;
int boardHeight;
int masuWidth;
int masuHeight;
vector<koma> Jibun, Aite;
vector<komaDate> komaDates;
map<string, Mat> komaImgs;
vector<vector<Mat>>KIFU;
vector<string> kifuTxt;
Mat BOARDIMG;
koma banmen[9][9], prevbanmen[9][9];//�@�ŐV�̔Ֆʁ@���O�̔Ֆ�
int misscount = 0;
//-----------------------------------------------
//�Ֆʉ摜�������Ɏg�����������̔z��
Mat nums[] = {
	imread("./image/kanji/1.png")
	, imread("./image/kanji/2.png")
	, imread("./image/kanji/3.png")
	, imread("./image/kanji/4.png")
	, imread("./image/kanji/5.png")
	, imread("./image/kanji/6.png")
	, imread("./image/kanji/7.png")
	, imread("./image/kanji/8.png")
	, imread("./image/kanji/9.png")
};
Mat num[] = {
	imread("./image/kanji/num9.png")
	, imread("./image/kanji/num8.png")
	, imread("./image/kanji/num7.png")
	, imread("./image/kanji/num6.png")
	, imread("./image/kanji/num5.png")
	, imread("./image/kanji/num4.png")
	, imread("./image/kanji/num3.png")
	, imread("./image/kanji/num2.png")
	, imread("./image/kanji/num1.png")
};

//------------------------------------------------
//��̉摜�Ɗ����ƌ������Z�b�g�ɂ����z��쐬�A�Ֆʉ摜�������Ɏg����̊����摜�̔z��쐬
void importKoma(){ 
	komaDates.push_back(komaDate(imread("./image/koma/fu1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/fu2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/gin1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/gin2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/gyoku2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/hisya1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/hisya2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kaku1.png"), "�p", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kaku2.png"), "�p", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kei1.png"), "�j", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kei2.png"), "�j", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kin1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kin2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kyo1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kyo2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/narigin1.png"), "�S", 1));
	komaDates.push_back(komaDate(imread("./image/koma/narigin2.png"), "�S", 2));
	komaDates.push_back(komaDate(imread("./image/koma/narikei1.png"), "�\", 1));
	komaDates.push_back(komaDate(imread("./image/koma/narikei2.png"), "�\", 2));
	komaDates.push_back(komaDate(imread("./image/koma/narikyo1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/narikyo2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/ou1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/ryu1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/ryu2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/to1.png"), "��", 1));
	komaDates.push_back(komaDate(imread("./image/koma/to2.png"), "��", 2));
	komaDates.push_back(komaDate(imread("./image/koma/uma1.png"), "�n", 1));
	komaDates.push_back(komaDate(imread("./image/koma/uma2.png"), "�n", 2));
	komaImgs["��"] = imread("./image/kanji/fu.png");
	komaImgs["�p"] = imread("./image/kanji/kaku.png");
	komaImgs["��"] = imread("./image/kanji/hisya.png");
	komaImgs["��"] = imread("./image/kanji/to.png");
	komaImgs["��"] = imread("./image/kanji/ou.png");
	komaImgs["��"] = imread("./image/kanji/gyoku.png");
	komaImgs["�j"] = imread("./image/kanji/kei.png");
	komaImgs["��"] = imread("./image/kanji/kyo.png");
	komaImgs["��"] = imread("./image/kanji/kin.png");
	komaImgs["��"] = imread("./image/kanji/gin.png");
	komaImgs["�S"] = imread("./image/kanji/narigin.png");
	komaImgs["�\"] = imread("./image/kanji/narikei.png");
	komaImgs["��"] = imread("./image/kanji/narikyo.png");
	komaImgs["�n"] = imread("./image/kanji/uma.png");
	komaImgs["��"] = imread("./image/kanji/ryu.png");
	komaImgs["("] = imread("./image/kanji/kakko.png");
	komaImgs[")"] = imread("./image/kanji/kokka.png");
	komaImgs["��"] = imread("./image/kanji/kurosankaku.png");
	komaImgs["��"] = imread("./image/kanji/sirosankaku.png");
	komaImgs["�s"] = imread("./image/kanji/zu.png");
	komaImgs["��"] = imread("./image/kanji/nari.png");
	komaImgs["��"] = imread("./image/kanji/utu.png");
	komaImgs["��"] = imread("./image/kanji/tori.png");
}

//------------------------------------------------
//��̓��������������̔���
//��{�I�Ɉړ��O�ƈړ���̍��W���r���ăp�^�[���ɍ����Ƃ���true ����ȊO��false��Ԃ��悤�ɂ��Ă���
//��A���A�p�A�n�A���͗�O
bool komaMovement(Point before, Point after, string koma,int dir){
	//�����������̋����ɂȂ�z��
	
	if (koma == "�\" || koma == "��" || koma == "�S" || koma == "��" || koma == "��" ){
		if (dir == 2){
			if (before.x == after.x - 1){
				if (before.y == after.y)return true;
				if (before.y == after.y - 1)return true;
			}
			if (before.x == after.x){
				if (before.y == after.y - 1)return true;
				if (before.y == after.y + 1)return true;
			}
			if (before.x == after.x + 1){
				if (before.y == after.y)return true;
				if (before.y == after.y - 1)return true;
			}
		}
		else{
			if (before.x == after.x - 1){
				if (before.y == after.y)return true;
				if (before.y == after.y + 1)return true;
			}
			if (before.x == after.x){
				if (before.y == after.y - 1)return true;
				if (before.y == after.y + 1)return true;
			}
			if (before.x == after.x + 1){
				if (before.y == after.y)return true;
				if (before.y == after.y + 1)return true;
			}
		}
	}
	else if(koma == "��"){
		if (before.x == after.x){
			if (dir == 1){
				if (before.y == after.y + 1)return true;
			}
			else{
				if (before.y == after.y - 1)return true;
			}
		}
	}
	else if (koma == "��"){
		if (dir == 2){
			if (before.x == after.x){
				for (int i = before.y + 1; i < after.y; i++){
					if (banmen[i][before.x].type != "�@"){ 
						return false; 
					}
				}
				return true;
			}
		}
		else{
			if (before.x == after.x){
				for (int i = before.y - 1; i > after.y; i--){
					if (banmen[i][before.x].type != "�@"){
						return false;
					}
				}
				return true;
			}
		}
	}
	else if (koma == "�j"){
		if (dir == 1){
			if (before.x == after.x - 1 || before.x == after.x + 1){
				if (before.y == after.y + 2)return true;
			}
		}
		else{
			if (before.x == after.x - 1 || before.x == after.x + 1){
				if (before.y == after.y - 2)return true;
			}
		}
	}
	else if (koma == "��"){
		if (dir == 2){
			if (before.x == after.x - 1){
				if (before.y == after.y + 1)return true;
				if (before.y == after.y - 1)return true;
			}
			if (before.x == after.x){
				if (before.y == after.y - 1)return true;
			}
			if (before.x == after.x + 1){
				if (before.y == after.y + 1)return true;
				if (before.y == after.y - 1)return true;
			}
		}
		else{
			if (before.x == after.x - 1){
				if (before.y == after.y + 1)return true;
				if (before.y == after.y - 1)return true;
			}
			if (before.x == after.x){
				if (before.y == after.y + 1)return true;
			}
			if (before.x == after.x + 1){
				if (before.y == after.y + 1)return true;
				if (before.y == after.y - 1)return true;
			}
		}
	}
	else if (koma == "�p"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px, py;
		if (abs(dx) == abs(dy)){
			px = dx < 0 ? -1 : 1;
			py = dy < 0 ? -1 : 1;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[before.y + i*py][before.x + i*px].type != "�@"){
					return false;
				}
			}
			return true;
		}
	}
	else if (koma == "�n"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px, py;
		if (abs(dx) == abs(dy)){
			px = dx < 0 ? -1 : 1;
			py = dy < 0 ? -1 : 1;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[before.y + i*dy][before.x + i*dx].type != "�@"){
					return false;
				}
			}
			return true;
		}
		if (before.x == after.x + 1 || before.x == after.x - 1){
			if (before.y == after.y) return true;
		}
		if (before.y == after.y + 1 || before.y == after.y - 1){
			if (before.x == after.x) return true;
		}
	}
	else if (koma == "��"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px = dx < 0 ? -1 : 1;
		int py = dy < 0 ? -1 : 1;
		if (before.x == after.x && before.y != after.y){
			int x = before.x;
			for (int i = 1; i < abs(dy); i++){
				if (banmen[before.y + i*py][x].type != "�@")return false;
			}
			return true;
		}
		else if (before.x != after.x && before.y == after.y){
			int y = before.y;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[y][before.x + i*px].type != "�@")return false;
			}
			return true;
		}
	}
	else if (koma == "��"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px = dx < 0 ? -1 : 1;
		int py = dy < 0 ? -1 : 1;
		if (before.x == after.x && before.y != after.y){
			int x = before.x;
			for (int i = 1; i < abs(dy); i++){
				if (banmen[before.y + i*py][x].type != "�@")return false;
			}
			return true;
		}
		else if (before.x != after.x && before.y == after.y){
			int y = before.y;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[y][before.x + i*px].type != "�@")return false;
			}
			return true;
		}
		if (before.x == after.x + 1 || before.x == after.x - 1){
			if (before.y == after.y - 1) return true;
			if (before.y == after.y + 1) return true;
		}
		if (before.y == after.y + 1 || before.y == after.y - 1){
			if (before.x == after.x - 1) return true;
			if (before.x == after.x + 1) return true;
		}
	}
	else if (koma == "��" || koma == "��"){
		if (before.x == after.x - 1){
			if (before.y == after.y)return true;
			if (before.y == after.y-1)return true;
			if (before.y == after.y+1)return true;
		}
		if (before.x == after.x){
			if (before.y == after.y - 1)return true;
			if (before.y == after.y + 1)return true;
		}
		if (before.x == after.x + 1){
			if (before.y == after.y)return true;
			if (before.y == after.y - 1)return true;
			if (before.y == after.y + 1)return true;
		}
	}
	return false;
}

//------------------------------------------------------------
//���݂̉f������Ֆʂ̐؂�o��
//�p�̍��W�������Ƃ��Ď󂯎�遨�󂯎�������_�ł͂ǂꂪ�ǂ̊p�Ȃ̂��킩��Ȃ����璆�S���W�Ƃ̔�r�Ō��߂�B
//�Ֆʂ̏c���̑傫�����Q���_�̋�������v�Z����i�c���ňقȂ邩�璍�Ӂj
//�����Ń}�X�̑傫�����v�Z���Ă����B
//�v�Z�����c���̑傫����Mat�𐶐����Ă����ɐ؂蔲�����摜��\��t����B

Mat takeCurrentBoard(Mat boardImg, vector<Point> points){
	vector<Point> corners(4);
	int maxX = -1, maxY = -1, minX = 1e10, minY = 1e10;
	for (int i = 0; i < points.size(); i++){
		maxX = max(maxX, points[i].x);
		maxY = max(maxY, points[i].y);
		minX = min(minX, points[i].x);
		minY = min(minY, points[i].y);
	}
	for (int i = 0; i < points.size(); i++){
		if (points[i].x < (maxX + minX) / 2 && points[i].y < (maxY + minY) / 2){
			corners[0] = points[i];
		}
		if (points[i].x < (maxX + minX) / 2 && points[i].y >(maxY + minY) / 2){
			corners[1] = points[i];
		}
		if (points[i].x >(maxX + minX) / 2 && points[i].y >(maxY + minY) / 2){
			corners[2] = points[i];
		}
		if (points[i].x >(maxX + minX) / 2 && points[i].y < (maxY + minY) / 2){
			corners[3] = points[i];
		}
	}
	float srcCorner[] = { corners[0].x, corners[0].y, corners[1].x, corners[1].y, corners[2].x, corners[2].y, corners[3].x, corners[3].y };
	double Height = sqrt(pow((corners[0].x - corners[1].x), 2.0) + pow((corners[0].y - corners[1].y), 2.0));
	double Width = sqrt(pow((corners[0].x - corners[3].x), 2.0) + pow((corners[0].y - corners[3].y), 2.0));
	float destCorner[] = { 0.0f, 0.0f, 0.0f, Height - 1, Width - 1, Height - 1, Width - 1, 0.0f };
	Mat srccp(4, 2, CV_32F, srcCorner);
	Mat destcp(4, 2, CV_32F, destCorner);
	Mat h = getPerspectiveTransform(srccp, destcp);
	boardHeight = Height;
	boardWidth = Width;
	masuWidth = Width / 9;
	masuHeight = Height / 9;
	Mat result(boardHeight, boardWidth, boardImg.type());
	warpPerspective(boardImg, result, h, result.size());
	return result;
}

//----------------------------------------------
//�Ֆʔz��̍X�V�{�Ֆʔz��f�[�^�̏o��
void outputBoard(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			cout << prevbanmen[i][j].type;
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			cout << banmen[i][j].type;
			prevbanmen[i][j] = banmen[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

//----------------------------------------------
//�Ֆʔz��̍X�V�̂�
void dontoutputBoard(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			prevbanmen[i][j] = banmen[i][j];
		}
	}
}

//----------------------------------------------
//�Ֆʔz��̍X�V�������ɔՖʔz��̃f�[�^�݂̂��o��
void justOutPut(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			cout << prevbanmen[i][j].type;
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			cout << banmen[i][j].type;
		}
		cout << endl;
	}
	cout << endl;
}

//----------------------------------------------
//�Ֆʔz��̏�����
void init(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			banmen[i][j].dir = 0;
			banmen[i][j].type = "�@";
		}
	}
}

//----------------------------------------------
//�ω������̃J�E���g
//��̎�ށA�����̂ǂ��炩����v���Ȃ������炻�̍��W�ƌ��݂����ɂ����̏���gap�ɒǉ����Ă����B
vector<pair<pair<int, int>, string> > searchGap(){
	vector<pair<pair<int, int>, string> > gap;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if ((banmen[i][j].type != prevbanmen[i][j].type) || (banmen[i][j].dir != prevbanmen[i][j].dir)){
				gap.push_back(make_pair(make_pair(i, j), banmen[i][j].type));
			}
		}
	}
	return gap;
}

//----------------------------------------------
//�Ֆʔz��Ƀf�[�^������
//�e���v���[�g�}�b�`���O�Ō��o������̒��S���W���}�X�̏c���̑傫���Ŋ��邱�ƂŔՖʏ�̂ǂ̃}�X�̗̈�ɋ�̒��S������̂����v�Z���ĔՖʔz��Ɋi�[����B
void setToBoardDatebase(int x, int y, string komaname, int dir){
	int pos_x = x / masuWidth;
	int pos_y = y / masuHeight;
	banmen[pos_y][pos_x].type = komaname;
	banmen[pos_y][pos_x].dir = dir;
}

//----------------------------------------------
//�e���v���[�g�}�b�`���O
//��̐e�f�[�^��؂蔲���Ƃ��Ɏg�����Ֆʉ摜�̉���530px�������̂ŁA�Ֆʂ̉��̒���/530����̐e�f�[�^�̏c���Ɋ|�����傫���Ɋg��k������B
//���Ƃ�-6 ~ 6�܂ł�3�x���ɌX���ăe���v���[�g�}�b�`���O����B
//�������l(threshhold)����̎�ނ��ƂɌ��߂āA�������Ƃ���̍��W������
//�������ŏ����猩�Ă����Ֆʔz��Ƀf�[�^���i�[����B
void templateMatching(Mat srcimg, pair<Mat, string> tmpimg, int dir){
	Mat src, result;
	srcimg.copyTo(src);
	Mat search;
	cvtColor(tmpimg.first, search, CV_BGR2GRAY);
	vector<Point> pos, revpos;
	Mat rollingImg, revImg;
	float scale = (float)(boardWidth / 530.0);
	Mat dstsearch(search.rows*scale + 1, search.cols*scale + 1, search.type());
	resize(search, dstsearch, dstsearch.size());
	Point2f center(dstsearch.cols / 2.0, dstsearch.rows / 2.0);
	for (double i = -6; i <= 6; i += 3){
		Mat matrix = cv::getRotationMatrix2D(center, i, 1.0);
		Point p(dstsearch.cols*scale, dstsearch.rows*scale);
		warpAffine(dstsearch, rollingImg, matrix, dstsearch.size());
		matchTemplate(src, rollingImg, result, CV_TM_CCORR_NORMED);
		float threshhold = 0.98f;
		if (tmpimg.second == "��"){
			threshhold = 0.99f;
		}
		if (tmpimg.second == "�S" || tmpimg.second == "�\"){
			threshhold = 0.985f;
		}
		for (int y = 0; y < result.rows; y++){
			for (int x = 0; x < result.cols; x++){
				if (result.at<float>(y, x) > threshhold){
					pos.push_back(Point(x, y));
				}
			}
		}
	}
	for (auto it = pos.begin(); it != pos.end(); it++){
		setToBoardDatebase(it->x + (search.cols / 2), it->y + (search.rows / 2), tmpimg.second, dir);
	}
}


//----------------------------------------------
//�Ֆʏ�ɑ��݂����̌��𐔂���
int countKomas(koma b[9][9]){
	int count = 0;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (b[i][j].type != "�@"){
				count++;
			}
		}
	}
	return count;
}

//----------------------------------------------
//�����s�����̔���
//����̗̈���ł̈ړ��A����̗̈�ւ̐N���A����̗̈悩��̒E�o�̂R�p�^�[���ɕ����Ăǂꂩ�̃p�^�[���ɓ��Ă͂܂�Ƃ��͐��̏����𖞂����Ƃ݂Ȃ��Ă���B
bool narazu(Point p, int dir){
	if (dir == 1){
		if ((p.x > 2 && p.y <= 2) || p.x <= 2){
			return true;
		}
	}
	else if (dir == 2){
		if ((p.x < 6 && p.y >= 6 ) || p.x >= 6){
			return true;
		}
	}
	return false;
}
//----------------------------------------------
//���łɐ����Ă��邩�ǂ����̔���
bool isNaried(string s){
	if (s == "��" || s == "��" || s == "�n" || s == "�S" || s == "�\" || s == "��"){
		return true;
	}
	return false;
}

string changeToNormal(string s){
	if (s == "��"){
		return "��";
	}
	else if (s == "��"){
		return "��";
	}
	else if (s == "�n"){
		return "�p";
	}
	else if (s == "�S"){
		return "��";
	}
	else if (s == "�\"){
		return "�j";
	}
	else if (s == "��"){
		return "��";
	}
}

//----------------------------------------------
//�����̏o��
//���ꊄ�Ƃ�₱������������������Ȃ�
//�Ȍ��Ɍ����Ə����@�ŃS������
bool firstMove = true;
void outputKifu(int abso, vector<pair<pair<int, int>, string> > gaps){
	vector<Mat> kifu;
	string kifutxt = "";
	string kanjinums[] = { "��", "��", "�O", "�l", "��", "�Z", "��", "��", "��" };
	cout << (turnDir == sente ? "��" : "��");
	kifutxt = kifutxt + (turnDir == sente ? "��" : "��");
	kifu.push_back(komaImgs[turnDir == sente ? "��" : "��"]);
	if (gaps.size() == 1 && abso == 1){
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << gaps[0].second << "��" << endl;
		kifutxt = kifutxt + to_string(9 - gaps[0].first.second ) + kanjinums[gaps[0].first.first] + gaps[0].second + "��";
		kifu.push_back(num[gaps[0].first.second]);
		kifu.push_back(nums[gaps[0].first.first]);
		kifu.push_back(komaImgs[gaps[0].second]);
		kifu.push_back(komaImgs["��"]);
	}
	else if (gaps.size() == 2 && abso == -1) {
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma b1 = banmen[gaps[1].first.first][gaps[1].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		koma p1 = prevbanmen[gaps[1].first.first][gaps[1].first.second];
		if (gaps[0].second != "�@"){
			kifu.push_back(num[gaps[0].first.second]);
			kifu.push_back(nums[gaps[0].first.first]);
			if (p1.type != b0.type){
				cout << 9 - gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "��(" << p0.type << "��)" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "��(" + p0.type + "��)";
				kifu.push_back(komaImgs[changeToNormal(gaps[0].second)]);
				kifu.push_back(komaImgs["��"]);
				kifu.push_back(komaImgs["("]);
				kifu.push_back(komaImgs[p0.type]);
				kifu.push_back(komaImgs["��"]);
				kifu.push_back(komaImgs[")"]);
			}
			else{
				Point move(gaps[1].first.first, gaps[0].first.first);
				if (narazu(move, p1.dir) && !isNaried(gaps[0].second)){
					cout << 9 - gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "�s��(" << p0.type << "��)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "�s��(" + p0.type + "��)";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["�s"]);
					kifu.push_back(komaImgs["��"]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p0.type]);
					kifu.push_back(komaImgs["��"]);
					kifu.push_back(komaImgs[")"]);
				}
				else{
					cout << 9 - gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "(" << p0.type << "��)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "(" + p0.type + "��)";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p0.type]);
					kifu.push_back(komaImgs["��"]);
					kifu.push_back(komaImgs[")"]);
				}
			}
			if (b0.dir == 2){
				koma add;
				add.dir = 2;
				add.type = p0.type;
				if (isNaried(add.type)){
					add.type = changeToNormal(add.type);
				}
				Jibun.push_back(add);
			}
			else{
				koma add;
				add.dir = 1;
				add.type = p0.type;
				if (isNaried(add.type)){
					add.type = changeToNormal(add.type);
				}
				Aite.push_back(add);
			}
		}
		else{
			kifu.push_back(num[gaps[1].first.second]);
			kifu.push_back(nums[gaps[1].first.first]);
			if (p0.type != b1.type){
				cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << p0.type << "��(" << p1.type << "��)" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "��(" + p1.type + "��)";
				kifu.push_back(komaImgs[changeToNormal(gaps[1].second)]);
				kifu.push_back(komaImgs["��"]);
				kifu.push_back(komaImgs["("]);
				kifu.push_back(komaImgs[p1.type]);
				kifu.push_back(komaImgs["��"]);
				kifu.push_back(komaImgs[")"]);
			}
			else{
				Point move(gaps[0].first.first, gaps[1].first.first);
				if (narazu(move, p0.dir) && !isNaried(gaps[1].second)){
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second << "�s��(" << p1.type << "��)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "�s��(" + p1.type + "��)";
					kifu.push_back(komaImgs[gaps[1].second]);
					kifu.push_back(komaImgs["�s"]);
					kifu.push_back(komaImgs["��"]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p1.type]);
					kifu.push_back(komaImgs["��"]);
					kifu.push_back(komaImgs[")"]);
				}
				else{
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second << "(" << p1.type << "��)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "(" + p1.type + "��)";
					kifu.push_back(komaImgs[gaps[1].second]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p1.type]);
					kifu.push_back(komaImgs["��"]);
					kifu.push_back(komaImgs[")"]);
				}
			}
			if (b1.dir == 2){
				koma add;
				add.dir = 2;
				add.type = p1.type;
				if (isNaried(add.type)){
					add.type = changeToNormal(add.type);
				}
				Jibun.push_back(add);
			}
			else{
				koma add;
				add.dir = 1;
				add.type = p1.type;
				if (isNaried(add.type)){
					add.type = changeToNormal(add.type);
				}
				Aite.push_back(add);
			}
		}
	}
	else if (gaps.size() == 2 && abso == 0){
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma b1 = banmen[gaps[1].first.first][gaps[1].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		koma p1 = prevbanmen[gaps[1].first.first][gaps[1].first.second];
		if (gaps[0].second != "�@"){
			kifu.push_back(num[gaps[0].first.second]);
			kifu.push_back(nums[gaps[0].first.first]);
			
			if (p1.type != b0.type){
				cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << p1.type << "��" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "��";
				kifu.push_back(komaImgs[changeToNormal(gaps[0].second)]);
				kifu.push_back(komaImgs["��"]);
			}
			else{
				Point move(gaps[1].first.first, gaps[0].first.first);
				if (narazu(move, p1.dir) && !isNaried(gaps[0].second)){
					cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "�s��" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "�s��";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["�s"]);
					kifu.push_back(komaImgs["��"]);
				}
				else{
					cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second;
					kifu.push_back(komaImgs[gaps[0].second]);
				}
			}
		}
		else{
			kifu.push_back(num[gaps[1].first.second]);
			kifu.push_back(nums[gaps[1].first.first]);
			kifu.push_back(komaImgs[gaps[1].second]);
			if (p0.type != b1.type){
				cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << p0.type << "��" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "��";
				kifu.push_back(komaImgs[changeToNormal(gaps[0].second)]);
				kifu.push_back(komaImgs["��"]);
			}
			else{
				Point move(gaps[0].first.first, gaps[1].first.first);
				if (narazu(move, p0.dir) && !isNaried(gaps[1].second)){
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second <<"�s��"<< endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "�s��";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["�s"]);
					kifu.push_back(komaImgs["��"]);
				}
				else{
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second << endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second;
					kifu.push_back(komaImgs[gaps[0].second]);
				}
			}
		}
	}
	kifuTxt.push_back(kifutxt);
	KIFU.push_back(kifu);
}

Mat makeKifuImg(vector<Mat> v){
	Mat K(20, 170, CV_8UC3, Scalar(255, 255, 255));
	int pos = 0;
	for (int i = 0; i < v.size(); i++){
		Rect tmp;
		if (v[i].cols == 17){
			tmp = Rect(pos, 2, v[i].cols, v[i].rows);
		}
		else{
			tmp = Rect(pos, 0, v[i].cols, v[i].rows);
		}
		pos += v[i].cols;
		Mat dst = K(tmp);
		v[i].copyTo(dst);
	}
	return K;
}
int kifuCount = 1;

void makeGuiLike(Mat frame){
	Mat GUI(800, 680, CV_8UC3, Scalar(255, 255, 255));
	Rect frameRect(20, 5, 640, 480);
	Mat dst = GUI(frameRect);
	frame.copyTo(dst);
	Rect boardRect(0, 500, 500, 300);
	dst = GUI(boardRect);
	BOARDIMG.copyTo(dst);
	line(GUI, Point(0, 495), Point(680, 495), Scalar(0, 0, 0), 2, 3);
	line(GUI, Point(500, 495), Point(500, 800), Scalar(0, 0, 0), 2, 3);
	int y = 0;
	if (KIFU.size() <= 10){
		for (int i = 0; i < KIFU.size(); i++){
			string s = to_string(kifuCount) + ":";
			Rect R(505, 520 + 25 * y, 170, 20);
			y++;
			Mat tmp = GUI(R);
			makeKifuImg(KIFU[i]).copyTo(tmp);
		}
	}
	else{
		for (int i = KIFU.size()-10; i < KIFU.size(); i++){
			Rect R(505, 520 + 25 * y, 170, 20);
			y++;
			Mat tmp = GUI(R);
			makeKifuImg(KIFU[i]).copyTo(tmp);
		}
	}
	imshow("SHOGI KIFU PROGRAM", GUI);
}

//----------------------------------------------
//�Ֆʉ摜�̍쐬
void makeBoardImg(){
	Mat outputBoardImg(300, 500, CV_8UC3, Scalar(255, 255, 255));
	Mat subdst;
	//�c�������̕`��
	for (int i = 0; i <= 9; i++){
		for (int j = 0; j <= 9; j++){
			line(outputBoardImg, Point(105 + 30 * i, 25), Point(105 + 30 * i, 295), Scalar(0, 0, 0), 2, 3);
			line(outputBoardImg, Point(105, 25 + 30 * j), Point(375, 25 + 30 * j), Scalar(0, 0, 0), 2, 3);
		}
	}

	line(outputBoardImg, Point(10, 5), Point(90, 5), Scalar(0, 0, 0), 2, 3);
	line(outputBoardImg, Point(10, 5), Point(10, 200), Scalar(0, 0, 0), 2, 3);
	line(outputBoardImg, Point(90, 5), Point(90, 200), Scalar(0, 0, 0), 2, 3);
	line(outputBoardImg, Point(10, 200), Point(90, 200), Scalar(0, 0, 0), 2, 3);

	line(outputBoardImg, Point(410, 95), Point(490, 95), Scalar(0, 0, 0), 2, 3);
	line(outputBoardImg, Point(410, 295), Point(410, 95), Scalar(0, 0, 0), 2, 3);
	line(outputBoardImg, Point(490, 295), Point(490, 95), Scalar(0, 0, 0), 2, 3);
	line(outputBoardImg, Point(410, 295), Point(490, 295), Scalar(0, 0, 0), 2, 3);
	for (int i = 0; i < Jibun.size(); i++){
		Rect komaRect(12 + (i % 4) * 19, 7 + (i / 4) * 19, 17, 17);
		subdst = outputBoardImg(komaRect);
		Mat tmp;
		komaImgs.find(Jibun[i].type)->second.copyTo(tmp);
		flip(tmp, tmp, -1);
		tmp.copyTo(subdst);
	}
	for (int i = 0; i < Aite.size(); i++){
		Rect komaRect(471 - (i % 4) * 19, 276 - (i / 4) * 19, 17, 17);
		subdst = outputBoardImg(komaRect);
		Mat tmp;
		komaImgs.find(Aite[i].type)->second.copyTo(tmp);
		
		tmp.copyTo(subdst);
	}
	//����̕`��
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (banmen[i][j].type != "�@"){
				Rect komaRect(112 + j * 30, 32 + i * 30, 17, 17);
				subdst = outputBoardImg(komaRect);
				Mat tmp;
				komaImgs.find(banmen[i][j].type)->second.copyTo(tmp);
				if (banmen[i][j].dir == 2){
					flip(tmp, tmp, -1);
				}
				tmp.copyTo(subdst);
			}
		}
	}
	//
	for (int i = 0; i < 9; i++){
		Rect rect1(378, 30 + 30 * i, nums[i].cols, nums[i].rows);
		subdst = outputBoardImg(rect1);
		nums[i].copyTo(subdst);
		Rect rect2((30 * i) + 115, 3, num[i].cols, num[i].rows);
		subdst = outputBoardImg(rect2);
		num[i].copyTo(subdst);
	}
	outputBoardImg.copyTo(BOARDIMG);
}
//---------------------------------------
//���[���ɂ����Ă��邩�ǂ����`�F�b�N
//����A��@�Ȉړ������m�ł���
bool checkRules(vector<pair<pair<int, int>, string> > gaps, int komaGap){
	pair<int, int> p = make_pair(gaps.size(), komaGap);
	if (p == make_pair(1, 1)){
		vector<pair<int,int> > fupos;
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		if (b0.dir != turnDir && !firstMove){
			P("���w���ł��B");
			return false;
		}
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				if (banmen[i][j].type == "��"){
					for (int k = 0; k < 9; k++){
						if (k != i){
							if (banmen[k][j].type == "��" && (banmen[k][j].dir == banmen[i][j].dir)){
								P("����ł��B");
								return false;
							}
						}
					}
				}
			}
		}
		if (firstMove){
			turnDir = sente = banmen[gaps[0].first.first][gaps[0].first.second].dir;
			firstMove = false;
		}
		bool t = true;
		if (b0.dir == 2){
			for (auto it = Jibun.begin(); it != Jibun.end(); it++){
				if (it->type == b0.type){
					Jibun.erase(it);
					return true;
				}
			}
			P("���ɂ���܂���B");
			return false;
		}
		else{
			for (auto it = Aite.begin(); it != Aite.end(); it++){
				if (it->type == b0.type){
					Aite.erase(it);
					return true;
				}
			}
			P("���ɂ���܂���B");
			return false;
		}
		return true;
	}
	else if (p == make_pair(2, -1)){
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma b1 = banmen[gaps[1].first.first][gaps[1].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		koma p1 = prevbanmen[gaps[1].first.first][gaps[1].first.second];

		if (gaps[0].second != "�@"){
			Point before(gaps[1].first.second, gaps[1].first.first), after(gaps[0].first.second, gaps[0].first.first);
			//
			if (banmen[gaps[0].first.first][gaps[0].first.second].dir != turnDir && !firstMove){
				P("���w���ł��B");
				return false;
			}
			if (!komaMovement(before, after, p1.type, b0.dir)){
				P("�s���ȓ����ł��B");
				return false;
			}
			if (firstMove){
				turnDir = sente = banmen[gaps[0].first.first][gaps[0].first.second].dir;
				firstMove = false;
			}
		}
		else{
			Point before(gaps[0].first.second, gaps[0].first.first), after(gaps[1].first.second, gaps[1].first.first);
			if (banmen[gaps[1].first.first][gaps[1].first.second].dir != turnDir && !firstMove){
				P("���w���ł��B");
				return false;
			}
			if (!komaMovement(before, after, p0.type, b1.dir)){
				P("�s���ȓ����ł��B");
				return false;
			}
			if (firstMove){
				turnDir = sente = banmen[gaps[1].first.first][gaps[1].first.second].dir;
				firstMove = false;
			}
		}
		return true;
	}
	else if (p == make_pair(2, 0)){
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma b1 = banmen[gaps[1].first.first][gaps[1].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		koma p1 = prevbanmen[gaps[1].first.first][gaps[1].first.second];
		if (gaps[0].second != "�@"){
			Point before(gaps[1].first.second, gaps[1].first.first), after(gaps[0].first.second, gaps[0].first.first);
			if (banmen[gaps[0].first.first][gaps[0].first.second].dir != turnDir && !firstMove){
				P("���w���ł��B");
				return false;
			}
			if (!komaMovement(before, after, p1.type, b0.dir)){
				P("�s���ȓ����ł��B");
				return false;
			}
			if (firstMove){
				turnDir = sente = banmen[gaps[0].first.first][gaps[0].first.second].dir;
				firstMove = false;
			}

		}
		else{
			Point before(gaps[0].first.second, gaps[0].first.first), after(gaps[1].first.second, gaps[1].first.first);
			if (banmen[gaps[1].first.first][gaps[1].first.second].dir != turnDir && !firstMove){
				P("���w���ł��B");
				return false;
			}
			if (!komaMovement(before, after, p0.type, b1.dir)){
				P("�s���ȓ����ł��B");
				return false;
			}
			if (firstMove){
				turnDir = sente = banmen[gaps[1].first.first][gaps[1].first.second].dir;
				firstMove = false;
			}
		}
		return true;
	}
}

//----------------------------------------------
//���C��
//���ꂾ��
//web�J������t���[���擾��GS���m�C�Y������Canny���֊s�擾���ő�̈�֊s�I����������������P�ڂ�while������E�o��(*)�Ֆʐ؂蔲����GS���e���v���[�g�}�b�`���O�����@�Ȉړ����ǂ������ʁ�
//��������������������o�͂��Ē��O�Ֆʔz��ɍŐV�Ֆʔz����R�s�[���遨�ȉ�(*)�ȍ~�����[�v�E�E�E
int main()
{
	importKoma();
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	Mat edges;
	Mat nowImg;
	vector<vector<Point> > boardFrame(0);
	vector<Point> corners(4);
	bool flag = false;
	Mat boardImg;
	vector<Point> contour;
	while (true)
	{
		Mat frame;
		vector<Vec4i> hierarchy;
		cap >> frame; // get a new frame from camera
		flip(frame, frame, -1);
		Mat contour;
		int maxIdx;
		vector<vector<Point> > contours;//�֊s�����W�̔z��̔z��ŕێ�
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 50, 3);
		findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
		double area, maxArea = -1, second = -1;
		maxIdx = -1;
		for (int i = 0; i < contours.size(); i++){
			area = fabs(contourArea(contours[i]));
			if (area > maxArea){
				maxIdx = i;
				maxArea = area;
			}
		}

		if (maxIdx >= 0){
			contour = Mat(contours[maxIdx]);
			vector<Point> approx;
			cv::convexHull(contour, approx, false);
			cv::approxPolyDP(approx, approx, 100, true);
			
			frame.copyTo(nowImg);
			for (int i = 0; i < approx.size(); i++){
				rectangle(nowImg, Point(approx[i].x - 2, approx[i].y - 2), Point(approx[i].x + 2, approx[i].y + 2), Scalar(0, 0, 255), 2, 8, 0);
			}
			drawContours(nowImg, contours, maxIdx, Scalar(0, 0, 255), 1);
			makeGuiLike(nowImg);
			if (approx.size() == 4 && flag){
				boardFrame.push_back(contours[maxIdx]);
				for (int i = 0; i < 4; i++){
					corners[i] = approx[i];
				}
				break;
			}
		}
		if (GetAsyncKeyState(VK_CONTROL)){
			flag = true;
		}
		if (waitKey(30) >= 0)break;
	}
	bool firstTime = true; 
	bool exit = true;
	while (exit){
		Mat frame;
		cap >> frame; // get a new frame from camera
		flip(frame, frame, -1);
		init();
		Mat searchsrc;
		cvtColor(takeCurrentBoard(frame, corners), searchsrc, CV_BGR2GRAY);
		for (int i = 0; i < komaDates.size(); i++){
			templateMatching(searchsrc, make_pair(komaDates[i].img,komaDates[i].type), komaDates[i].dir);
			cap >> frame; // get a new frame from camera
			flip(frame, frame, -1);
			for (int i = 0; i < corners.size(); i++){
				rectangle(frame, Point(corners[i].x - 2, corners[i].y - 2), Point(corners[i].x + 2, corners[i].y + 2), Scalar(0, 0, 255), 2, 8, 0);
			}
			drawContours(frame, boardFrame, 0, Scalar(0, 0, 255), 1);
			makeGuiLike(frame);
			waitKey(1);
		}
		if (firstTime){
			dontoutputBoard();
		}
		int komaNow, komaPrev;
		komaNow = countKomas(banmen);
		komaPrev = countKomas(prevbanmen);
		int komaGap = komaNow - komaPrev;
		vector<pair<pair<int, int>, string> > gaps = searchGap();
		pair<int, int> p = make_pair(gaps.size(), komaGap);
		if (p != make_pair(0, 0)){
			cout << "�ω�����:" << gaps.size() << " " << "��̑���:" << komaGap << endl;
		}
		if (p == make_pair(1, 1) || p == make_pair(2,-1) || p == make_pair(2,0) || firstTime){
			
			if(firstTime){
				P("�΋ǊJ�n�ł��B(ALT�L�[�ŏI��)");
			}

			if (checkRules(gaps, komaGap) || firstTime){
				if (!firstTime){
					outputKifu(komaGap, gaps);
				}
				dontoutputBoard();
				makeBoardImg();
				turnDir = turnDir == 1 ? 2 : 1;
			}
			firstTime = false;
		}
		if (GetAsyncKeyState(VK_MENU)){
			exit = false;
		}
	}
HERE:
	cout << "������ۑ����܂����H(y/n):";
	string yn;
	cin >> yn;
	if (yn == "y"){
		cout << "------�����̊����t�@�C��------"<< endl;
		system("ls | grep txt");
		cout << "�t�@�C��������͂��Ă�������(�����̖��O�ɂ����ꍇ�㏑������܂�):";
		string filename;
		ofstream file;
		cin >> filename;
		if (filename.find(".txt") != string::npos){
			file = ofstream(filename, ios_base::trunc);
		}
		else{
			file = ofstream(filename + ".txt", ios_base::trunc);
		}
		
		for (int i = 0; i < kifuTxt.size(); i++){
			file << i + 1 << " " << kifuTxt[i] << endl;
		}
		goto END;
	}
	else if (yn == "n"){
		goto END;
	}
	else{
		cout << "y��n����͂��Ă��������B" << endl;
		goto HERE;
	}
END:
	cout << "������ۑ����܂����B" << endl;
	return 0;
}