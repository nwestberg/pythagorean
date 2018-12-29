#include <time.h>
#include <stdio.h>

//-----------------------------------------------------------
// Example code from:
// https://aras-p.info/blog/2018/12/28/Modern-C-Lamentations/
void simplest()
{
	int i = 0;
	for (int z = 1; ; ++z)
		for (int x = 1; x <= z; ++x)
			for (int y = x; y <= z; ++y)
				if (x*x + y*y == z*z) {
					printf("(%i,%i,%i)\n", x, y, z);
					if (++i == 100)
						return;
				}
}

struct pytriples
{
	pytriples() : x(1), y(1), z(1) {}
	void next()
	{
		do
		{
			if (y < z)
				++y;
			else
			{
				if (x < z)
					++x;
				else
				{
					x = 1;
					++z;
				}
				y = x;
			}
		} while (x*x + y*y != z*z);
	}
	int x, y, z;
};

void reusable()
{
	pytriples py;
	for (int c = 0; c < 100; ++c)
	{
		py.next();
		printf("(%i,%i,%i)\n", py.x, py.y, py.z);
	}
}
//-----------------------------------------------------------


//-----------------------------------------------------------
// Similar reusable solution but keeping the three nested 
// for loops and using goto for picking up where we left off.
struct pytriples_goto
{
	pytriples_goto() : x(1), y(1), z(1) {}
	void next()
	{
		goto start;
		for (z = 1; ; ++z) {
			for (x = 1; x <= z; ++x) {
				for (y = x; y <= z; ++y) {
					if (x*x + y*y == z*z) {
						return;
					}
				start:;
				}
			}
		}
	}
	int x, y, z;
};

void reusable_goto()
{
	pytriples_goto py;
	for (int c = 0; c < 100; ++c)
	{
		py.next();
		printf("(%i,%i,%i)\n", py.x, py.y, py.z);
	}
}
//-----------------------------------------------------------


//-----------------------------------------------------------
// Another reusable solution where we use nested loops for
// clarity and separate the inner state from return value.
struct triple
{
	int x, y, z;
};

struct pytriples_while
{
	pytriples_while() : x(1), y(1), z(1) {}
	triple next()
	{
		while (z > 0) {
			while (x <= z) {
				while (y <= z) {
					if (x*x + y*y == z*z) {
						triple t{ x, y, z };
						++y;
						return t;
					}
					++y;
				}
				++x;
				y = x;
			}
			++z;
			x = 1;
		}
		return triple{ 0, 0, 0 };
	}
private:
	int x, y, z;
};

void reusable_while()
{
	pytriples_while py;
	for (int c = 0; c < 100; ++c)
	{
		triple t = py.next();
		printf("(%i,%i,%i)\n", t.x, t.y, t.z);
	}
}
//-----------------------------------------------------------


int main()
{
	clock_t t0;

	t0 = clock();
	simplest();
	printf("%ims\n", (int)(clock() - t0) * 1000 / CLOCKS_PER_SEC);

	t0 = clock();
	reusable();
	printf("%ims\n", (int)(clock() - t0) * 1000 / CLOCKS_PER_SEC);

	t0 = clock();
	pytriples_goto();
	printf("%ims\n", (int)(clock() - t0) * 1000 / CLOCKS_PER_SEC);

	t0 = clock();
	reusable_while();
	printf("%ims\n", (int)(clock() - t0) * 1000 / CLOCKS_PER_SEC);

	return 0;
}

