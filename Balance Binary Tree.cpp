#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
using namespace std;
struct node
{
    int data, height;                                               // 节点数据和高度
    node *lchild;                                                   // 左子树指针
    node *rchild;                                                   // 右子树指针
    node(int x) : data(x), height(1), lchild(NULL), rchild(NULL) {} // 构造函数
};

int getheight(node *p)
{
    return p ? p->height : 0; // 如果节点存在返回高度，否则返回0
} // 获取节点高度

void updateheight(node *p)
{
    p->height = max(getheight(p->lchild), getheight(p->rchild)) + 1; // 节点高度等于左右子树高度的最大值加1
} // 更新节点高度（在插入节点后）

int getbalance(node *p)
{
    return getheight(p->lchild) - getheight(p->rchild); // 平衡因子等于左子树高度减去右子树高度
} // 获取节点平衡因子以计算是否需要旋转

node *rr(node *p)
{
    node *temp = p->lchild;
    node *temp2 = temp->rchild;
    temp->rchild = p;
    p->lchild = temp2;
    updateheight(p);
    updateheight(temp);
    return temp;
} // 左不平衡时右旋

node *ll(node *p)
{
    node *temp = p->rchild;
    node *temp2 = temp->lchild;
    temp->lchild = p;
    p->rchild = temp2;
    updateheight(p);
    updateheight(temp);
    return temp;
} // 右不平衡时左旋

node *insert(node *p, int x)
{
    if (!p)
        return new node(x); // 如果当前节点为空，创建新节点并返回
    if (x < p->data)
        p->lchild = insert(p->lchild, x); // 如果插入值小于当前节点数据，递归插入到左子树
    else if (x > p->data)
        p->rchild = insert(p->rchild, x); // 如果插入值大于当前节点数据，递归插入到右子树
    else
        return p;            // 如果插入值等于当前节点数据，不插入，直接返回当前节点
    updateheight(p);         // 插入节点后更新当前节点高度
    int bal = getbalance(p); // 根据平衡因子判断是否需要旋转及旋转类型
    if (bal > 1 && x < p->lchild->data)
        return rr(p);
    if (bal < -1 && x > p->rchild->data)
        return ll(p);
    if (bal > 1 && x > p->lchild->data)
    {
        p->lchild = ll(p->lchild);
        return rr(p);
    }
    if (bal < -1 && x < p->rchild->data)
    {
        p->rchild = rr(p->rchild);
        return ll(p);
    }
    return p;
} // 插入节点并保持树平衡

void preorder(node *p)
{
    if (!p)
        return;
    cout << p->data << " ";
    preorder(p->lchild);
    preorder(p->rchild);
} // 递归前序遍历

void inorder(node *p)
{
    if (!p)
        return;
    inorder(p->lchild);
    cout << p->data << " ";
    inorder(p->rchild);
} // 递归中序遍历

void postorder(node *p)
{
    if (!p)
        return;
    postorder(p->lchild);
    postorder(p->rchild);
    cout << p->data << " ";
} // 递归后序遍历

void fpreorder(node *p)
{
    stack<node *> st;
    node *cur = p;
    while (!st.empty() || cur)
    {
        while (cur)
        {
            cout << cur->data << " ";
            st.push(cur);
            cur = cur->lchild;
        }
        cur = st.top();
        st.pop();
        cur = cur->rchild;
    }
} // 非递归前序遍历

void finorder(node *p)
{
    stack<node *> st;
    node *cur = p;
    while (!st.empty() || cur)
    {
        while (cur)
        {
            st.push(cur);
            cur = cur->lchild;
        }
        cur = st.top();
        st.pop();
        cout << cur->data << " ";
        cur = cur->rchild;
    }
} // 非递归中序遍历

void fpostorder(node *p)
{
    stack<node *> st1, st2;
    st1.push(p);
    while (!st1.empty())
    {
        node *cur = st1.top();
        st1.pop();
        st2.push(cur);
        if (cur->lchild)
            st1.push(cur->lchild);
        if (cur->rchild)
            st1.push(cur->rchild);
    }
    while (!st2.empty())
    {
        cout << st2.top()->data << " ";
        st2.pop();
    }
} // 非递归后序遍历（使用两个栈使得代码更清晰）

void levelorder(node *p)
{
    if (!p)
        return;
    queue<node *> q;
    q.push(p);
    while (!q.empty())
    {
        node *cur = q.front();
        q.pop();
        cout << cur->data << " ";
        if (cur->lchild)
            q.push(cur->lchild);
        if (cur->rchild)
            q.push(cur->rchild);
    }
} // 层序遍历

int search(node *p, int x)
{
    if (!p)
        return 0;
    if (x < p->data)
        return search(p->lchild, x);
    else if (x > p->data)
        return search(p->rchild, x);
    else
        return 1;
} // 查找关键值，返回1表示找到，0表示未找到

void swaplr(node *p)
{
    if (!p)
        return;
    swap(p->lchild, p->rchild); // 若只交换大子树，写到此处即可，后面是构造镜像树
    swaplr(p->lchild);
    swaplr(p->rchild);
} // 交换左右子树指针实现镜像

int getdepth(node *p, int target, int level = 1)
{
    if (!p)
        return 0;
    if (p->data == target)
        return level;
    int left = getdepth(p->lchild, target, level + 1);
    if (left)
        return left;                               // 在左子树中继续寻找目标节点，如果找到返回深度
    return getdepth(p->rchild, target, level + 1); // 获取节点深度，返回值为节点所在层数，根节点深度为1
} // 获取节点深度，返回值为节点所在层数，根节点深度为1,如果节点不存在返回0

int countleaf(node *p)
{
    if (!p)
        return 0;
    if (!p->lchild && !p->rchild)
        return 1;                                       // 如果当前节点是叶子节点，返回1
    return countleaf(p->lchild) + countleaf(p->rchild); // 递归统计左子树和右子树的叶子节点数量之和
} // 统计叶子节点数量

node *getmin(node *p)
{
    while (p->lchild)
        p = p->lchild; // 最小节点在左子树的最左边
    return p;
} // 获取以p为根的子树中的最小节点

node *delnode(node *p, int x)
{
    if (!p)
        return p;
    if (x < p->data)
        p->lchild = delnode(p->lchild, x);
    else if (x > p->data)
        p->rchild = delnode(p->rchild, x);
    else
    { // 找到那个要删除的节点
        if (!p->lchild || !p->rchild)
        {
            node *temp = p->lchild ? p->lchild : p->rchild; // 如果当前节点只有一个子树，直接用子树替代当前节点
            delete p;
            return temp;
        }
        // 如果当前节点有两个子树，找到右子树中的最小节点替代当前节点，并删除那个最小节点
        node *temp = getmin(p->rchild);
        p->data = temp->data; // 用右子树的最小节点数据替代当前节点数据,因为树的性质，右子树的最小节点一定大于当前节点数据且小于右子树其他节点数据，
        // 所以用它来替代当前节点数据后仍然满足二叉搜索树的性质
        p->rchild = delnode(p->rchild, temp->data); // 删除右子树中的最小节点,因为它已经被用来替代当前节点了
    }
    if (!p)
        return p;
    updateheight(p);         // 每当删除节点后更新当前节点高度
    int bal = getbalance(p); // 根据平衡因子判断是否需要旋转及旋转类型
    if (bal > 1 && getbalance(p->lchild) >= 0)
        return rr(p); // 左子树比右子树高，且左子树的平衡因子大于等于0，说明是LL型不平衡，右旋
    if (bal < -1 && getbalance(p->rchild) <= 0)
        return ll(p); // 右子树比左子树高，且右子树的平衡因子小于等于0，说明是RR型不平衡，左旋
    if (bal > 1 && getbalance(p->lchild) < 0)
    { // 左子树比右子树高，且左子树的平衡因子小于0，说明是LR型不平衡，先对左子树进行左旋，再对当前节点进行右旋
        p->lchild = ll(p->lchild);
        return rr(p);
    }
    if (bal < -1 && getbalance(p->rchild) > 0)
    { // 右子树比左子树高，且右子树的平衡因子大于0，说明是RL型不平衡，先对右子树进行右旋，再对当前节点进行左旋
        p->rchild = rr(p->rchild);
        return ll(p);
    }
    return p;
} // 删除节点并保持树平衡

int main()
{
    node *root = NULL;
    int x;
    cout << "请输入节点数据，-1表示结束输入:";
    while (cin >> x && x != -1)
        root = insert(root, x); // 输入节点数据，-1表示结束输入

    cout << "递归前序遍历：";
    preorder(root);
    cout << endl; // 递归前序遍历：根节点->左子树->右子树

    cout << "递归中序遍历：";
    inorder(root);
    cout << endl; // 递归中序遍历：左子树->根节点->右子树

    cout << "递归后序遍历：";
    postorder(root);
    cout << endl; // 递归后序遍历：左子树->右子树->根节点

    cout << "非递归前序遍历：";
    fpreorder(root);
    cout << endl; // 非递归前序遍历：根节点->左子树->右子树

    cout << "非递归中序遍历：";
    finorder(root);
    cout << endl; // 非递归中序遍历：左子树->根节点->右子树

    cout << "非递归后序遍历：";
    fpostorder(root);
    cout << endl; // 非递归后序遍历：左子树->右子树->根节点

    cout << "层序遍历：";
    levelorder(root);
    cout << endl; // 层序遍历：按层从上到下、从左到右访问节点

    cout << "树叶子节点数量：" << countleaf(root) << endl; // 统计树的叶子节点数量

    cout << "交换左右子树指针实现镜像后层序遍历：";
    swaplr(root);
    levelorder(root);
    cout << endl;
    swaplr(root); // 再交换一次恢复原树

    int searchval;
    cout << "请输入要查找的关键值：";
    cin >> searchval;
    if (search(root, searchval))
        cout << "存在" << endl;
    else
        cout << "不存在" << endl;

    int depthval;
    cout << "请输入要获取深度的节点数据：";
    cin >> depthval;
    int depth = getdepth(root, depthval);
    if (depth)
        cout << "节点深度为：" << depth << endl;
    else
        cout << "节点不存在" << endl;

    int delval;
    cout << "请输入要删除的节点数据：";
    cin >> delval;
    root = delnode(root, delval);
    cout << "删除节点后树的层序遍历：";
    levelorder(root);
    cout << endl;
    return 0;
}
