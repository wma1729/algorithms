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
struct building
{
	int start;	// starting x-coordinate
	int end;	// ending x-coordinate
	int height;	// height of the building: y-coordinate

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

/*
 * Less than operator for building. Since we move from left-to-right,
 * we are only concerned about the start.
 */
bool
operator< (const building &b1, const building &b2)
{
	return (b1.start < b2.start);
}

```
The following code snippet reads the building coordinates. It saves buildings in a vector.
```C++
int x1, x2, h;
vector<building> buildings;

fstream fin(<file-containing-building-coordinates>, ios_base::in);
while (fin >> x1 >> x2 >> h)
        buildings.emplace_back(x1, x2, h);
fin.close();
```

### Key Points
The **key** (or **critical**) points of a building are top-left and bottom-right coordinates of a building. If a single building is taken, the key points define the skyline of the building.

### General approach
The general approach to solve the problem is:
* adjust the height of key points.
* remove redundant key points.

Some examples here:
![skyline](skyline.jpeg)
![skyline2](skyline2.jpeg)

### Approach 1 (Look at all the buildings to determine the height of a key point)
1. All the key points are added to the skyline.
2. The y-coordinate (or height) of each key point is adjusted such that it is the highest point in all buildings containing the key point.
3. The key points are sorted.
4. Redundant key points deleted.

To adjust the key point heights, we can possibly check *n* buildings for every building. As such, this algorithm has a complexity of *n * n* or *n<sup>2</sup>*.
```C++
/*
 * Remove redundant key points along the same y-axis (retaining the lowest x-axis point).
 * Input:  { (x1, y1), (x2, y1), (x3, y2), (x4, y3), (x5, y3), (x6, y4) }
 * Output: { (x1, y1), (x3, y2), (x4, y3), (x6, y4) }
 */
static void
remove_redundant(vector<point> &keypoints)
{
	vector<point>::iterator i, j;

	i = j = keypoints.begin();
	if (i != keypoints.end())
		i++;

	while (i != keypoints.end()) {
		if (j->y != i->y) {
			j++;
			*j = *i;
		}
		i++;
	}

	if (j != keypoints.end())
		keypoints.erase(j + 1, keypoints.end());
}

/*
 * Solves the skyline problem.
 *
 * @param [in] buildings - the set of input buildings.
 *
 * @return the skyline.
 */
vector<point>
skyline_v1(const vector<building> &buildings)
{
	vector<point> keypoints;

	/*
	 * Add all key points: top-left and bottom-right corners.
	 */
	for (auto b : buildings) {
		keypoints.emplace_back(b.start, b.height);
		keypoints.emplace_back(b.end, 0);
	}

	/*
	 * Update height (y-coordinate) of each keypoint.
	 * It must be the highest building height containing the keypoint.
	 */
	vector<point>::iterator i;
	for (i = keypoints.begin(); i != keypoints.end(); ++i) {
		int h = i->y;

		vector<building>::const_iterator k;
		for (k = buildings.begin(); k != buildings.end(); ++k)
			h = max(h, k->contains(*i));

		i->y = h;
	}

	/* Sort keypoints */
	sort(keypoints.begin(), keypoints.end());

	/* Remove duplicates */
	remove_redundant(keypoints);

	return keypoints;
}
```

### Approach 2 (Grow skyline by adding one building at a time)
1. Start with empty skyline.
2. Add a building at a time.
3. Adding a building is effectively merging the building's skyline to the overall skyline.

To add *i<sup>th</sup>* building to the existing skyline, we need to check the skyline of *(i - 1)* buildings. This is slightly better in proactive but the complexity is still *n<sup>2</sup>*.
```C++
/*
 * Add building to the skyline.
 *
 * @param [in] kp1 - the existing skyline.
 * @param [in] b   - the building to add.
 *
 * @return the final skyline.
 */
vector<point>
add_building(const vector<point> &kp1, const building &b)
{
	vector<point> keypoints;
	vector<point> kp2;

	kp2.emplace_back(b.start, b.height);
	kp2.emplace_back(b.end, 0);

	int h1 = 0; // last height in kp1 skyline
	int h2 = 0; // last height in kp2 skyline

	vector<point>::const_iterator i = kp1.begin();
	vector<point>::const_iterator j = kp2.begin();

	/*
	 * Choose the keypoint with smaller x-coordinate.
	 * Adjust the y-coordinate of the skyline. The y-coordinate is
	 * the maximum of y-coordinate and the last height of the
	 * other skyline.
	 */
	while ((i != kp1.end()) && (j != kp2.end())) {
		if (i->x < j->x) {
			keypoints.emplace_back(i->x, max(i->y, h2));
			h1 = i->y;
			i++;
		} else if (i->x == j->x) {
			keypoints.emplace_back(i->x, max(i->y, j->y));
			h1 = i->y;
			h2 = j->y;
			i++;
			j++;
		} else /* if (i->x > j->y) */ {
			keypoints.emplace_back(j->x, max(h1, j->y));
			h2 = j->y;
			j++;
		}
	}

	/*
	 * Take care of the remaining key points.
	 */

	while (i != kp1.end()) {
		keypoints.push_back(*i);
		i++;
	}

	while (j != kp2.end()) {
		keypoints.push_back(*j);
		j++;
	}

	/* Remove duplicates */
	remove_redundant(keypoints);

	return keypoints;
}

/*
 * Solves the skyline problem.
 *
 * @param [in] buildings - the set of input buildings.
 *
 * @return the skyline.
 */
vector<point>
skyline_v2(const vector<building> &buildings)
{
	vector<point> keypoints;

	/*
	 * Add one building at a time.
	 */
	for (auto b : buildings)
		keypoints = std::move(add_building(keypoints, b));

	return keypoints;
}
```

### Approach 2 (Merge skylines)
Here the skyline is expanded by merging skylines of buildings. The complexity is *n * log<sub>2</sub>(n)*.
```C++
/*
 * Merge two skylines. Not very different from add_building.
 *
 * @param [in] kp1 - the first skyline.
 * @param [in] kp2 - the second skyline.
 *
 * @return the merged skyline.
 */
vector<point>
merge_skylines(const vector<point> &kp1, const vector<point> &kp2)
{
	vector<point> keypoints;
	int h1 = 0; // last height in kp1 skyline
	int h2 = 0; // last height in kp2 skyline

	vector<point>::const_iterator i = kp1.begin();
	vector<point>::const_iterator j = kp2.begin();

	/*
	 * Choose the keypoint with smaller x-coordinate.
	 * Adjust the y-coordinate of the skyline. The y-coordinate is
	 * the maximum of y-coordinate and the last height of the
	 * other skyline.
	 */
	while ((i != kp1.end()) && (j != kp2.end())) {
		if (i->x < j->x) {
			keypoints.emplace_back(i->x, max(i->y, h2));
			h1 = i->y;
			i++;
		} else if (i->x == j->x) {
			keypoints.emplace_back(i->x, max(i->y, j->y));
			h1 = i->y;
			h2 = j->y;
			i++;
			j++;
		} else /* if (i->x > j->y) */ {
			keypoints.emplace_back(j->x, max(h1, j->y));
			h2 = j->y;
			j++;
		}
	}

	/*
	 * Take care of the remaining key points.
	 */

	while (i != kp1.end()) {
		keypoints.push_back(*i);
		i++;
	}

	while (j != kp2.end()) {
		keypoints.push_back(*j);
		j++;
	}

	/* Remove duplicates */
	remove_redundant(keypoints);

	return keypoints;
}

/*
 * Merge skylines recursively.
 *
 * @param [in] buildings - the set of input buildings.
 * @param [in] lo        - the start of buildings.
 * @param [in] hi        - the end of buildings (inclusive).
 *
 * @return the skyline.
 */ 
vector<point>
skyline_recursive(const vector<building> &buildings, size_t lo, size_t hi)
{
	if (lo == hi) {
		vector<point> kp;
		kp.emplace_back(buildings[lo].start, buildings[lo].height);
		kp.emplace_back(buildings[lo].end, 0);
		return kp;
	} else {
		size_t mid = (lo + hi) / 2;
		vector<point> kp1 = std::move(skyline_recursive(buildings, lo, mid));
		vector<point> kp2 = std::move(skyline_recursive(buildings, mid + 1, hi));
		return merge_skylines(kp1, kp2);
	}
}

/*
 * Solves the skyline problem.
 *
 * @param [in] buildings - the set of input buildings.
 *
 * @return the skyline.
 */
vector<point>
skyline_v3(const vector<building> &buildings)
{
	if (buildings.empty()) {
		return vector<point> ();
	}

	return skyline_recursive(buildings, 0, buildings.size() - 1);
}
```
