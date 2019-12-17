typedef struct {
    int x,  y[10];
    s1 *t3,  t4[15];
} s1;

typedef struct {
    s1 *t1, t2[20];
} s2;

int fun1(int a, bool b) {
    int *pointertob;

    if (b) {
        *pointertob = (fun1(*&*&*&*pointertob, b) + a) - 4 * 12;
    }else
    {
        if(a > 10)
        {
            pointertob = &a;
        }
    }
    return *&*&*&*&*pointertob;
}


int main() {
    int limit,cont,option,test2,t,u;
    bool g;
    s2 test;

    option = 1;
    limit = 10;
    cont = 0;
    t = 1;
    u = 2;
    g = true;

    test.t1->x = u;
    test.t2[3].y[3] = 3;

    test.t1->y[4] = test.t1->x + test.t2[3].y[3];

    test.t1->t4[5].x = test.t1->y[4] + 1;

    test.t1->t3->t3->t3->t3->t3->t4[1].x = test.t1->t4[5].x * 2;

    fun1(t, g);

    while (cont < limit) {
        switch (option) {
            case 1:
                option = 2;
                test2 = 1;
                break;

            case 2:
                option = 3;
                test2 = 2;
                break;

            case 3:
                option = 1;
                test2 = 3;
                break;
        }
        cont = cont + 1;
    }
    return 0;
}