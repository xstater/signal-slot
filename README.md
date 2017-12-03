# signal-slot
一个简单的信号槽机制的实现(线程不安全)

## 使用方法

```
//简单的使用
//这个程序会输出
//fuck
//u
#include <iostream>
#include "signal-slot.hpp"

using namespace std;

int main(imt argc,char*argv[]){
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
//fuck
#include <iostream>
#include "signal-slot.hpp"

using namespace std;

int main(imt argc,char*argv[]){
	Signal<void()> sig;
	sig.connect([]()->void{
		cout<<"fuck"<<endl;
	});
	auto connection = sig.connect([]()->void{
		cout<<"u"<<endl
	});
	connection.disconnect();
	sig.emit();
	return 0;
}
```
