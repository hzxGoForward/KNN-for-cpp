
#include <iostream>
#include <math.h>
#include <map>
#include <cassert>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
const int test_number = 10;
std::vector<std::string> mysplit(const std::string &str, const std::string &delim);
class KNN
{
private:
    uint32_t m_k;
    uint32_t m_train_row;
    uint32_t m_test_row;
    uint32_t m_column;
    
    int m_result;

public:
    KNN(uint32_t k, char *FileName, uint32_t Row, uint32_t Column, uint32_t Number = test_number) : m_k(k), m_column(Column)
    {
        assert(Number > 0 && Number < Row);
        m_train_row = Row - Number;
        m_test_row = Number;
        readData(FileName);
        // AutoNorm();
    }

    KNN(std::vector<int>& data, int k, int Column, int test_row, int train_row){
        m_k = k;
        m_column = Column;
        m_train_row = train_row;
        m_test_row = test_row;
        readData(data);
    }

    int Classify(std::vector<int> &Input);
    int GetMaxSeq(std::vector<int>& LabelMinIdx);
    int GetMinDistIndex(std::vector<int>& Distance);
    int GetDistance(std::vector<int>& Input, std::vector<int>& Traindata, int i);
    int CorrectRate();
    void readData(const std::string& fname);
    void readData(const std::vector<int> &data);
    std::string get_train_result() const{
        return std::to_string(m_result);
    }

protected:
    std::vector<int> m_TrainData;
    std::vector<int> m_TestData;
    std::vector<int> m_Trainlabels;
    std::vector<int> m_TestLabels;
};