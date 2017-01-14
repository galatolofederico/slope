## Synopsis

Super Light Objective Processor Emulator (aka slope) is a very light and simple virtual machine for an object-oriented processor.
The entire project was realized for an university exam, so it **is not** complete or usable in real life.
I've developed too a super-simple compiler(that compiles in bytecode) for a waek-typed scripting language.

## Install

```bash
make
sudo make install
```

## Examples

```
function main(){
	a = 10
	print("str + str")
	print("Hello "+"world")
	print("str + int")
	print("Test "+a)
	print("str * int")
	print("rep "*a)
}
```

Outputs:

```
str + str
Hello world
str + int
Test 10
str * int
rep rep rep rep rep rep rep rep rep rep 
```

```
#type list 256
#operation push 0
#operation pop 1
#operation index 2
#operation len 3

function crealista(max){
	j = 0
	li = list()
	while(j < max){
		li.push(j)
		j = j + 1
	}
	return(li)
}


function main(){
	a = 0
	print("n?")
	input(a)
	l = crealista(a)
	print("The list is "+l))
}
```

Outputs:

```
n?
10
The list is [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

You can find other examples in the scripts/ folder
