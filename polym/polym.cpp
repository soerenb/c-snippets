/*
 * polym.cpp - Polymorphism snippet
 * Copyright (C) 2017  Sören Brinkmann <soeren.brinkmann@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <iostream>

using namespace std;

class base {
public:
	base(const char *s="default") { cout << "base constructor: " << s << "\n"; };
	virtual ~base(void) { cout << "base destructor\n"; };

	void foo(void) { cout << "foo from base\n"; };
	virtual void bar(void) { cout << "bar from base\n"; };
};

class derived : public base {
public:
	derived(const char *s="custom") : base(s) { cout << "derived constructor\n"; };
	~derived(void) { cout << "derived destructor\n"; };

	void foo(void) { cout << "foo from derived\n"; };
	virtual void bar(void) override { cout << "bar from derived\n"; };
};

int main(int argc, char *argv[])
{
	base *b = new base;
	derived *d = new derived("baz");
	cout << "\n";

	cout << "calling base via 'base *'\n";
	b->foo();
	b->bar();
	cout << "\n";

	cout << "calling derived via 'derived *'\n";
	d->foo();
	d->bar();
	cout << "\n";

	delete b;
	cout << "\n";

	cout << "calling derived via 'base *'\n";
	b = d;
	b->foo();
	b->bar();
	cout << "\n";

	delete b;
	cout << "\n";

	return 0;
}
