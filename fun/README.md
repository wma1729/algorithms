# Fun Problems

## Towers of Hanoi
*Problem:* There are 3 pegs and *n* disks of different sizes. Initially all the disks are placed on one peg in ascending order with the smallest disk on the top. The problem is to move all the disks to another peg respecting the following rules:
1. Only 1 disk can move at a time.
2. Larger disks must not be placed on the smaller one.

*Solution:* Let's call the three pegs S*(ource)*, I*(ntermediate)*, and T*(arget). Initially all the disks are on peg S. The goal is to move all the disks to peg T.

For *n == 1*, the solution is simple.
- Move disk from S to T.

For *n == 2*, the solution is still simple.
- Move disk from S to I.
- Move disk from S to T.
- Move disk from I to T.

For *n == 3*, the solution starts getting big cumbersome.
- Move disk from S to T.
- Move disk from S to I.
- Move disk from T to I.
- Move disk from S to T.
- Move disk from I to S.
- Move disk from I to T.
- Move disk from S to T.

The essence of the process is:
1. Move *n - 1* disks from S to I.
2. Move the final *n<super>th</super>* disk to T.
3. Finally move the remaining *n - 1* disks from I to T.

Here is a rough algorithm:
```
// Move n disks from s to t using i as the intermediate.
toh(n, peg s, peg i, peg t)
{
	if (n == 1) {
		// make the final move; terminates recursion.
		move from s to t.
	} else {
		// move n - 1 disk from s to i using t as the intermediate
		toh(n - 1, s, t, i)

		// make the final move
		move from s to t

		// move n - 1 disk from i to t using s as the intermediate
		toh(n - 1, i, s, t)
	}
}
```
And here is a C++ version:
```C++
void
toh(int h, const string &s, const string &i, const string &t)
{
	if (h) {
		toh(h - 1, s, t, i);
		cout << "move disk " << h << " from " << s << " to " << t << endl;
		toh(h - 1, i, s, t);
	}
}
```
