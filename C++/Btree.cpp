/* *********************
 * @file:   Btree.cpp
 * @brief:  已完成函数如下：
 *			二叉树的插入，删除，搜索，
 *			二叉树的层次遍历
 *			二叉树的最大深度，最小深度
 *			二叉树的后继节点
 * @date:   2019/5/7
 * @author: luhao
 * *********************/
#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

template<class T>
struct TreeNode
{
	T _key;
	TreeNode<T>* _l; //左子节点
	TreeNode<T>* _r; //右子节点
	TreeNode<T>* _p; //父节点
	TreeNode(const T& key) //初始化Node
		: _l(NULL)
		  ,_r(NULL)
		  ,_p(NULL)
		  ,_key(key)
	{}
};

template<class T>
class Btree
{
	typedef TreeNode<T> Node;
	typedef TreeNode<T>* pNode; 
public:
	pNode _root; //根节点
	Btree() {_root=NULL;}; //初始化二叉树
	/*
	~Btree(); //折构函数
	pNode search(T x); //返回元素x的指针
	void insert(T x); //插入元素x
	pNode successor(pNode x); //返回x的后继节点			
	void Delete(T x);   //删除元素x
	int maxDepth(); //返回最大深度
	int minDepth(); //返回最小深度
	void levelorder(); //层次遍历
	*/
	
	int maxDepth(){
		return MaxDepth(_root);
	}

	int MaxDepth(pNode z)
	{	/* 返回二叉树最大深度
		   递归实现，分别求左子树和右子树的深度，最后加1 */
		if (z==NULL)
			return 0;
		return max(MaxDepth(z->_l)+1,MaxDepth(z->_r)+1);
	}
	
	int minDepth(){
		return MinDepth(_root);
	}

	int MinDepth(pNode z)
	{	/* 返回二叉树最小深度
		   注意当_root只有一个孩子时，并不能返回最小深度1，因为它不是叶子节点 */
		if (!z)
			return 0;
		if (z->_l && z->_r)
			return min(MinDepth(z->_l)+1,MinDepth(z->_r)+1);
		else 
			return MinDepth(z->_l) + MinDepth(z->_r) + 1;
	}

	pNode Search(pNode z, T x)
	{
		if (z->_key == x)
			return z;
		if (z == NULL)
			return NULL; //没找到
		if (x < z->_key)
 			return Search(z->_l,x);
		else
			return Search(z->_r,x);
	}

	pNode search(T x)
	{
		return Search(_root, x);
	}

	void insert(T x)
	{
		pNode z = new Node(x);
		//若二叉树为空，直接插root
		if (_root == NULL)
			_root = z;
		else{
			pNode tmp = _root;
			pNode y = NULL; //y即是待插节点的父节点
			while (tmp!=NULL){
				y = tmp;
				if (x > tmp->_key)//若大于当前节点，向右子节点搜索
					tmp = tmp->_r;
				else
					tmp = tmp->_l;
			}
			z->_p = y;
			if (z->_key < y->_key) //更新为左子节点
				y->_l = z;
			else  //更新为右子节点
				y->_r = z;
		}	
	}

	pNode successor(pNode x)
	{	//返回x的后继节点,即x的右节点的最左节点
		if (x->_r==NULL)
			return NULL;
		pNode z = x->_r;
		pNode tmp = NULL;
		while (z!=NULL){
			tmp = z;
			z = z->_l;
		}
		return tmp;
	}

	void Delete(T x)
	{	//删除节点，分3种情形
		pNode z = search(x);
		pNode p = z->_p; //p是x的父节点，当x是根节点是p不存在，拿出来讨论
		if (z==NULL) return;
		//1. x是叶子节点，直接删除
		if (z->_l==NULL && z->_r==NULL){
			if (z == _root)  //x是根节点
				_root = NULL;
			if (p->_l == z)  //x是父节点的左孩子
				p->_l = NULL;
			else   //x是父节点的右孩子
				p->_r = NULL;
		}

		//2. x只有一个孩子,替换即可
		else if (z->_l==NULL && z->_r!=NULL){ //有一个右孩子
			if (z==_root)
				_root = z->_r;
			else{
				if (p->_l == z){
					p->_l = z->_r;
					z->_r->_p = p;
				}
				else {
					p->_r = z->_r;
					z->_r->_p = p;
				}
			}
		}
		else if (z->_l!=NULL && z->_r==NULL){ //有一个左孩子
			if (z==_root)
				_root = z->_l;
			else{
				if (p->_l == z){
					p->_l = z->_l;
					z->_l->_p = p;
				}
				else {
					p->_r = z->_l;
					z->_l->_p = p;
				}
			}
		}

		else { //有两个孩子
			pNode nex = successor(z); //nex是x的后继节点
			/* nex必然没有左孩子
			 * 若nex是左孩子，那么把nex的p的左孩子指向nex的右孩子
			 * 若nex是右孩子，那么把nex的p的右孩子指向nex的右孩子*/
			z->_key = nex->_key; //用nex替换x
			if (nex == nex->_p->_l)
				nex->_p->_l = nex->_r;
			if (nex == nex->_p->_r)
				nex->_p->_r = nex->_r;
		}
	}

	void levelorder()
	{   // 层次遍历； 每层元素压队列即可
		cout << "层次遍历：" << endl;
		pNode z;
		queue<pNode> Q; // TODO 重点：Q存放的是指向Node<T>类型的指针
		Q.push(_root);
		while (!Q.empty())	{
			z = Q.front();
			cout << z->_key << ' ';
			if (z->_l)
				Q.push(z->_l); //左右子节点入队列
			if (z->_r)
				Q.push(z->_r);
			Q.pop();
		}
		cout << endl;
	}
};


int main(){
	Btree<int> B;
	B.insert(15);
	B.insert(6);
	B.insert(18);
	B.insert(3);
	B.insert(2);
	B.insert(7);
	B.insert(13);
	B.insert(9);
	B.insert(17);
	B.insert(20);
	B.insert(4);
	B.levelorder();
	cout << B.minDepth() << endl;
	return 1;

}
