#include <map>
#include <stdio.h>
#include <fstream>
#include <dirent.h>

using namespace std;

class DocuSet {
public:
    static char* content;
    static int length;

    // return true for file, false for dir
    bool input_file(string path) {
        return path.find(".input") != path.npos;
    }

    // read a file in to a char array
    static char* read_from(string file, int& length) {
        filebuf *pbuf;
        ifstream filestr;
        long size;
        filestr.open (file.c_str(), ios::binary);
        // 获取filestr对应buffer对象的指针 
        pbuf=filestr.rdbuf();
        
        // 获取文件大小
        size=pbuf->pubseekoff (0,ios::end,ios::in);
        pbuf->pubseekpos (0,ios::in);
        
        char* buffer;
        buffer=new char[size];
        // 获取文件内容
        pbuf->sgetn (buffer,size);

        length = size;
        
        filestr.close();
        return buffer;
    }

    // load the content
    static void load_doc(string path) {
        if (DocuSet::content != NULL) {
            delete []content;
        }
        DocuSet::content = DocuSet::read_from(path, DocuSet::length);
    }

    // return content (point)
    static char* get_content(string filename) {
        return content;
    } 
};
