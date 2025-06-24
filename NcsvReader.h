#pragma once

#include<string>
#include<Vector>
#include <unordered_map>

using std::vector;
using std::string;

using std::unordered_map;

//�uunordered_map�v�Ƃ�
//���O�Ɠ��e���ꏏ�ɊǗ����锠�̂悤�Ȃ��́@�A�z�z��

//���� 
//�@���Ԃ͋C�ɂ��Ȃ�
//�A�������O��2��o�^���邱�Ƃ͂ł��Ȃ�


class CsvReaderYZ
{
public:
	//csv��ǂݍ��ޗp
	void Load(const string& path);

	//paramMap_���猟�����A��������CurrenParamLine_�Ɋi�[����
	bool GetParamLine(const string& key);

	//�l���擾�E�ϊ��p
	string   PopParamStr();  //csv���當������擾����
	int      PopParamInt();  //csv���琮�����擾����
	float    PopParamFloat();//csv���畂�������_���擾����
	bool     PopParamBool(); //csv����^�U���擾����

	//�ꊇ�Ŏ擾����A�Ȃ���΋��vector��Ԃ�
	vector<std::string> GetParamLineAsStringArray(const std::string& key) const;

private:
	std::unordered_map<std::string, std::vector<std::string>> paramMap_;//�X�e�[�W���Ƃ��̏����֘A�t����
	std::vector<std::string> currentParamLine_;//���ݑ��쒆�̃f�[�^��ێ�����
	size_t paramIndex_ = 0;//���݂̑��쒆�̈ʒu������

};

