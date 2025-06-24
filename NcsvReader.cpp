#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "NcsvReader.h"

void CsvReaderYZ::Load(const std::string& path) 
{
    std::filesystem::path p = std::filesystem::current_path();//���݂̍�ƃf�B���N�g�����擾�E�ݒ�
    std::ifstream file(path);

    //�����J���ĂȂ�������
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file: " + path);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key;
        std::getline(iss, key, ',');

        std::vector<std::string> values;
        std::string token;
        while (std::getline(iss, token, ',')) 
        {
            values.push_back(token);
        }

        paramMap_[key] = std::move(values);
    }
}

/// <summary>
/// ����̕�������w�肵�A���ꂪ�������烊�Z�b�g����
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool CsvReaderYZ::GetParamLine(const std::string& key)
{
    auto it = paramMap_.find(key);
    if (it != paramMap_.end()) {
        currentParamLine_ = it->second;
        paramIndex_ = 0;
        return true;
    }
    return false;
}

//CSV �s���玟�̃p�����[�^���擾���A�C���f�b�N�X��i�߂�
string CsvReaderYZ::PopParamStr() 
{
    if (paramIndex_ >= currentParamLine_.size())
    {
        //��O�X���[��Ԃ�
        throw std::out_of_range("CsvReader::PopParamStr: No more parameters to pop");
    }

    return currentParamLine_[paramIndex_++];
}

int CsvReaderYZ::PopParamInt()
{
    //int�ɕϊ����ĕԂ�
    return std::stoi(PopParamStr());
}

float CsvReaderYZ::PopParamFloat()
{
    //float�ɕϊ����ĕԂ�
    return std::stof(PopParamStr());
}

bool CsvReaderYZ::PopParamBool()
{
    //�擾���������� "1"�E"true"�܂���"TRUE" �̂����ꂩ�ƈ�v�����ꍇtrue��Ԃ�
    std::string val = PopParamStr();
    return (val == "1" || val == "true" || val == "TRUE");
}

//�w�肳�ꂽ�L�[�ɑΉ����� CSV �s�̃f�[�^�� std::vector<std::string> �Ƃ��ĕԂ�
vector<std::string> CsvReaderYZ::GetParamLineAsStringArray(const std::string& key) const
{
    auto it = paramMap_.find(key);
    if (it != paramMap_.end()) {
        return it->second;
    }
    return {};
}