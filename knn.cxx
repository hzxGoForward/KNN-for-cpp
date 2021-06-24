#include "knn.h"

const int MaxCol = 10;
const int MaxRow = 1000;
const int MaxK = 10;
int TrainData[MaxRow][MaxCol];
int TestData[MaxRow][MaxCol];
int TrainLabels[MaxRow];
int TestLabels[MaxRow];

std::vector<std::string> mysplit(const std::string& str, const std::string &delim){
    std::vector<std::string> res;
    if("" == str)
        return res;
    std::string::size_type s = 0;
    std::string::size_type e = str.find(delim, s);
    while(e!=str.npos){
        auto ts = std::string(&str[s], e - s);
        res.push_back(ts);
        s = e + 1;
        e = str.find(delim, s);
    }
    res.push_back(std::string(&str[s], str.size() - s));
    return res;
}

void KNN::readData(const std::vector<std::string>& data)
{
    
    int idx = 0;
    for (int i = 0; i < m_test_row; i++)
    {
        std::vector<std::string> vdata = mysplit(data[idx++], "\t");
        for (int j = 0; j < m_column; j++)
        {
            TestData[i][j] = std::stoi(vdata[j]);
        }
        TestLabels[i] = std::stoi(vdata[m_column]);
    }

    for (int i = 0; i < m_train_row; i++)
    {
       std::vector<std::string> vdata = mysplit(data[idx++], "\t");
        for (int j = 0; j < m_column; j++)
        {
            TrainData[i][j] = std::stoi(vdata[j]);
        }
        TrainLabels[i]  = std::stoi(vdata[m_column]);
    }
}

void KNN::readData(const std::string& fname)
{
    std::ifstream fin;
    fin.open(fname);
    assert(fin.is_open());
    for (int i = 0; i < m_test_row; i++)
    {
        for (int j = 0; j < m_column; j++)
        {
            fin >> TestData[i][j];
        }
        fin >> TestLabels[i];
    }

    for (int i = 0; i < m_train_row; i++)
    {
        for (int j = 0; j < m_column; j++)
        {
            fin >> TrainData[i][j];
        }
        fin >> TrainLabels[i];
    }
}

void KNN::Print()
{
    // std::cout << "**************Train Data***************" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < m_column; j++)
        {
            // std::cout << TrainData[i][j] << " ";
        }
        // std::cout << std::endl;
    }

    // std::cout << "**************Test Data***************" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < m_column; j++)
        {
            // std::cout << TestData[i][j] << " ";
        }
        // std::cout << std::endl;
    }

    // std::cout << "distance = " << GetDistance(TestData[0], TrainData[0]) << std::endl;
    ;
}

int KNN::GetDistance(int Input[], int TrainData[])
{
    if (Input == nullptr)
    {
        // std::cout << "error!" << std::endl;
        return -9999;
    }

    int Distance = 0;
    for (int i = 0; i < m_column; i++)
    {
        Distance += (Input[i] - TrainData[i]) * (Input[i] - TrainData[i]);
    }

    return Distance;
}

int KNN::GetMinDistIndex(int Distance[])
{
    int Index = -1;
    int DistMin = 99;
    if (Distance == nullptr)
    {
        // std::cout << "error!" << std::endl;
        return -9999;
    }
    for (int i = 0; i < m_train_row; i++)
    {
        if (Distance[i] < DistMin && Distance[i] >= 0)
        {
            DistMin = Distance[i];
            Index = i;
        }
    }
    Distance[Index] = -1; //找出最小值后,将其置为-1
    return Index;
}

int KNN::GetMaxSeq(int LabelMinIdx[])
{
    std::map<int, int> LabelAppearTime; //key为Label值，value为出现次数
    std::map<int, int>::iterator iter;
    for (int i = 0; i < m_k; i++)
    {
        iter = LabelAppearTime.find(TrainLabels[LabelMinIdx[i]]);
        if (iter != LabelAppearTime.end())
            iter->second++;
        else
        {
            LabelAppearTime.insert(std::pair<int, int>(TrainLabels[LabelMinIdx[i]], 1));
        }
    }

    int LabelMaxSeq = -1;
    int times = 0;
    for (iter = LabelAppearTime.begin(); iter != LabelAppearTime.end(); iter++)
    {
        if (iter->second > times)
        {
            times = iter->second;
            LabelMaxSeq = iter->first;
        }
    }
    return LabelMaxSeq;
}

int KNN::Classify(int Input[])
{
    //Column是数据特征变量的个数
    //Row是DataSet的输入向量的长度
    /*
  算法的基本思想是，计算Input向量与DataSet每个向量的距离，并用一个数组Distance储存。
  找出Distance中的最小的前k个值，在Labels向量中找出对应下标并记录对应Labels值
  找出记录下的Labels值中，出现频率最高的作为返回值。

  */

    int Distance[MaxRow];
    for (int i = 0; i < m_train_row; i++)
    {
        Distance[i] = GetDistance(Input, TrainData[i]);
    }

    int LabelMinIdx[MaxK];
    for (int i = 0; i < m_k; i++)
    {
        LabelMinIdx[i] = GetMinDistIndex(Distance); //返回的是Label下标
    }
    return GetMaxSeq(LabelMinIdx);
}

int KNN::classify_all(int test_row)
{
    int CorrectNum = 0;
    for (int i = 0; i < test_row; ++i)
    {
        if (Classify(TestData[i]) == TestLabels[i])
            CorrectNum++;
    }
    return CorrectNum;
}

int KNN::CorrectRate()
{
    // 对于每个TestData，利用Classify获得LabelsPredict，再和TestData的真实Label计算正确率
    int CorrectNum = classify_all(m_test_row);
    // std::cout << "correctNum: " << CorrectNum << " m_test_row: " << m_test_row << std::endl;
    int CorrectRate = (CorrectNum * 100) / m_test_row;
    std::cout << "m_test_row = " << m_test_row << "\n";
    std::cout << "CorrectNum = " << CorrectNum << "\n";
    std::cout << "CorrectRate = " << CorrectRate << "%\n";
    m_result = CorrectRate;
    return 0;
}

int main(int argc, char **argv)
{
    int k, row, col;
    std::cout << "Wo...\n";
    char *FileName;
    if (argc != 5)
    {
        std::cout << "The input should be like this : ./a.out k row col filename" << std::endl;
        exit(1);
    }
    k = atoi(argv[1]);
    row = atoi(argv[2]);
    col = atoi(argv[3]);
    FileName = argv[4];

    //KNN *k = new KNN(7,FileName,1000,3);

    std::ifstream fs(FileName, std::ios::in);
    std::vector<std::string> data;
    
    while (!fs.eof() && data.size()<row)
    {
        std::string str;
        getline(fs, str, '\n');
        data.push_back(str);
    }
    std::cout << "read " << data.size() << " rows\n";
    KNN *knn = new KNN(k, data, row, col);
    knn->CorrectRate();
    std::cout << knn->get_train_result();

    return 0;
}