# signal-slot
一个简单的信号槽机制的实现(线程不安全)
需要C++11以上

## 特点
优点:使用简单，内存安全
缺点:大量使用模板，不是线程安全的

## 使用方法

```
//简单的使用
//这个程序会输出
//fuck
//u
#include <iostream>
#include "signal-slot.hpp"

using namespace std;

int main(int argc,char*argv[]){
	Signal<void()> sig;
	sig.connect([]()->void{
		cout<<"fuck"<<endl;
	});
	sig.connect([]()->void{
		cout<<"u"<<endl
	});
	sig.emit();
	return 0;
}
```

```
//通过Connection对象控制
//这个程序会输出
//1
//0
//fuck
#include <iostream>
#include "signal-slot.hpp"

using namespace std;

int main(int argc,char*argv[]){
	Signal<void()> sig;
	sig.connect([]()->void{
		cout<<"fuck"<<endl;
	});
	auto connection = sig.connect([]()->void{
		cout<<"u"<<endl
	});
	cout<<connection.isVaild()<<endl;
	connection.disconnect();
	cout<<connection.isVaild()<<endl;
	sig.emit();
	return 0;
}
```
