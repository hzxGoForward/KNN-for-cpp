if __name__ == "__main__":
    rf = open('TitanicData2.csv', 'r')
    wf = open('TitanicData.txt', 'w')
    num = 0;
    newline = "int32[700] memory data{} = [\n".format(num);
    line_cnt = 0
    data_len = 0
    ele_cnt = 0
    for line in rf:
        line = line.replace('\n', '').replace('\t', ' ')
        data = line.split(' ')
        
        for e in data:
            if ele_cnt + 1 == 1:
                e = e.replace(e, 'int32({})'.format(e))
                ele_cnt+= 1
                newline += e
            else:
                newline += ', '+ e
            data_len += 1
        newline += '\n'
        if(data_len % 700 == 0):
            num += 1
            ele_cnt = 0
            newline += "];\n\n int32[700] memory data{} = [\n".format(num)
        line_cnt += 1
    print("read ", line_cnt , "lines\n")
    wf.write(newline)
    wf.close()
    rf.close()