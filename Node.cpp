#include<sys/stat.h>
#include<iostream>
#include<filesystem>
#include<string>
#include<cstring>
#include<fstream>
#include<unistd.h>
using namespace std;
//struct of node
struct Node{
    string *megaData;
    string *p0;
    string *p1;
    string *p2;
    string *p3;
    string *p4;
    string *p5;
    string *p6;
    string *p7;
    string *p8;
    string *p9;
    string *p10;
    string *p11;
    string **p12;
    string ***p13;
};
int main(int argc, char ** argv)
{
    if(argc != 3)
    {
    	cout << "invalid number of arguments. arguments must be 3" << endl;
    	return -2;
    }
    char path[1024];
    string fullpath;
    string s;
    if (getcwd(path, sizeof(path)) != NULL) {
        s = path;
    }
    const char* subDir1 = "input_file1_dir";
    int status = mkdir(subDir1, 0777);
    //object of node
    Node n;
    n.megaData = new string(argv[1]);
    //assigning memory to indirect block
    n.p12 = new string*[100];
    n.p13 = new string**[100];
    for(int i = 0; i < 100; i++)
    {
        n.p13[i] = new string *[100];
    }
    //array to store names for direct blocks
    string file_names_for_direct_blocks[] = {"zero.txt", "one.txt", "two.txt", "three.txt", "four.txt", "five.txt", "six.txt", "seven.txt", "eight.txt", "nine.txt", "ten.txt", "eleven.txt"};
    //array for checking of valid block numbers
    bool validBlockNum[10112];
    //assigning it to false
    for(int i = 0; i < 10112; i++)
    {
        validBlockNum[i] = 0;
    }
    struct stat st;
    stat(argv[1], &st);
    off_t filesize = st.st_size;
    off_t maxfilesize = 1024 * 1024 * 1024; // 1 GB
    if (filesize > maxfilesize) {
        std::cerr << "Input file greater than max supported file size!\n";
        return -1;
    }
    //counter variables
    int k = 0;
    int count = 0;
    //file handle
    fstream superBlock;
    fstream fin;
    //for reading from input file
    string s1;
    //open file
    superBlock.open(s + "/input_file1_dir/super_block.txt", ios::out);
    if(!superBlock)
    {
    	cout << "super block does not exist" << endl;
    }
    fin.open(argv[1], ios::in);
    if(!fin)
    {
        std::cout << "Error file does not exist" << endl;
    }
    //to store file names
    string s2;
    //to get block number from input files
    int blockNum;
    char * token;
    //for ignoring first line that contains basic info
    getline(fin, s1);
    while(getline(fin, s1))
    {
        //tokenize the string
        token = strtok((char *)s1.c_str(), ",");
        //convert the block number in integer
        blockNum = stoi(token);
        //extract content from file
        token = strtok(NULL, ",");
        validBlockNum[blockNum] = 1;
        if(blockNum < 12)
        {
            //storing file names for direct blocks
            s2 = file_names_for_direct_blocks[blockNum];
            fullpath = s + "/" + "input_file1_dir/" + file_names_for_direct_blocks[blockNum];
            fstream ofs;
            //opening file for writing data
            ofs.open(fullpath, ios::out);
            ofs << token;
            //closing file
            ofs.close();
            //assign memory and initializing it
            if(blockNum == 0)
            {
                n.p0 = new string(s2);
            }
            else if(blockNum == 1)
            {
                n.p1 = new string(s2);
            }
            else if(blockNum == 2)
            {
                n.p2 = new string(s2);
            }
            else if(blockNum == 3)
            {
                n.p3 = new string(s2);
            }
            else if(blockNum == 4)
            {
                n.p4 = new string(s2);
            }
            else if(blockNum == 5)
            {
                n.p5 = new string(s2);
            }
            else if(blockNum == 6)
            {
                n.p6 = new string(s2);
            }
            else if(blockNum == 7)
            {
                n.p7 = new string(s2);
            }
            else if(blockNum == 8)
            {
                n.p8 = new string(s2);
            }
            else if(blockNum == 9)
            {
                n.p9 = new string(s2);
            }
            else if(blockNum == 10)
            {
                n.p10 = new string(s2);
            }
            else if(blockNum == 11)
            {
                n.p11 = new string(s2);
            }

        }
        else if(blockNum >= 12 && blockNum <= 111)
        {
            //storing file for direct single block
            s2 = "twelve_" + to_string(blockNum - 12) + ".txt";
            fullpath = s + "/" + "input_file1_dir/" + "twelve_" + to_string(blockNum - 12) + ".txt";
            fstream ofs;
            //open file
            ofs.open(fullpath, ios::out);
            //writing content
            ofs << token;
            //closing file
            ofs.close();
            //assinging file and initializing 
            n.p12[blockNum - 12] = new string(s2);
        }
        else
        {
            if(blockNum == 112)
            {
               k = 0;
            }
            if(k % 100 == 0 && k != 0)
            {
               k = 0;
               count++;
            }
            //storing file name for double indirect block
            s2 = "thirteen_" + to_string(count) + "_" +  to_string(k) + ".txt";
            fullpath = s + "/" + "input_file1_dir/" + "thirteen_" + to_string(count) + "_" +  to_string(k) + ".txt";
            fstream ofs;
            //opening file
            ofs.open(fullpath, ios::out);
            //wrinting to file
            ofs << token;
            //closing file
            ofs.close();
            //assiging memory and initializing it
            n.p13[count][k] = new string(s2);
            k++;
        } 
        superBlock << s2 << endl;
    }
    //closing input file
    superBlock.close();
    fin.close();
    //open second file
    fin.open(argv[2], ios::in);
    if(!fin)
    {
        std::cout << "Access trace file does not exist " << endl;
    }
    //variable to stroe content of second file and other loacal variables
    string accData;
    string mode;
    string content;
    string block;
    fstream ofs;
    fstream inpf;
    string s3;
    int pointerNum;
    while(getline(fin, accData))
    {
        //it will tokenize string and store first argument
        mode = strtok((char *)accData.c_str(), ",");
        // if mode is writing
        if(mode == "W")
        {
            //getting block number
            block = strtok(NULL, ",");
            blockNum = stoi(block);
            //getting content
            content = strtok(NULL, ",");
            int l = 0;
            while(content[l] == ' ')
            {
                l++;
            }
            //removing spaces
            content = content.substr(l);
            cout << "Write request for block number " << blockNum << "." << endl;
            if(blockNum < 12)
            {
                cout << "Accessed pointer " << blockNum << " of myNode: " << file_names_for_direct_blocks[blockNum] << endl;
                if(validBlockNum[blockNum] == 0)
                {
                    cout << "Invalid block number!" << endl;
                    cout << endl;
                }
                else
                {
                    if(blockNum == 0)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p0, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 0 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 1)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p1, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 1 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 2)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p2, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 2 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 3)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p3, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 3 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 4)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p4, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 4 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 5)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p5, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 5 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 6)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p6, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 6 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 7)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p7, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 7 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 8)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p8, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 8 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 9)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p9, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 9 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 10)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p10, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 10 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                    else if(blockNum == 11)
                    {
                        ofs.open(s + "/input_file1_dir/" + *n.p11, ios::out);
                        if(!ofs)
                        {
                            std::cout << "file 11 not opened" << endl;
                        }
                        ofs << content;
                        ofs.close();
                    }
                }
            }
            else if(blockNum >= 12 && blockNum <= 111)
            {
                blockNum = blockNum - 12;
                cout << "Accessed pointer 12 of myNode: twelve.txt" << endl;
                if(validBlockNum[blockNum] == 0)
                {
                    cout << "Invalid block number!" << endl;
                    cout << endl;
                }
                else
                {
                    cout << "Accessed pointer " << blockNum << " of twelve.txt: " << *n.p12[blockNum] << endl; 
                    ofs.open(s + "/input_file1_dir/" + *n.p12[blockNum], ios::out);
                    if(!ofs)
                    {
                        std::cout << "file 12 does not exist" << endl;
                    }
                    ofs << content;
                    ofs.close();
                    cout << endl;
                }
            }
            else
            {
                std::cout << "Accessed pointer 13 of myNode: thirteen.txt" << endl;
                if(validBlockNum[blockNum] == 0)
                {
                    cout << "Invalid block number!" << endl;
                    cout << endl;
                }
                else
                {
                    std::cout << "Accessed pointer " << (blockNum-112)/100 << " of thirteen.txt: thirteen_" << (blockNum-112)/100 << ".txt" << endl;
                    std::cout << "Accessed pointer " << (blockNum-112)%100 << " of thirteen_" << (blockNum-112)/100 << ".txt: : " << *n.p13[(blockNum-112)/100][(blockNum-112)%100] << endl;
                    ofs.open(s + "/input_file1_dir/" + *n.p13[(blockNum-112)/100][(blockNum-112)%100], ios::out);
                    if(!ofs)
                    {
                        std::cout << "file 13 does not exist" << endl;
                    }
                    ofs << content;
                    ofs.close();
                    cout << endl;
                }
                
            } 
        }
        else
        {
            block = strtok(NULL, ",");
            blockNum = stoi(block);
            std::cout << "Read request for block number " << blockNum << "." << endl;
            if(blockNum < 12)
            {
                std::cout << "Accessed pointer " << blockNum << " of myNode: " << file_names_for_direct_blocks[blockNum] << endl;
                if(validBlockNum[blockNum] == 0)
                {
                    cout << "Invalid block number!" << endl;
                    cout << endl;
                    continue;
                }
            }
            else if(blockNum > 11 && blockNum < 112)
            {
            	std::cout << "Accessed pointer 12 of myNode: twelve.txt" << endl;
                if(validBlockNum[blockNum] == 0)
                {
                    cout << "Invalid block number!" << endl;
                    cout << endl;
                    continue;
                }
            }
            else
            {
            	std::cout << "Accessed pointer 13 of myNode: thirteen.txt" << endl;
                if(validBlockNum[blockNum] == 0)
                {
                    cout << "Invalid block number!" << endl;
                    cout << endl;
                    continue;
                }
            }
            if(blockNum < 12)
            {
                std::cout << "Reading from " << file_names_for_direct_blocks[blockNum] << "." << endl;
                std::cout << "Contents:" << endl;
                if(blockNum == 0)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p0, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 0 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 1)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p1, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 1 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << " " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 2)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p2, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 2 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 3)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p3, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 3 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 4)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p4, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 4 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 5)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p5, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 5 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 6)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p6, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 6 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 7)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p7, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 7 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 8)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p8, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 8 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 9)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p9, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 9 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 10)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p10, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 10 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
                else if(blockNum == 11)
                {
                    inpf.open(s + "/input_file1_dir/" + *n.p11, ios::in);
                    if(!inpf)
                    {
                        std::cout << "file 11 not opened" << endl;
                    }
                    while(getline(inpf, s3))
                    {
                        std::cout << "  " << s3 << endl;
                    }
                    inpf.close();
                    std::cout << endl;
                }
            }
            else if(blockNum >= 12 && blockNum <= 111)
            {
                blockNum = blockNum - 12;
                std::cout << "Accesed pointer " << blockNum << " of twelve.txt: " << *n.p12[blockNum] << endl;
                std::cout << "Reading from " << *n.p12[blockNum] << "." << endl;
                std::cout << "Contents:" << endl;
                inpf.open(s + "/input_file1_dir/" + *n.p12[blockNum], ios::in);
                if(!inpf)
                {
                    std::cout << "file 12 does not exist" << endl;
                }
                while(getline(inpf, s3))
                {
                    std::cout << "  " << s3 << endl;
                }
                inpf.close();
                std::cout << endl;
            }
            else
            {
                std::cout << "Accessed pointer " << (blockNum-112)/100 << " of thirteen.txt: thirteen_" << (blockNum-112)/100 << ".txt" << endl;
                std::cout << "Accessed pointer " << (blockNum-112)%100 << " of thirteen_" << (blockNum-112)/100 << ".txt: : " << *n.p13[(blockNum-112)/100][(blockNum-112)%100] << endl;
                inpf.open(s + "/input_file1_dir/" + *n.p13[(blockNum-112)/100][(blockNum-112)%100], ios::in);
                if(!inpf)
                {
                    std::cout << "file 13 does not exist" << endl;
                }
                std::cout << "Reading from " << *n.p13[(blockNum-112)/100][(blockNum-112)%100] << "." << endl;
                std::cout << "Contents:" << endl;
                while(getline(inpf, s3))
                {
                    std::cout << "  " << s3 << endl;
                }
                inpf.close();
                std::cout << endl;  
            } 
        }
	
    }
    delete n.megaData;
    delete n.p0;
    delete n.p1;
    delete n.p2;
    delete n.p3;
    delete n.p4;
    delete n.p5;
    delete n.p6;
    delete n.p7;
    delete n.p8;
    delete n.p9;
    delete n.p10;
    delete n.p11;
    for(int i = 0; i < 100; i++)
    {
	delete n.p12[i];
    }
    delete [] n.p12;
    for(int i = 0; i < 100; i++)
    {
    	delete [] n.p13[i];
    }
    delete [] n.p13;
    return 0;
}
