#include "path.h"

Path::Path() : pathchain_(NULL) {
    initialize();
}

Path::~Path() {
    finalize();
}


void Path::finalize() {
    if (pathchain_) {
        delete[] pathchain_;
        pathchain_ = NULL;
    }
}

void Path::initialize() {
    // 1.  Õ∑≈ø’º‰±‹√‚ƒ⁄¥Ê–π¬©
    if (!pathchain_) {
        pathchain_ = new int[MAXH + 1];
    }
    memset(pathchain_, -1, (MAXH + 1) * sizeof(int));
}

void Path::add(int nowpath, int prepath) {
    pathchain_[nowpath] = prepath;
}

void Path::getPath(int start, vector <int>& path) {
    path.clear();
    while (start != -1) {
        path.push_back(start);
        start = pathchain_[start];
    }
    reverse(path.begin(), path.end());
}

