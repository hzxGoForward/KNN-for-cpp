#include "knn.h"
#include <vector>

std::vector<std::string> mysplit(const std::string &str, const std::string &delim)
{
    std::vector<std::string> res;
    if ("" == str)
        return res;
    std::string::size_type s = 0;
    std::string::size_type e = str.find(delim, s);
    while (e != str.npos)
    {
        auto ts = std::string(&str[s], e - s);
        res.push_back(ts);
        s = e + 1;
        e = str.find(delim, s);
    }
    res.push_back(std::string(&str[s], str.size() - s));
    return res;
}

void KNN::readData(const std::vector<int> &data)
{
    
    static int test_train = 0;
    static int test_test = 0;

    m_TrainData.resize(m_train_row * m_column, 0);
    m_TestData.resize(m_test_row * m_column, 0);
    m_Trainlabels.resize(m_train_row, 0);
    m_TestLabels.resize(m_test_row, 0);

    int i = 0;
    int test_data_idx = 0;
    int test_label_idx = 0;
    // reading test data
    while(i < data.size())
    {
        for (int j = 0; j < m_column; ++j)
            m_TestData[test_data_idx++] = data[i++];
        m_TestLabels[test_label_idx++] = data[i++];
        if( test_label_idx >= m_test_row)
            break;
    }

    {
        std::cout << "TestData....\n";
        int output_cnt = 0;
        
        for (auto&e: m_TestData)
        {
            std::cout << e << " ";
            ++output_cnt;
            if(output_cnt % m_column == 0)
                std::cout << std::endl;
        }
        std::cout << std::endl
                    << "TestLabel....\n";
        for (auto &e : m_TestLabels)
            std::cout << e << " ";
        std::cout << std::endl;
    }

    
    int train_data_idx = 0;
    int train_label_idx = 0;
    // reading train data
    while(i < data.size())
    {
        for (int j = 0; j < m_column; ++j)
            m_TrainData[train_data_idx++] = data[i++];
        m_Trainlabels[train_label_idx++] = data[i++];
        if( train_label_idx >= m_train_row)
            break;
    }


    {
        std::cout << "TrainData....\n";
        int output_cnt = 0;
        
        for (auto&e: m_TrainData)
        {
            std::cout << e << " ";
            ++output_cnt;
            if(output_cnt % m_column == 0)
                std::cout << std::endl;
        }
        std::cout << std::endl
                    << "TrainLabel....\n";
        for (auto &e : m_Trainlabels)
            std::cout << e << " ";
        std::cout << std::endl;
    }
    
}

int KNN::GetDistance(std::vector<int>& Input, std::vector<int>& Traindata, int i)
{
    int Dist = 0;
    for (int k = 0; k < Input.size(); ++k)
    {
        int diff = Input[k] - Traindata[i++];
        diff = diff * diff;
        Dist += diff;
    }
    return Dist;
}

int KNN::GetMinDistIndex(std::vector<int> &Distance)
{
    int idx = -1;
    if (Distance.empty())
        return idx;
    idx = 0;
    int DistMin = 999999;
    for (int i = 0; i < Distance.size(); i++)
    {
        if (Distance[i] < DistMin && Distance[i] >= 0)
        {
            DistMin = Distance[i];
            idx = i;
        }
    }
    return idx;
}

int KNN::GetMaxSeq(std::vector<int> &LabelMinIdx)
{
    // std::map<uint32_t, uint32_t> LabelAppearTime; //key为Label值，value为出现次数
    std::vector<uint32_t> LabelAppearTime(m_k);
    uint32_t times = 0;
    int LabelMaxSeq = -1;
    for (uint32_t i = 0; i < m_k; ++i)
    {
        // 找到第 i 个点的索引
        int label = m_Trainlabels[LabelMinIdx[i]];
        if (++LabelAppearTime[label] > times)
        {
            times = LabelAppearTime[label];
            LabelMaxSeq = label;
        }
    }
    assert(LabelMaxSeq >= 0);
    return LabelMaxSeq;
}

int KNN::Classify(std::vector<int> &Input)
{
    //Column是数据特征变量的个数
    //Row是DataSet的输入向量的长度
    /*
  算法的基本思想是，计算Input向量与DataSet每个向量的距离，并用一个数组Distance储存。
  找出Distance中的最小的前k个值，在Labels向量中找出对应下标并记录对应Labels值
  找出记录下的Labels值中，出现频率最高的作为返回值。

  */

    std::vector<int> Distance(m_train_row, 9999);
    for (int i = 0; i < m_train_row; i++)
    {
        Distance[i] = 9999;
        int param = i * m_column;
        Distance[i] = GetDistance(Input, m_TrainData, param);
    }

    std::vector<int> LabelMinIdx(m_k, -1);
    for (int i = 0; i < m_k; i++)
    {
        LabelMinIdx[i] = GetMinDistIndex(Distance); //返回的是Label下标
        Distance[LabelMinIdx[i]] = 9999;
    }
    return GetMaxSeq(LabelMinIdx);
}

int KNN::CorrectRate()
{
    // 对于每个TestData，利用Classify获得LabelsPredict，再和TestData的真实Label计算正确率
    int CorrectNum = 0;
    for (int i = 0; i < m_test_row; ++i)
    {
        std::vector<int> Input(m_column, 0);
        for (int x = i * m_column, idx = 0; x < Input.size(); ++x)
            Input[idx++] = m_TestData[x];
        if (Classify(Input) == m_TestLabels[i])
            CorrectNum++;
    }
    int CorrectRate = (CorrectNum * 100) / m_test_row;
    std::cout << "m_train_row = " << m_train_row << "\n";
    std::cout << "m_test_row = " << m_test_row << "\n";
    std::cout << "CorrectNum = " << CorrectNum << "\n";
    std::cout << "CorrectRate = " << CorrectRate << "%\n";
    m_result = CorrectRate;
    return 0;
}

int main(int argc, char **argv)
{
    int k, col, test_row, test_col;
    std::cout << "Wo...\n";
    char *FileName;
    if (argc != 5)
    {
        std::cout << "The input should be like this : ./a.out k row col filename" << std::endl;
        exit(1);
    }
    k = atoi(argv[1]);
    col = atoi(argv[2]);
    test_row = atoi(argv[3]);
    test_col = atoi(argv[4]);
    FileName = "TitanicData2.csv";

    std::ifstream fs(FileName, std::ios::in);
    std::vector<int> data;

    while (!fs.eof() && data.size() < (test_row+test_col) * (col + 1))
    {
        int tmp;
        fs >> tmp;
        data.push_back(tmp);
    }
    std::cout << "read " << data.size() << " rows\n";

    {
        std::cout << "first 5 rows\n";
        for (int i = 0; i < 5 * (col + 1); ++i)
        {
            std::cout << data[i] << " ";
            if(i >0 && (i+1)%(col+1) ==0)
                std::cout << std::endl;
        }
    }

    KNN knn(data, k, col, test_row, test_col);
    knn.CorrectRate();
    std::cout << knn.get_train_result();

    return 0;
}