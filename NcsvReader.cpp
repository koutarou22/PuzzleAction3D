#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "NcsvReader.h"

void CsvReaderYZ::Load(const std::string& path) 
{
    std::filesystem::path p = std::filesystem::current_path();//現在の作業ディレクトリを取得・設定
    std::ifstream file(path);

    //もし開いてなかったら
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
/// 特定の文字列を指定し、それがあったらリセットする
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

//CSV 行から次のパラメータを取得し、インデックスを進める
string CsvReaderYZ::PopParamStr() 
{
    if (paramIndex_ >= currentParamLine_.size())
    {
        //例外スローを返す
        throw std::out_of_range("CsvReader::PopParamStr: No more parameters to pop");
    }

    return currentParamLine_[paramIndex_++];
}

int CsvReaderYZ::PopParamInt()
{
    //intに変換して返す
    return std::stoi(PopParamStr());
}

float CsvReaderYZ::PopParamFloat()
{
    //floatに変換して返す
    return std::stof(PopParamStr());
}

bool CsvReaderYZ::PopParamBool()
{
    //取得した文字列が "1"・"true"または"TRUE" のいずれかと一致した場合trueを返す
    std::string val = PopParamStr();
    return (val == "1" || val == "true" || val == "TRUE");
}

//指定されたキーに対応する CSV 行のデータを std::vector<std::string> として返す
vector<std::string> CsvReaderYZ::GetParamLineAsStringArray(const std::string& key) const
{
    auto it = paramMap_.find(key);
    if (it != paramMap_.end()) {
        return it->second;
    }
    return {};
}