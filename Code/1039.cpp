#include "iostream"
#include "cstdio"
#include "cmath"
#include "cstring"
using namespace std;


int fuck_data[30009] = { 0 };
template <class T>
class mQueue {
	class node {
	public:
		T data;
		node* next = nullptr;
	};
	node* head = nullptr,
		* rear = nullptr;
	size_t _size = 0;

public:
	//构造函数
	mQueue() {
		head = new node;
		rear = head;
	}
	//判断是否为空
	bool empty() {
		return head->next == nullptr;
	}
	//增加
	void push(const T& val) {
		rear->next = new node;
		rear = rear->next;
		rear->data = val;
		++_size;
	}
	//删除
	void pop() {
		//安全措施
		if (head->next == nullptr)
			return;
		auto temp = head->next;
		delete head;
		head = temp;
		--_size;
	}
	//大小
	size_t size() {
		return _size;
	}
	//最前
	T& front() {
		//安全措施
		return head->next->data;
	}
	//最后
	T& back() {
		//安全措施
		return rear->data;
	}
};

template <class T>
class mVector {
	T** vector_data = nullptr;
	unsigned int size = 0;
	unsigned int capacity = 0;

public:
	//构造函数
	mVector() = default;
	mVector(unsigned int _size) :size(_size) {
		capacity = _size * 2;
		vector_data = new T * [capacity] {nullptr};
	}
	mVector(unsigned int _size, const T& val) :size(_size) {
		capacity = _size * 2;
		vector_data = new T * [capacity] {nullptr};
		for (unsigned int i = 0; i < _size; ++i)
			vector_data[i] = new T(val);
	}
	mVector(const mVector<T> & _vector) :size(_vector.size), capacity(_vector.capacity) {
		vector_data = new T * [capacity] {nullptr};
		for (int i = 0; i < size; ++i)
			vector_data[i] = new T(*_vector.vector_data[i]);
	}
	//析构函数
	~mVector() {
		for (unsigned int i = 0; i < size; ++i)
			delete vector_data[i];
		delete[] vector_data;
	}

	//迭代器
	class iterator {
		T** pos = nullptr;
		friend iterator mVector<T>::begin();
		friend iterator mVector<T>::end();
		friend void mVector<T>::erase(const iterator& val);
	public:
		iterator() = default;
		iterator(const iterator& other) {
			pos = other.pos;
		}
		iterator& operator++() {
			++pos;
			return *this;
		}
		iterator operator++(int) {
			iterator temp(*this);
			++pos;
			return temp;
		}
		iterator& operator--() {
			--pos;
			return *this;
		}
		iterator operator--(int) {
			iterator temp(*this);
			--pos;
			return temp;
		}
		bool operator==(const iterator& other) const {
			return pos == other.pos;
		}
		bool operator!=(const iterator& other) const {
			return pos != other.pos;
		}
		iterator& operator=(const iterator& other) {
			pos = other.pos;
			return *this;
		}
		T& operator*() const {
			return **pos;
		}
	};

	//增加元素
	void push_back(const T & val) {
		if (size < capacity)
			vector_data[size++] = new T(val);
		else {
			T** temp_data = new T * [(capacity + 1) * 2]{ nullptr };
			for (unsigned int i = 0; i < size; ++i)
				temp_data[i] = vector_data[i];
			temp_data[size++] = new T(val);
			capacity = (capacity + 1) * 2;
			delete[] vector_data;
			vector_data = temp_data;
		}
	}

	//删除末尾
	void pop_back() {
		delete vector_data[size];
		vector_data[size--] = nullptr;
	}

	//清空
	void clear() {
		for (unsigned int i = 0; i < size; ++i) {
			delete vector_data[i];
			vector_data[i] = nullptr;
		}
		size = 0;
	}

	//删除
	void erase(const iterator & val) {
		delete* val.pos;
		for (auto p = val.pos; p != vector_data + size - 1; ++p)
			* p = *(p + 1);
		--size;
		vector_data[size] = nullptr;
	}

	//重载运算符
	T & operator[](const unsigned int& pos) {
		return *vector_data[pos];
	}

	iterator begin() {
		iterator temp;
		temp.pos = vector_data;
		return temp;
	}

	iterator end() {
		iterator temp;
		temp.pos = vector_data + size;
		return temp;
	}

};

//二叉树类
template <class T>
class myBinaryTree {
public:
	class node {
	public:
		T data;
		node* lchild = nullptr, * rchild = nullptr, * parent = nullptr;
	};
	node* root = nullptr;
	node** nodeList = nullptr;
	long long sizeOfTree = 0;

	//创建树
	void createTree(int num) {
		nodeList = new node * [num + 1]{ nullptr };
		sizeOfTree = num;
	}

	void creadNode(int nodeNum, int lchild, int rchild, int data) {
		if (nodeList[nodeNum] == nullptr)
			nodeList[nodeNum] = new node;

		nodeList[nodeNum]->data = data;

		if (lchild != -1) {
			if (nodeList[lchild] == nullptr)
				nodeList[lchild] = new node;
			nodeList[lchild]->parent = nodeList[nodeNum];
			nodeList[nodeNum]->lchild = nodeList[lchild];
		}

		if (rchild != -1) {
			if (nodeList[rchild] == nullptr)
				nodeList[rchild] = new node;
			nodeList[rchild]->parent = nodeList[nodeNum];
			nodeList[nodeNum]->rchild = nodeList[rchild];
		}
	}

	void checkRoot() {
		root = nodeList[1];
		fuck_data[1] = 1;
		get_fuck(1);
	}

	//清空
	void clear() {
		clear(root);
	}

	void clear(node * p) {
		if (p == nullptr)
			return;
		clear(p->lchild);
		clear(p->rchild);
		delete p;
		p = nullptr;
	}

	//构造
	myBinaryTree() {}

	//析构
	~myBinaryTree() {
		clear(root);
	}

	//判断是否非空
	bool empty() {
		return root == nullptr;
	}

	//判断是否为完全二叉树
	bool isCBT() {
		if (empty())
			return false;
		//层次遍历
		mQueue<node*> treeQueue;
		bool flag = false;
		//先入队
		treeQueue.push(root);

		//反复
		while (!treeQueue.empty()) {
			//先出队一个
			node* temp = treeQueue.front();
			treeQueue.pop();

			//右孩子有左孩子没有
			if (temp->lchild == nullptr && temp->rchild != nullptr)
				return false;

			if (!flag) {
				//左孩子有右孩子没有
				if (temp->lchild != nullptr && temp->rchild == nullptr) {
					flag = true;
					//左孩子入队
					treeQueue.push(temp->lchild);
				}
				//左右孩子都没有
				else if (temp->lchild == nullptr && temp->rchild == nullptr) {
					flag = true;
				}
				//左右都有孩子
				else {
					treeQueue.push(temp->lchild);
					treeQueue.push(temp->rchild);
				}
			}
			else {
				//判断是否为叶子
				if (temp->lchild != nullptr || temp->rchild != nullptr)
					return false;
			}
		}

		return true;
	}

	//返回大小
	long long size() {
		return sizeOfTree;
	}

	void get_fuck(int pos) {
		if (nodeList[pos]->lchild) {
			fuck_data[nodeList[pos]->lchild->data] = fuck_data[pos] * 2;
			get_fuck(nodeList[pos]->lchild->data);
		}
		if (nodeList[pos]->rchild) {
			fuck_data[nodeList[pos]->rchild->data] = fuck_data[pos] * 2 + 1;
			get_fuck(nodeList[pos]->rchild->data);
		}
	}

	//层次遍历
	mVector<T> levelTraversal() {
		if (sizeOfTree == 0)
			return *new mVector<T>;

		mVector<T> result;
		mQueue<node*> treeQueue;

		//根结点入队
		treeQueue.push(root);

		//层次遍历
		while (!treeQueue.empty()) {
			//出队
			node* temp = treeQueue.front();
			treeQueue.pop();

			//入队
			if (temp->lchild != nullptr)
				treeQueue.push(temp->lchild);
			if (temp->rchild != nullptr)
				treeQueue.push(temp->rchild);

			//写入
			result.push_back(temp->data);
		}

		//返回
		return result;
	}

	//后序遍历
	mVector<T> lostorderTraversal() {
		mVector<T> result;
		lostorderTraversal(root, result);
		return result;
	}

	void lostorderTraversal(node * treeRoot, mVector<T> & result) {
		if (treeRoot->lchild != nullptr)
			lostorderTraversal(treeRoot->lchild, result);
		if (treeRoot->rchild != nullptr)
			lostorderTraversal(treeRoot->rchild, result);
		//显示当前
		result.push_back(treeRoot->data);
	}
};



int main() {
	int num;
	myBinaryTree<int> bTree;
	cin >> num;
	bTree.createTree(num + 1);

	for (int i = 1; i <= num; i++) {
		int code, lchild, rchild;
		scanf("%d %d %d", &code, &lchild, &rchild);
		bTree.creadNode(code, lchild, rchild, code);
		fuck_data[code] = i;
	}

	bTree.checkRoot();
	cout << fuck_data[1];
	for (int i = 2; i <= num; ++i)
		cout << " " << fuck_data[i];
	cout << endl;
	auto ans = bTree.lostorderTraversal();
	for (auto p : ans)
		cout << p << " ";

	return 0;
}