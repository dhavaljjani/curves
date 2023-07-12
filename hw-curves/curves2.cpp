#include "WorkingScene.h"

// This file includes the basic functions that your program must fill in.  
// Note that there are several helper functions from Curve.cpp that can be used!
// In particular, take a look at moveActivePoint, connectTheDots, drawLine, etc.

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
void WorkingScene::drag(int x, int y) {
	/* YOUR CODE HERE */
	//you must figure out how to transform x and y so they make sense
	//update oldx, and oldy
	theOnlyCurve->moveActivePoint((float)(x - oldx) / (float)width, (float)(oldy - y) / (float)height);

	oldx = x;
	oldy = y;
	//make sure scene gets redrawn
	glutPostRedisplay();
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {
	// theOnlyCurve is the current type of curve being drawn. It is created in Scene.cpp.
	if (theOnlyCurve && state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			/* YOUR CODE HERE */
			theOnlyCurve->addPoint((float)(x) / (float)(width), 1 - (float)(y) / (float)(height));
		}
		if (button == GLUT_RIGHT_BUTTON) {	
			/* YOUR CODE HERE */
			theOnlyCurve->deleteActivePoint();
			theOnlyCurve->updateActivePoint(oldx, oldy);
		}
	}

	/* YOUR CODE HERE */
	//update oldx, and oldy
	//make sure scene gets redrawn
	oldx = x;
	oldy = y;
	glutPostRedisplay();
}



#include "Bezier.h"

// Bezier drawing function.  This is by deCasteljau or equivalent algorithm. 
// It should support Bezier curves of arbitrary degree/order.
void Bezier::draw(int levelOfDetail) {

	connectTheDots();
	int i, j, k;
	Pvector::iterator p;
	Pvector Bezier_points;

	if (points.size() > 2) {
		for (i = 0; i <= levelOfDetail; i++) {
			float t = ((float)i / (float)levelOfDetail);
			//printf("t:[%f]\n", t);
			Pvector P_curr = points; //this copies over all points from one vector to other
			//k is level
			for (k = 0; k < points.size() - 1; k++) {
				Pvector P_newlist;
				for (int f = 0; f < P_curr.size() - 1; f++) {
					Point P_new = points[0];
					P_new.x = t * P_curr[f].x + (1 - t) * P_curr[f + 1].x;
					P_new.y = t * P_curr[f].y + (1 - t) * P_curr[f + 1].y;
					P_newlist.push_back(P_new);
				}
				P_curr = P_newlist;
			}
			Bezier_points.push_back(P_curr[0]);
		}
	}
	//draw Bezier Points
	//printf("BEZIER POINTS SIZE: %i\n", Bezier_points.size());
	if (Bezier_points.size() > 2) {
		for (k = 0; k < Bezier_points.size() - 1; k++) {
			//printf("___%i___: BEZIER_X:[%f], BEZIER_Y:[%f]\n", k, (float)Bezier_points[k].x, (float)Bezier_points[k].y);
			drawLine((float)Bezier_points[k].x,
				(float)Bezier_points[k].y,
				(float)Bezier_points[k + 1].x,
				(float)Bezier_points[k + 1].y);
		}
	}
}



#include "Bspline.h"

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {
	connectTheDots();
	/* YOUR CODE HERE */
	if (points.size() >= 4) {
		for (int i = 0; i < points.size(); i++) {
			if (i + 3 < points.size()) {
				Bspline::drawSegment(points.begin() + i, points.begin() + i + 1, points.begin() + i + 2, points.begin() + i + 3, levelOfDetail);
			}
		}
	}

}

void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {
	//draw segment
	//then create a Point to be drawn where the knot should be
	Pvector BSpline;
	for (int i = 0; i <= levelOfDetail; i++) {
		float u = ((float)i / (float)levelOfDetail);
		//DECASTELJAU (NOT WORKING)
		/*Point q1(0, 0);
		q1.x = ((*p2).x * ((1 - u) / (float)(3.0))) + ((*p1).x * ((2 + u) / (float)(3.0)));
		q1.y = ((*p2).y * ((1 - u) / (float)(3.0))) + ((*p1).y * ((2 + u) / (float)(3.0)));
		Point q2(0, 0);
		q2.x = ((*p2).x * ((1 + u) / (float)(3.0))) + ((*p3).x * ((2 - u) / (float)(3.0)));
		q2.y = ((*p2).y * ((1 + u) / (float)(3.0))) + ((*p3).y * ((2 - u) / (float)(3.0)));
		Point q3(0, 0);
		q3.x = ((*p4).x * (1 - (u / (float)(3.0)))) + ((*p3).x * (u / (float)(3.0)));
		q3.y = ((*p4).y * (1 - (u / (float)(3.0)))) + ((*p3).y * (u / (float)(3.0)));

		Point r1(0, 0);
		r1.x = (q1.x * ((1 + u) / (float)(2.0))) + (q2.x * ((1 - u) / (float)(2.0)));
		r1.y = (q1.y * ((1 + u) / (float)(2.0))) + (q2.y * ((1 - u) / (float)(2.0)));
		Point r2(0, 0);
		r2.x = (q3.x * (1 - (u / (float)(2.0)))) + (q2.x * (u / (float)(2.0)));
		r2.y = (q3.y * (1 - (u / (float)(2.0)))) + (q2.y * (u / (float)(2.0)));

		Point s1(0, 0);
		s1.x = (r1.x * (float)(u)) + (r2.x * (float)(1 - u));
		s1.y = (r1.y * (float)(u)) + (r2.y * (float)(1 - u));*/

		//EXPLICIT FORMULA
		Point knot(0, 0);
		knot.x = (1 / (float)(6.0)) * (((1 - (3 * u) + (3 * ((float)(u) * (float)(u))) - ((float)(u) * (float)(u) * (float)(u))) * (*p1).x)
			+ ((4 - (6 * ((float)(u) * (float)(u))) + (3 * ((float)(u) * (float)(u) * (float)(u)))) * (*p2).x)
			+ ((1 + (3 * u) + (3 * ((float)(u) * (float)(u))) - (3 * ((float)(u) * (float)(u) * (float)(u)))) * (*p3).x)
			+ (((float)(u) * (float)(u) * (float)(u)) * (*p4).x));
		knot.y = (1 / (float)(6.0)) * (((1 - (3 * u) + (3 * ((float)(u) * (float)(u))) - ((float)(u) * (float)(u) * (float)(u))) * (*p1).y)
			+ ((4 - (6 * ((float)(u) * (float)(u))) + (3 * ((float)(u) * (float)(u) * (float)(u)))) * (*p2).y)
			+ ((1 + (3 * u) + (3 * ((float)(u) * (float)(u))) - (3 * ((float)(u) * (float)(u) * (float)(u)))) * (*p3).y)
			+ (((float)(u) * (float)(u) * (float)(u)) * (*p4).y));
		BSpline.push_back(knot);
		if (i == levelOfDetail) {
			knot.draw();
		}
	}
	for (int k = 0; k < BSpline.size() - 1; k++) {
		drawLine((float)BSpline[k].x,
			(float)BSpline[k].y,
			(float)BSpline[k + 1].x,
			(float)BSpline[k + 1].y);
	}
}

#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {
		/* YOUR CODE HERE  (only one to three lines)*/
		(*it).x = (float)(((*it).x) + (*(it + 1)).x) / (float)(2.0);
		(*it).y = (float)(((*it).y) + (*(it + 1)).y) / (float)(2.0);
		//(*it).draw();
		accum->push_back(*it);
	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}


// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {
				//YOUR CODE HERE
				drawLine((float)(*it).x,
					(float)(*it).y,
					(float)(*(it + 1)).x,
					(float)(*(it + 1)).y);
			}
		}
	} else {
		Pvector* accum = new Pvector();
		Bezier2 left, right;

		accumulateNextLevel(accum, points);
		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
		/* YOUR CODE HERE */
		for (int i = 0; i < points.size(); i++) {
			if (i <= (points.size() / 2)) {
				left.addPoint(points[i].x, points[i].y);
			}
			if (i >= (points.size() / 2)) {
				right.addPoint(points[i].x, points[i].y);
			}
		}

		left.draw(1-levelOfDetail);
		right.draw(1-levelOfDetail);
		delete accum;
	}
}
