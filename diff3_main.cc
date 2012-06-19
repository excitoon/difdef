
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "diffn.h"

struct stringfile {
    std::string filename;
    int fileid;
    StringSet *unique_lines;
    std::vector<const std::string *> lines;
    
    stringfile(std::string fname, int fileid, StringSet &u):
        filename(fname), fileid(fileid), unique_lines(&u)
    {}
    
    void read_lines();
    int size() const { return lines.size(); }
};

void stringfile::read_lines()
{
    std::ifstream in(this->filename.c_str());
    std::string line;
    while (std::getline(in, line)) {
        const std::string *s = this->unique_lines->add(this->fileid, line);
        this->lines.push_back(s);
    }
}


int main(int argc, char **argv)
{
    StringSet unique_lines;
    std::vector<stringfile> all_files;
    
    NUM_FILES = argc-1;

    for (int i=1; i < argc; ++i) {
        stringfile f(argv[i], i-1, unique_lines);
        f.read_lines();
        all_files.push_back(f);
    }

    Diff result = diff_three_files(all_files[0].lines, all_files[1].lines, all_files[2].lines);

    /* Print out the diff. */
    const int n = result.size();
    for (int i=0; i < n; ++i) {
        const Diff::Line &line = result[i];
        putchar(line.in_[0] ? 'a' : ' ');
        putchar(line.in_[1] ? 'b' : ' ');
        putchar(line.in_[2] ? 'c' : ' ');
        putchar(' ');
        puts(line.text->c_str());
    }

    return 0;
}