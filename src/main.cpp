#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Lexer.cpp"
#include "Parser.cpp"
#include "DocuSet.cpp"

using namespace std;

bool input_file(string path) {
    return path.find(".input") != path.npos;
}

char* read_from_(string file, int& length) {
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

int main() {
    string aql_file;
    string doc_path;

    //printf("input aql file name\n");
    cin >> aql_file;
    //aql_file = "Revenue.aql";
    //printf("doc file/dir path\n");
    cin >> doc_path;
    //doc_path = "Revenue.input";

    int length;
    char* code = read_from_(aql_file, length);

    DocuSet doc;


    /*
        process single file or files in a dir
    */
    if (input_file(doc_path)) {                         // single file
        doc.load_doc(doc_path);
        Lexer lexer = Lexer(code, length);
        Parser parser = Parser(&lexer);
        cout << "Processing " << doc_path << endl;
        parser.program();
    } else {                                                       // a dir
        Lexer lexer = Lexer(code, length);
        struct dirent *ptr;    
        DIR *dir;
        dir=opendir(doc_path.c_str());
        while((ptr=readdir(dir))!=NULL)         // tranvesal the dir
        {
            //跳过'.'和'..'两个目录
            if(ptr->d_name[0] == '.')
                continue;
            if (input_file(ptr->d_name)&&input_file(ptr->d_name)) {          // single file
                if (doc_path[doc_path.size()-1] != '/') {
                    doc.load_doc(doc_path+"/"+ptr->d_name);
                } else {
                    doc.load_doc(doc_path+ptr->d_name);
                }
                lexer.back_end();
                Parser parser = Parser(&lexer);
                cout << "Processing " << ptr->d_name << endl;
                parser.program();
                cout << endl;
            }
        }
        closedir(dir);
    }

    return 0;
}
