# Fun Problems

## Towers of Hanoi
*Problem:* There are 3 pegs and *n* disks of different sizes. Initially all the disks are placed on one peg in ascending order with the smallest disk on the top. The problem is to move all the disks to another peg respecting the following rules:
1. Only 1 disk can move at a time.
2. Larger disks must not be placed on the smaller one.

*Solution:* Let's call the three pegs *S(ource)*, *I(ntermediate)*, and *T(arget)*. Initially all the disks are on peg S. The goal is to move all the disks to peg T.

For *n == 1*, the solution is simple.
- Move disk from S to T.

For *n == 2*, the solution is still simple.
- Move disk from S to I.
- Move disk from S to T.
- Move disk from I to T.

For *n == 3*, the solution starts getting cumbersome.
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

## The skyline problem
*Problem:* A set of buildings are given. Each building is defined by the following coordinates: starting/ending points on the x-axis and the height of the building on the y-axis. The input is provided as *{(s<sub>1</sub>, e<sub>1</sub>, h<sub>1</sub>), (s<sub>2</sub>, e<sub>2</sub>, h<sub>2</sub>), ... (s<sub>n</sub>, e<sub>n</sub>, h<sub>n</sub>)}*.
![buildings](buildings.jpeg)
The problem is to find the skyline (or silhouette) of the buidlings. The skyline for the above set if buildings is
![skyline](buildings2.jpeg)
But we know only the horizontal line segments along the x-axis, it is sufficient to draw the skyline.
![output](buildings3.jpeg)
The solution output can thus include only the starting point of each horizontal line segment as *{(x<sub>1</sub>, y<sub>1</sub>), (x<sub>2</sub>, y<sub>2</sub>), ... (x<sub>m</sub>, y<sub>m</sub>)}*.<br>
*Solution:* The solution is not straight-forward. A few data-structures are needed before exploring the solution.
```C++
/*
 * Defines a single point on the x-y axis.
 */
struct point
{
	int x;
	int y;

	explicit point(int a, int b) : x(a), y(b) {}
	bool operator==(const point &p) { return ((p.x == x) && (p.y == y)); }
};

ostream &
operator<< (ostream &os, const point &pt)
{
	os << "(" << pt.x << ", " << pt.y << ")";
	return os;
}

/*
 * Less than operator for point. Since we move from left-to-right,
 * we are only concerned about the x-coordinate.
 */
bool
operator< (const point &p1, const point &p2)
{
	return (p1.x < p2.x);
}

/*
 * Defines a building.
 */
class building
{
private:
	int start;	// starting x-coordinate
	int end;	// ending x-coordinate
	int height;	// height of the building: y-coordinate

public:
	explicit building(int x1, int x2, int h) : start(x1), end(x2), height(h) {}

	/*
	 * Is the point contained in the building? A point is considered
	 * with in the building if:
	 * start <= p.x < end AND
	 * p.y <= height.
	 *
	 * @return height of the building if the point is contained in the
	 * building, -1 if the building does not contain the point.
	 */
	int contains(const point &p) const
	{
		if ((p.x >= start) && (p.x < end) && (p.y <= height))
			return height;
		return -1;
	}
};
```
The following code snippet reads the building coordinates. It saves buildings in a vector. It also saves the **key** (or **critical**) points for each building. The key points of a building are top-left and bottom-right coordinates.
```C++
int x1, x2, h;
vector<point> keypoints;
vector<building> buildings;

fstream fin(<file-containing-building-coordinates>, ios_base::in);
while (fin >> x1 >> x2 >> h) {
        buildings.emplace_back(x1, x2, h);
        keypoints.emplace_back(x1, h);
        keypoints.emplace_back(x2, 0);
}
fin.close();
```
