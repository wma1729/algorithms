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

void
skyline_v1(const vector<building> &buildings, vector<point> &keypoints)
{
	vector<point>::iterator i, j;

	// Update height (y-coordinate) of each keypoint.
	for (i = keypoints.begin(); i != keypoints.end(); ++i) {
		int Y = i->y;

		vector<building>::const_iterator k;
		for (k = buildings.begin(); k != buildings.end(); ++k)
			Y = max(Y, k->contains(*i));

		i->y = Y;
	}

	// Sort keypoints
	sort(keypoints.begin(), keypoints.end());

	// Remove duplicates (retain keypoints with lower x value)
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

int
main(int argc, const char **argv)
{
	int x1, x2, h;
	vector<point> keypoints;
	vector<building> buildings;

	fstream fin(argv[1], ios_base::in);
	while (fin >> x1 >> x2 >> h) {
		buildings.emplace_back(x1, x2, h);
		keypoints.emplace_back(x1, h);
		keypoints.emplace_back(x2, 0);
	}
	fin.close();

	skyline_v1(buildings, keypoints);

	for (auto p : keypoints)
		cout << p << endl;

	return 0;
}
