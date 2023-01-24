#include <iostream>
#include <cstdio>
using namespace std;

namespace LIST
{

    struct NODE {
    	int data;
    	NODE *next;
    };

    NODE *head = NULL;
    int len = 0;

    void init() {
    	head = new NODE();
    	head->next = head;
    }
    NODE* move(int i) {
        NODE *p = head;
        if (!p) return NULL;
        for (int j = 0; j < i; j++) p = p->next;
        return p;
    }
    void insert(int i, int x) {
    	NODE *p, *q;
    	p = head;
		p = move(i);
    	q = new NODE;
    	q->data = x;
    	q->next = p->next;
    	p->next = q;
    	len++;
    }
    void remove(int i) {
    	NODE *p, *q;
    	p = move(i);
		q = p->next;
		p->next = q->next;
		len--;
		delete q;
    }
    void remove_insert(int i) {
    	NODE *p, *tmp;
        p = move(i);
        tmp = p->next;
        p->next = tmp->next;
        len--;
        insert(len,tmp->data);
        delete tmp;
    }
    void get_length() {
		cout<<len<<endl;
    }
    void query(int i) {
    	NODE *p;
    	p = head;
    	for(int l = 0;l <= i; l++)
    	{
    		p = p->next;
			if(p == head)
    		{
    			cout << "-1" <<endl;
    			return;
			}
		}
		cout << p->data <<endl;
    }
    void get_max() {
    	int max=-1;
        NODE *p=head->next;
        while(p!=head)
        {
        	if(max<p->data) max=p->data;
        	p=p->next;
		}
		cout<<max<<endl;
    }
    void clear() {
    	NODE *p, *q;
        p=head->next;
    	while(p != head)
    	{
    		q = p->next;
    		delete p;
    		p = q;
		}
		head->next = head;
    }
}

int n;
int main()
{
    cin >> n;
    int op, x, p;
    LIST::init();
    for (int m = 0; m < n; ++m)
    {
        cin >> op;
        switch(op) {
            case 0:
                LIST::get_length();
                break;
            case 1:
                cin >> p >> x;
                LIST::insert(p,x);
                break;
            case 2:
                cin >> p;
                LIST::query(p);
                break;
            case 3:
                cin >> p;
                LIST::remove(p);
                break;
            case 4:
                cin >> p;
                LIST::remove_insert(p);
                break;
            case 5:
                LIST::get_max();
                break;
        }
    }
    LIST::clear();
    return 0;
}