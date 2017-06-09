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
koma banmen[9][9], prevbanmen[9][9];//　最新の盤面　直前の盤面
int misscount = 0;
//-----------------------------------------------
//盤面画像生成時に使う数字たちの配列
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
//駒の画像と漢字と向きをセットにした配列作成、盤面画像生成時に使う駒の漢字画像の配列作成
void importKoma(){ 
	komaDates.push_back(komaDate(imread("./image/koma/fu1.png"), "歩", 1));
	komaDates.push_back(komaDate(imread("./image/koma/fu2.png"), "歩", 2));
	komaDates.push_back(komaDate(imread("./image/koma/gin1.png"), "銀", 1));
	komaDates.push_back(komaDate(imread("./image/koma/gin2.png"), "銀", 2));
	komaDates.push_back(komaDate(imread("./image/koma/gyoku2.png"), "玉", 2));
	komaDates.push_back(komaDate(imread("./image/koma/hisya1.png"), "飛", 1));
	komaDates.push_back(komaDate(imread("./image/koma/hisya2.png"), "飛", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kaku1.png"), "角", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kaku2.png"), "角", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kei1.png"), "桂", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kei2.png"), "桂", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kin1.png"), "金", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kin2.png"), "金", 2));
	komaDates.push_back(komaDate(imread("./image/koma/kyo1.png"), "香", 1));
	komaDates.push_back(komaDate(imread("./image/koma/kyo2.png"), "香", 2));
	komaDates.push_back(komaDate(imread("./image/koma/narigin1.png"), "全", 1));
	komaDates.push_back(komaDate(imread("./image/koma/narigin2.png"), "全", 2));
	komaDates.push_back(komaDate(imread("./image/koma/narikei1.png"), "圭", 1));
	komaDates.push_back(komaDate(imread("./image/koma/narikei2.png"), "圭", 2));
	komaDates.push_back(komaDate(imread("./image/koma/narikyo1.png"), "杏", 1));
	komaDates.push_back(komaDate(imread("./image/koma/narikyo2.png"), "杏", 2));
	komaDates.push_back(komaDate(imread("./image/koma/ou1.png"), "王", 1));
	komaDates.push_back(komaDate(imread("./image/koma/ryu1.png"), "龍", 1));
	komaDates.push_back(komaDate(imread("./image/koma/ryu2.png"), "龍", 2));
	komaDates.push_back(komaDate(imread("./image/koma/to1.png"), "と", 1));
	komaDates.push_back(komaDate(imread("./image/koma/to2.png"), "と", 2));
	komaDates.push_back(komaDate(imread("./image/koma/uma1.png"), "馬", 1));
	komaDates.push_back(komaDate(imread("./image/koma/uma2.png"), "馬", 2));
	komaImgs["歩"] = imread("./image/kanji/fu.png");
	komaImgs["角"] = imread("./image/kanji/kaku.png");
	komaImgs["飛"] = imread("./image/kanji/hisya.png");
	komaImgs["と"] = imread("./image/kanji/to.png");
	komaImgs["王"] = imread("./image/kanji/ou.png");
	komaImgs["玉"] = imread("./image/kanji/gyoku.png");
	komaImgs["桂"] = imread("./image/kanji/kei.png");
	komaImgs["香"] = imread("./image/kanji/kyo.png");
	komaImgs["金"] = imread("./image/kanji/kin.png");
	komaImgs["銀"] = imread("./image/kanji/gin.png");
	komaImgs["全"] = imread("./image/kanji/narigin.png");
	komaImgs["圭"] = imread("./image/kanji/narikei.png");
	komaImgs["杏"] = imread("./image/kanji/narikyo.png");
	komaImgs["馬"] = imread("./image/kanji/uma.png");
	komaImgs["龍"] = imread("./image/kanji/ryu.png");
	komaImgs["("] = imread("./image/kanji/kakko.png");
	komaImgs[")"] = imread("./image/kanji/kokka.png");
	komaImgs["▲"] = imread("./image/kanji/kurosankaku.png");
	komaImgs["△"] = imread("./image/kanji/sirosankaku.png");
	komaImgs["不"] = imread("./image/kanji/zu.png");
	komaImgs["成"] = imread("./image/kanji/nari.png");
	komaImgs["打"] = imread("./image/kanji/utu.png");
	komaImgs["取"] = imread("./image/kanji/tori.png");
}

//------------------------------------------------
//駒の動きが正しいかの判別
//基本的に移動前と移動後の座標を比較してパターンに合うときはtrue それ以外はfalseを返すようにしている
//飛、龍、角、馬、香は例外
bool komaMovement(Point before, Point after, string koma,int dir){
	//成った時金の挙動になる奴ら
	
	if (koma == "圭" || koma == "杏" || koma == "全" || koma == "金" || koma == "と" ){
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
	else if(koma == "歩"){
		if (before.x == after.x){
			if (dir == 1){
				if (before.y == after.y + 1)return true;
			}
			else{
				if (before.y == after.y - 1)return true;
			}
		}
	}
	else if (koma == "香"){
		if (dir == 2){
			if (before.x == after.x){
				for (int i = before.y + 1; i < after.y; i++){
					if (banmen[i][before.x].type != "　"){ 
						return false; 
					}
				}
				return true;
			}
		}
		else{
			if (before.x == after.x){
				for (int i = before.y - 1; i > after.y; i--){
					if (banmen[i][before.x].type != "　"){
						return false;
					}
				}
				return true;
			}
		}
	}
	else if (koma == "桂"){
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
	else if (koma == "銀"){
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
	else if (koma == "角"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px, py;
		if (abs(dx) == abs(dy)){
			px = dx < 0 ? -1 : 1;
			py = dy < 0 ? -1 : 1;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[before.y + i*py][before.x + i*px].type != "　"){
					return false;
				}
			}
			return true;
		}
	}
	else if (koma == "馬"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px, py;
		if (abs(dx) == abs(dy)){
			px = dx < 0 ? -1 : 1;
			py = dy < 0 ? -1 : 1;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[before.y + i*dy][before.x + i*dx].type != "　"){
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
	else if (koma == "飛"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px = dx < 0 ? -1 : 1;
		int py = dy < 0 ? -1 : 1;
		if (before.x == after.x && before.y != after.y){
			int x = before.x;
			for (int i = 1; i < abs(dy); i++){
				if (banmen[before.y + i*py][x].type != "　")return false;
			}
			return true;
		}
		else if (before.x != after.x && before.y == after.y){
			int y = before.y;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[y][before.x + i*px].type != "　")return false;
			}
			return true;
		}
	}
	else if (koma == "龍"){
		int dx = after.x - before.x;
		int dy = after.y - before.y;
		int px = dx < 0 ? -1 : 1;
		int py = dy < 0 ? -1 : 1;
		if (before.x == after.x && before.y != after.y){
			int x = before.x;
			for (int i = 1; i < abs(dy); i++){
				if (banmen[before.y + i*py][x].type != "　")return false;
			}
			return true;
		}
		else if (before.x != after.x && before.y == after.y){
			int y = before.y;
			for (int i = 1; i < abs(dx); i++){
				if (banmen[y][before.x + i*px].type != "　")return false;
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
	else if (koma == "王" || koma == "玉"){
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
//現在の映像から盤面の切り出し
//角の座標を引数として受け取る→受け取った時点ではどれがどの角なのかわからないから中心座標との比較で決める。
//盤面の縦横の大きさを２頂点の距離から計算する（縦横で異なるから注意）
//ここでマスの大きさも計算しておく。
//計算した縦横の大きさのMatを生成してそこに切り抜いた画像を貼り付ける。

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
//盤面配列の更新＋盤面配列データの出力
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
//盤面配列の更新のみ
void dontoutputBoard(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			prevbanmen[i][j] = banmen[i][j];
		}
	}
}

//----------------------------------------------
//盤面配列の更新をせずに盤面配列のデータのみを出力
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
//盤面配列の初期化
void init(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			banmen[i][j].dir = 0;
			banmen[i][j].type = "　";
		}
	}
}

//----------------------------------------------
//変化部分のカウント
//駒の種類、向きのどちらかが一致しなかったらその座標と現在そこにある駒の情報をgapに追加していく。
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
//盤面配列にデータを入れる
//テンプレートマッチングで検出した駒の中心座標をマスの縦横の大きさで割ることで盤面上のどのマスの領域に駒の中心があるのかを計算して盤面配列に格納する。
void setToBoardDatebase(int x, int y, string komaname, int dir){
	int pos_x = x / masuWidth;
	int pos_y = y / masuHeight;
	banmen[pos_y][pos_x].type = komaname;
	banmen[pos_y][pos_x].dir = dir;
}

//----------------------------------------------
//テンプレートマッチング
//駒の親データを切り抜くときに使った盤面画像の横が530pxだったので、盤面の横の長さ/530を駒の親データの縦横に掛けた大きさに拡大縮小する。
//あとは-6 ~ 6までを3度毎に傾けてテンプレートマッチングする。
//しきい値(threshhold)を駒の種類ごとに決めて、超えたところの座標をメモ
//メモを最初から見ていき盤面配列にデータを格納する。
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
		if (tmpimg.second == "と"){
			threshhold = 0.99f;
		}
		if (tmpimg.second == "全" || tmpimg.second == "圭"){
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
//盤面上に存在する駒の個数を数える
int countKomas(koma b[9][9]){
	int count = 0;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (b[i][j].type != "　"){
				count++;
			}
		}
	}
	return count;
}

//----------------------------------------------
//成か不成かの判別
//相手の領域内での移動、相手の領域への侵入、相手の領域からの脱出の３パターンに分けてどれかのパターンに当てはまるときは成の条件を満たすとみなしている。
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
//すでに成っているかどうかの判別
bool isNaried(string s){
	if (s == "と" || s == "龍" || s == "馬" || s == "全" || s == "圭" || s == "杏"){
		return true;
	}
	return false;
}

string changeToNormal(string s){
	if (s == "と"){
		return "歩";
	}
	else if (s == "龍"){
		return "飛";
	}
	else if (s == "馬"){
		return "角";
	}
	else if (s == "全"){
		return "銀";
	}
	else if (s == "圭"){
		return "桂";
	}
	else if (s == "杏"){
		return "香";
	}
}

//----------------------------------------------
//棋譜の出力
//これ割とややこしいから説明したくない
//簡潔に言うと消去法でゴリ押し
bool firstMove = true;
void outputKifu(int abso, vector<pair<pair<int, int>, string> > gaps){
	vector<Mat> kifu;
	string kifutxt = "";
	string kanjinums[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
	cout << (turnDir == sente ? "▲" : "△");
	kifutxt = kifutxt + (turnDir == sente ? "▲" : "△");
	kifu.push_back(komaImgs[turnDir == sente ? "▲" : "△"]);
	if (gaps.size() == 1 && abso == 1){
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << gaps[0].second << "打" << endl;
		kifutxt = kifutxt + to_string(9 - gaps[0].first.second ) + kanjinums[gaps[0].first.first] + gaps[0].second + "打";
		kifu.push_back(num[gaps[0].first.second]);
		kifu.push_back(nums[gaps[0].first.first]);
		kifu.push_back(komaImgs[gaps[0].second]);
		kifu.push_back(komaImgs["打"]);
	}
	else if (gaps.size() == 2 && abso == -1) {
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma b1 = banmen[gaps[1].first.first][gaps[1].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		koma p1 = prevbanmen[gaps[1].first.first][gaps[1].first.second];
		if (gaps[0].second != "　"){
			kifu.push_back(num[gaps[0].first.second]);
			kifu.push_back(nums[gaps[0].first.first]);
			if (p1.type != b0.type){
				cout << 9 - gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "成(" << p0.type << "取)" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "成(" + p0.type + "取)";
				kifu.push_back(komaImgs[changeToNormal(gaps[0].second)]);
				kifu.push_back(komaImgs["成"]);
				kifu.push_back(komaImgs["("]);
				kifu.push_back(komaImgs[p0.type]);
				kifu.push_back(komaImgs["取"]);
				kifu.push_back(komaImgs[")"]);
			}
			else{
				Point move(gaps[1].first.first, gaps[0].first.first);
				if (narazu(move, p1.dir) && !isNaried(gaps[0].second)){
					cout << 9 - gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "不成(" << p0.type << "取)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "不成(" + p0.type + "取)";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["不"]);
					kifu.push_back(komaImgs["成"]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p0.type]);
					kifu.push_back(komaImgs["取"]);
					kifu.push_back(komaImgs[")"]);
				}
				else{
					cout << 9 - gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "(" << p0.type << "取)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "(" + p0.type + "取)";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p0.type]);
					kifu.push_back(komaImgs["取"]);
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
				cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << p0.type << "成(" << p1.type << "取)" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "成(" + p1.type + "取)";
				kifu.push_back(komaImgs[changeToNormal(gaps[1].second)]);
				kifu.push_back(komaImgs["成"]);
				kifu.push_back(komaImgs["("]);
				kifu.push_back(komaImgs[p1.type]);
				kifu.push_back(komaImgs["取"]);
				kifu.push_back(komaImgs[")"]);
			}
			else{
				Point move(gaps[0].first.first, gaps[1].first.first);
				if (narazu(move, p0.dir) && !isNaried(gaps[1].second)){
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second << "不成(" << p1.type << "取)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "不成(" + p1.type + "取)";
					kifu.push_back(komaImgs[gaps[1].second]);
					kifu.push_back(komaImgs["不"]);
					kifu.push_back(komaImgs["成"]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p1.type]);
					kifu.push_back(komaImgs["取"]);
					kifu.push_back(komaImgs[")"]);
				}
				else{
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second << "(" << p1.type << "取)" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "(" + p1.type + "取)";
					kifu.push_back(komaImgs[gaps[1].second]);
					kifu.push_back(komaImgs["("]);
					kifu.push_back(komaImgs[p1.type]);
					kifu.push_back(komaImgs["取"]);
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
		if (gaps[0].second != "　"){
			kifu.push_back(num[gaps[0].first.second]);
			kifu.push_back(nums[gaps[0].first.first]);
			
			if (p1.type != b0.type){
				cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << p1.type << "成" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "成";
				kifu.push_back(komaImgs[changeToNormal(gaps[0].second)]);
				kifu.push_back(komaImgs["成"]);
			}
			else{
				Point move(gaps[1].first.first, gaps[0].first.first);
				if (narazu(move, p1.dir) && !isNaried(gaps[0].second)){
					cout << 9-gaps[0].first.second << "" << kanjinums[gaps[0].first.first] << "" << gaps[0].second << "不成" << endl;
					kifutxt = kifutxt + to_string(9 - gaps[0].first.second) + "" + kanjinums[gaps[0].first.first] + "" + gaps[0].second + "不成";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["不"]);
					kifu.push_back(komaImgs["成"]);
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
				cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << p0.type << "成" << endl;
				kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "成";
				kifu.push_back(komaImgs[changeToNormal(gaps[0].second)]);
				kifu.push_back(komaImgs["成"]);
			}
			else{
				Point move(gaps[0].first.first, gaps[1].first.first);
				if (narazu(move, p0.dir) && !isNaried(gaps[1].second)){
					cout << 9-gaps[1].first.second << "" << kanjinums[gaps[1].first.first] << "" << gaps[1].second <<"不成"<< endl;
					kifutxt = kifutxt + to_string(9 - gaps[1].first.second) + "" + kanjinums[gaps[1].first.first] + "" + gaps[1].second + "不成";
					kifu.push_back(komaImgs[gaps[0].second]);
					kifu.push_back(komaImgs["不"]);
					kifu.push_back(komaImgs["成"]);
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
//盤面画像の作成
void makeBoardImg(){
	Mat outputBoardImg(300, 500, CV_8UC3, Scalar(255, 255, 255));
	Mat subdst;
	//縦線横線の描画
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
	//駒たちの描画
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (banmen[i][j].type != "　"){
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
//ルールにそっているかどうかチェック
//二歩、違法な移動を検知できる
bool checkRules(vector<pair<pair<int, int>, string> > gaps, int komaGap){
	pair<int, int> p = make_pair(gaps.size(), komaGap);
	if (p == make_pair(1, 1)){
		vector<pair<int,int> > fupos;
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		if (b0.dir != turnDir && !firstMove){
			P("二手指しです。");
			return false;
		}
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				if (banmen[i][j].type == "歩"){
					for (int k = 0; k < 9; k++){
						if (k != i){
							if (banmen[k][j].type == "歩" && (banmen[k][j].dir == banmen[i][j].dir)){
								P("二歩です。");
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
			P("手駒にありません。");
			return false;
		}
		else{
			for (auto it = Aite.begin(); it != Aite.end(); it++){
				if (it->type == b0.type){
					Aite.erase(it);
					return true;
				}
			}
			P("手駒にありません。");
			return false;
		}
		return true;
	}
	else if (p == make_pair(2, -1)){
		koma b0 = banmen[gaps[0].first.first][gaps[0].first.second];
		koma b1 = banmen[gaps[1].first.first][gaps[1].first.second];
		koma p0 = prevbanmen[gaps[0].first.first][gaps[0].first.second];
		koma p1 = prevbanmen[gaps[1].first.first][gaps[1].first.second];

		if (gaps[0].second != "　"){
			Point before(gaps[1].first.second, gaps[1].first.first), after(gaps[0].first.second, gaps[0].first.first);
			//
			if (banmen[gaps[0].first.first][gaps[0].first.second].dir != turnDir && !firstMove){
				P("二手指しです。");
				return false;
			}
			if (!komaMovement(before, after, p1.type, b0.dir)){
				P("不正な動きです。");
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
				P("二手指しです。");
				return false;
			}
			if (!komaMovement(before, after, p0.type, b1.dir)){
				P("不正な動きです。");
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
		if (gaps[0].second != "　"){
			Point before(gaps[1].first.second, gaps[1].first.first), after(gaps[0].first.second, gaps[0].first.first);
			if (banmen[gaps[0].first.first][gaps[0].first.second].dir != turnDir && !firstMove){
				P("二手指しです。");
				return false;
			}
			if (!komaMovement(before, after, p1.type, b0.dir)){
				P("不正な動きです。");
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
				P("二手指しです。");
				return false;
			}
			if (!komaMovement(before, after, p0.type, b1.dir)){
				P("不正な動きです。");
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
//メイン
//流れだけ
//webカメからフレーム取得→GS→ノイズ除去→Canny→輪郭取得→最大領域輪郭選択→正しかったら１個目のwhile文から脱出→(*)盤面切り抜き→GS→テンプレートマッチング→合法な移動かどうか判別↓
//→正しかったら棋譜を出力して直前盤面配列に最新盤面配列をコピーする→以下(*)以降をループ・・・
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
		vector<vector<Point> > contours;//輪郭を座標の配列の配列で保持
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
			cout << "変化部分:" << gaps.size() << " " << "駒の増減:" << komaGap << endl;
		}
		if (p == make_pair(1, 1) || p == make_pair(2,-1) || p == make_pair(2,0) || firstTime){
			
			if(firstTime){
				P("対局開始です。(ALTキーで終了)");
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
	cout << "棋譜を保存しますか？(y/n):";
	string yn;
	cin >> yn;
	if (yn == "y"){
		cout << "------既存の棋譜ファイル------"<< endl;
		system("ls | grep txt");
		cout << "ファイル名を入力してください(既存の名前にした場合上書きされます):";
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
		cout << "yかnを入力してください。" << endl;
		goto HERE;
	}
END:
	cout << "棋譜を保存しました。" << endl;
	return 0;
}