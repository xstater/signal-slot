# signal-slot
一个简单的信号槽机制的实现,单线程
需要C++11以上

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
