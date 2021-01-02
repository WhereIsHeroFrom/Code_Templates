#include "pushboxgame.h"

using namespace std;

int main() {
    PushBoxGame pbc;
    char filebuf[256];
    while (1) {
        printf("请输入关卡编号：\n");
        int lvl;
        scanf("%d", &lvl);
        sprintf(filebuf, "Mission/%d.x", lvl);

        if (pbc.read(filebuf)) {
            pbc.output();
            pbc.genId();
            if (pbc.search()) {
                pbc.render();
            }
            else{
                printf("抱歉！没算出来！\n");
            }
        }
    }
    return 0;
}
