#pragma once

#include<string>
#include<Vector>
#include <unordered_map>

using std::vector;
using std::string;

using std::unordered_map;

//「unordered_map」とは
//名前と内容を一緒に管理する箱のようなもの　連想配列

//特徴 
//①順番は気にしない
//②同じ名前を2回登録することはできない


class CsvReaderYZ
{
public:
	//csvを読み込む用
	void Load(const string& path);

	//paramMap_から検索し、見つけたらCurrenParamLine_に格納する
	bool GetParamLine(const string& key);

	//値を取得・変換用
	string   PopParamStr();  //csvから文字列を取得する
	int      PopParamInt();  //csvから整数を取得する
	float    PopParamFloat();//csvから浮動小数点を取得する
	bool     PopParamBool(); //csvから真偽を取得する

	//一括で取得する、なければ空のvectorを返す
	vector<std::string> GetParamLineAsStringArray(const std::string& key) const;

private:
	std::unordered_map<std::string, std::vector<std::string>> paramMap_;//ステージ名とその情報を関連付ける
	std::vector<std::string> currentParamLine_;//現在操作中のデータを保持する
	size_t paramIndex_ = 0;//現在の操作中の位置を示す

};

