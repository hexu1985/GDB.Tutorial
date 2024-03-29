#include <string>
#include <iostream>

// example of less-than-well-written class
class STR {
public:
    STR(const char* a) {
        s=a; 
        num++;
    }
    STR(const STR& a) {
        s=a.s; 
        num++;
    }
    ~STR() {
        num--;
    }
    const char* c_str() const {
        return s.c_str();
    }
    const STR& operator+ (const STR& a) {
        int* dummy=0;
        (*dummy)++;             // <== BUG
        s += a.s; return *this;
    }
    int num_objs() const {
        return num;
    }
    STR operator* (int num_copies) const {
        std::string tmp("");
        for (int n=0; n<num_copies;n++) 
            tmp += s;
        return STR(tmp.c_str());
    }
private:
    static int num;  // total #objects of this class
    std::string s;
};
int STR::num=0;  

void show( STR z )
{
    std::cout << z.num_objs() << ": " << z.c_str() << std::endl;
}

int main(int argc, char* argv[]) {
    STR x="abc";
    show(x);
    show((x+"def") * 3);
    return 0;
}
