#include <stdio.h>
#include <stdbool.h>

/*
 * ノード方式:
 *   ゲートも入力端子も「ノード」1 個。
 *   ノードは (種類, 入力元ノードへのポインタ, 自分の出力値) だけを持つ。
 *   つなぐ = 入力元ポインタに相手のアドレスを入れる。
 *   計算する = 入力元の出力値を読んで、自分の出力値を決める。
 */

typedef enum
{
    INPUT, /* 外から値を入れる端子 */
    NOT,
    AND,
    OR,
    NAND,
    NOR,
    XOR,
    XNOR
} Kind;

typedef struct Node
{
    Kind kind;
    struct Node *a; /* 入力元 1 (なければ NULL) */
    struct Node *b; /* 入力元 2 (なければ NULL) */
    bool out;       /* このノードの出力値 */
} Node;

/* 回路 = ノードの配列。作った順に並ぶ */
typedef struct
{
    Node nodes[32];
    int count;
} Circuit;

/* ノードを 1 つ作って、そのポインタを返す */
Node *add_node(Circuit *c, Kind kind, Node *a, Node *b)
{
    Node *n = &c->nodes[c->count];
    c->count++;
    n->kind = kind;
    n->a = a;
    n->b = b;
    n->out = false;
    return n;
}

/* 部品を作る関数。戻り値を次の部品に渡せば「つなぐ」ことになる */
Node *make_input(Circuit *c) { return add_node(c, INPUT, NULL, NULL); }
Node *make_not(Circuit *c, Node *a) { return add_node(c, NOT, a, NULL); }
Node *make_and(Circuit *c, Node *a, Node *b) { return add_node(c, AND, a, b); }
Node *make_or(Circuit *c, Node *a, Node *b) { return add_node(c, OR, a, b); }
Node *make_nand(Circuit *c, Node *a, Node *b) { return add_node(c, NAND, a, b); }
Node *make_nor(Circuit *c, Node *a, Node *b) { return add_node(c, NOR, a, b); }
Node *make_xor(Circuit *c, Node *a, Node *b) { return add_node(c, XOR, a, b); }
Node *make_xnor(Circuit *c, Node *a, Node *b) { return add_node(c, XNOR, a, b); }

/* 全ノードを作った順に計算する (入力元は必ず先に作られているので、1 回で足りる) */
void evaluate(Circuit *c)
{
    for (int i = 0; i < c->count; i++)
    {
        Node *n = &c->nodes[i];

        if (n->kind == INPUT)
            continue; /* 入力端子は外から値を入れるので計算しない */

        /* 入力元の出力値を読む。入力元がなければ false 扱い (NOT の b など) */
        bool a = n->a != NULL && n->a->out;
        bool b = n->b != NULL && n->b->out;

        switch (n->kind)
        {
        case NOT:  n->out = !a; break;
        case AND:  n->out = a && b; break;
        case OR:   n->out = a || b; break;
        case NAND: n->out = !(a && b); break;
        case NOR:  n->out = !(a || b); break;
        case XOR:  n->out = a != b; break;
        case XNOR: n->out = a == b; break;
        default:   break;
        }
    }
}

int main(void)
{
    Circuit c = {0};

    /* XOR 回路を組む: out = (A AND NOT B) OR (NOT A AND B) */
    Node *A   = make_input(&c);
    Node *B   = make_input(&c);
    Node *nA  = make_not(&c, A);
    Node *nB  = make_not(&c, B);
    Node *x   = make_and(&c, A, nB);
    Node *y   = make_and(&c, nA, B);
    Node *out = make_or(&c, x, y);

    printf("A B | nA nB x y | out\n");
    for (int i = 0; i < 4; i++)
    {
        A->out = i & 1;        /* 入力端子に値を入れる */
        B->out = (i >> 1) & 1;

        evaluate(&c);          /* 全ノードを計算 */

        printf("%d %d | %d  %d  %d %d |  %d\n", A->out, B->out, nA->out, nB->out, x->out, y->out,
               out->out);
    }
    return 0;
}
