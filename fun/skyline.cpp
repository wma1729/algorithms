#include <cassert>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

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

int
main(int argc, const char **argv)
{
	int x1, x2, h;
	vector<building> buildings;

	fstream fin(argv[1], ios_base::in);
	while (fin >> x1 >> x2 >> h)
		buildings.emplace_back(x1, x2, h);
	fin.close();

	vector<point> keypoints;

	keypoints = std::move(skyline_v1(buildings));
	cout << "V1" << endl;
	for (auto p : keypoints)
		cout << p << endl;
	cout << endl;

	keypoints = std::move(skyline_v2(buildings));
	cout << "V2" << endl;
	for (auto p : keypoints)
		cout << p << endl;
	cout << endl;

	keypoints = std::move(skyline_v3(buildings));
	cout << "V3" << endl;
	for (auto p : keypoints)
		cout << p << endl;
	cout << endl;


	return 0;
}
