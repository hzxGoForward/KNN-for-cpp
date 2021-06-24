
#include <iostream>
#include <math.h>
#include <map>
#include <cassert>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> mysplit(const std::string &str, const std::string &delim);
class KNN
{
private:
    int m_k;
    int m_train_row;
    int m_test_row;
    int m_column;
    int m_result;

public:
    KNN(int k, char *FileName, int Row, int Column, int Number = 91) : m_k(k), m_column(Column)
    {
        assert(Number > 0 && Number < Row);
        m_train_row = Row - Number;
        m_test_row = Number;
        readData(FileName);
        // AutoNorm();
    }

    KNN(int k, std::vector<std::string>& data, int Row, int Column, int Number = 91):m_k(k), m_column(Column){
        assert(Number > 0 && Number < Row);
        m_train_row = Row - Number;
        m_test_row = Number;
        readData(data);
    }

    int Classify(int Input[]);
    int classify_all(int test_row);
    int GetMaxSeq(int LabelMinIdx[]);
    int GetMinDistIndex(int Distance[]);
    int GetDistance(int[], int[]);
    int CorrectRate();
    void readData(const std::string& fname);
    void readData(const std::vector<std::string> &data);
    void AutoNorm();
    void Print();
    std::string get_train_result() const{
        return std::to_string(m_result);
    }
};