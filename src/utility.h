#pragma once

#include <iostream>

template<typename T, typename ...Types>
void print(T const& firstArg, Types const&... args)
{
	std::cout << firstArg << ' ';

	if constexpr (sizeof...(args) > 0) {
		print(args...);
	}
	else {
		std::cout << '\n';
	}
	
}


/*
The names of all types — classes, structs, type aliases, enums, and type template parameters — have 
the same naming convention. Type names should start with a capital letter and have a capital letter for each new word. 


The names of variables (including function parameters) and data members are all lowercase,
 with underscores between words. Data members of classes (but not structs) additionally have trailing underscores.
  For instance: a_local_variable, a_struct_data_member, a_class_data_member_.

Variables declared constexpr or const, and whose value is fixed for the duration of the program, are named with a leading "k" followed by mixed case.

const int kDaysInAWeek = 7;
const int kAndroid8_0_0 = 24;  // Android 8.0.0

Regular functions have mixed case; accessors and mutators may be named like variables.
Ordinarily, functions should start with a capital letter and have a capital letter for each new word.

AddTableEntry()
DeleteUrl()
OpenFileOrDie()

 How Bjorn Str
Accessors and mutators (get and set functions) may be named like variables.
 These often correspond to actual member variables, but this is not required. For example, int count() and void set_count(int count).

Enumerators (for both scoped and unscoped enums) should be named like constants, not like macros.
That is, use kEnumName not ENUM_NAME.
enum class UrlTableError {
  kOk = 0,
  kOutOfMemory,
  kMalformedInput,
};

Line Length
80 characters is the maximum.
A line may exceed 80 characters if it is

a comment line which is not feasible to split without harming readability, ease of cut and paste or auto-linking -- e.g., if a line contains an example command or a literal URL longer than 80 characters.
a raw-string literal with content that exceeds 80 characters. Except for test code, such literals should appear near the top of a file.
an include statement.
a header guard
a using-declaration

Use the u8 prefix to guarantee that a string literal containing \uXXXX escape sequences is encoded as UTF-8.
 Do not use it for strings containing non-ASCII characters encoded as UTF-8, because that will produce incorrect
  output if the compiler does not interpret the source file as UTF-8.
You shouldn't use the C++11 char16_t and char32_t character types, since they're for non-UTF-8 text.
 For similar reasons you also shouldn't use wchar_t (unless you're writing code that interacts with the Windows API,
  which uses wchar_t extensively).


eturn type on the same line as function name, parameters on the same line if they fit. 
Wrap parameter lists which do not fit on a single line as you would wrap arguments in a function call.

ReturnType ClassName::FunctionName(Type par_name1, Type par_name2) {
  DoSomething();
  ...
}

Lambda Expressions
Format parameters and bodies as for any other function, and capture lists like other comma-separated lists.

For by-reference captures, do not leave a space between the ampersand (&) and the variable name.

int x = 0;
auto x_plus_n = [&x](int n) -> int { return x + n; }

Floating-point literals should always have a radix point, with digits on both sides, 
even if they use exponential notation. Readability is improved if all floating-point
 literals take this familiar form, as this helps ensure that they are not mistaken for 
 integer literals, and that the E/e of the exponential notation is not mistaken for a
  hexadecimal digit. It is fine to initialize a floating-point variable with an integer literal
   (assuming the variable type can exactly represent that integer), but note that a number in 
   exponential notation is never an integer literal.
float f = 1.0f;
float f2 = 1;   // Also OK
long double ld = -0.5L;
double d = 1248.0e6;

Conditionals
The if and else keywords belong on separate lines. There should be a space between the if and the open parenthesis, and between the
 close parenthesis and the curly brace (if any), but no space between the parentheses and the condition.
if (condition) {  // no spaces inside parentheses
  ...  // 2 space indent.
} else if (...) {  // The else goes on the same line as the closing brace.
  ...
} else {
  ...
}

Braces are optional for single-statement loops.

for (int i = 0; i < kSomeNumber; ++i)
  printf("I love you\n");

for (int i = 0; i < kSomeNumber; ++i) {
  printf("I take it back\n");
}

// These are fine, space preceding.
char *c;
const std::string &str;

// These are fine, space following.
char* c;
const std::string& str;

ariable and Array Initialization
Your choice of =, (), or {}.

You may choose between =, (), and {}; the following are all correct:

int x = 3;
int x(3);
int x{3};
std::string name = "Some Name";
std::string name("Some Name");
std::string name{"Some Name"};

Be careful when using a braced initialization list {...} on a type with an std::initializer_list constructor. A nonempty braced-init-list prefers the std::initializer_list constructor whenever possible. Note that empty braces {} are special, and will call a default constructor if available. 
To force the non-std::initializer_list constructor, use parentheses instead of braces.

std::vector<int> v(100, 1);  // A vector containing 100 items: All 1s.
std::vector<int> v{100, 1};  // A vector containing 2 items: 100 and 1.

Sections in public, protected and private order, each indented one space.

The basic format for a class definition (lacking the comments, see Class Comments for a discussion of what comments are needed) is:

class MyClass : public OtherClass {
 public:      // Note the 1 space indent!
  MyClass();  // Regular 2 space indent.
  explicit MyClass(int var);
  ~MyClass() {}

  void SomeFunction();
  void SomeFunctionThatDoesNothing() {
  }

  void set_some_var(int var) { some_var_ = var; }
  int some_var() const { return some_var_; }

 private:
  bool SomeInternalFunction();

  int some_var_;
  int some_other_var_;
};

*/