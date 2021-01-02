#include "commondef.h"

class Path {
public:
    Path();
    virtual ~Path();

private:
    int *pathchain_;

public:
    void finalize();
    void initialize();
    void add(int nowpath, int prepath);
    void getPath(int start, vector <int>& path);
};
