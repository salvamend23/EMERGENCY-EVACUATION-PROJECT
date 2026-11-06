#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include<algorithm>
using namespace std;

struct Connection {
	int from;
	int to;
	double distance;
};
struct path_results {
	vector<int> route;
	double total_distance = 0.0;
	int exitid = -1;
	bool found = false;

};

class Pathfinder {
public:
	vector <Connection> connections;
	void addconnections(int from, int to, double distance) {
		connections.push_back({ from, to, distance });
		connections.push_back({ to, from, distance });
	}
	void showneighbors(int locationid) {
		for (Connection c : connections) {
			if (c.from == locationid) {
				cout << "location neighbor: " << c.to << " Distance: " << c.distance << " METERS" << endl;

			}
		}

	}
	path_results calculate_distances(
		int startid,
   	    int blockedHallwayId = -1,
 	    int targetExitId = -1,
        const set<int>& blockedExitIds = {},
        const set<int>& blockedHallwayIds = {}
	) {
		path_results results;
		if (startid == blockedHallwayId) { // THIS WILL RETURN THE ORIGINAL RESULTS IF THE START LOCATION IS BLOCKED
			return results;
		}
		map <int, double> distances;
		set<int> visited;
		map<int, int> previous;
		for (Connection c : connections) {
			distances[c.from] = numeric_limits<double>::infinity();
			distances[c.to] = numeric_limits<double>::infinity();
		}
		distances[startid] = 0;
		
		// find unvisited location with smallest distance
		while (true) {
			int current = -1;
			double shortest = numeric_limits<double>::infinity();
			for (auto item : distances) {
				int id = item.first;

				double distance = item.second;

				if (visited.count(id) == 0 && distance < shortest) {
					shortest = distance;
					current = id;
				}


			}
			if (current == -1) {
				break; // all locations visited
			}
			visited.insert(current);
			for (Connection c : connections) {
				if (c.from == blockedHallwayId ||
    				c.to == blockedHallwayId ||
    				blockedHallwayIds.count(c.from) > 0 ||
  				    blockedHallwayIds.count(c.to) > 0) {
 				    continue;
				}

				if (blockedExitIds.count(c.from) > 0 ||
    				blockedExitIds.count(c.to) > 0) {
   					 continue;
				}

				if (c.from == current && visited.count(c.to) == 0) {

					double newdistance = distances[current] + c.distance;

					if (newdistance < distances[c.to]){
						distances[c.to] = newdistance;
						previous[c.to] = current;
					}
				}
			}

		}
		cout << "\n\nShortest distances from location " << startid << ":\n"; 
		for (auto d : distances) {
			cout << "location: " << d.first  << " Distance: " << d.second << "Meters" << endl;

		}




		/// EXIT ROUTE CALCULATION
		vector <int> route;
		int exitid;
		if (distances[201] == numeric_limits<double>::infinity() && distances[202] == numeric_limits<double>::infinity()) {
			cout << "You are trapped! No evacuation route available!!!!" << endl;
			return results;

		}
		if (targetExitId == 201 || targetExitId == 202) {
			exitid = targetExitId;
		}
		else {
			if (distances[201] <= distances[202]) {
				exitid = 201;
			}
			else {
				exitid = 202;
			}
		}
		
		
	

		int current = exitid;

		while (current != startid) {

			route.push_back(current);

			if (previous.count(current) == 0) {
				cout << "No route found!" << endl;
				return results;
			}

			current = previous[current];
		}

		route.push_back(startid);


		cout << "\nEvacuation route: ";

		reverse(route.begin(), route.end());
		// instert variables inthe contstruct of results
		results.exitid = exitid;
		results.found = true;
		results.route = route;
		results.total_distance = distances[exitid];
		return results;
		for (int i : route) {
			
			cout << i << " ";
			while (i != exitid) {
				cout << " --> ";
				break;
			}

		
		}
		



	



	}


	// CREATION OF THE EMERGENCY SIMULATION FUNCTION
	path_results simulate_emergency(int startid, int blockedHallwayId) {
		return calculate_distances(startid, blockedHallwayId); // THIS LINE IS THE DJIKSTRA ALGORITHM FUNCTION CALL WITH THE BLOCKED HALLWAY ID AS A PARAMETER.

	}


	// COMPARE ROUTES TO EXITS

	void compare_routes(int startid, int blockedHallwayId = -1) {
		path_results path1 = calculate_distances(startid, blockedHallwayId, 201);

		path_results path2 = calculate_distances(startid, blockedHallwayId, 202);

		cout << "\n\nComparing routes to exits from room " << startid << ":\n";

		if (path1.found) {
			cout << "Route to exit 201 from room " << startid << ": " << path1.total_distance << " METERS" << endl;

		}
		else {
			cout << "No route to exit 201 from room " << startid << endl;
		}

		if (path2.found) {
			cout << "Route to exit 202 from room " << startid << ": " << path2.total_distance << " METERS" << endl;

		}
		else {
			cout << "No route to exit 202 from room " << startid << endl;
		}


		cout << "\nBest route to exit from room " << startid << ": ";

				if (path1.found && path2.found) {
			if (path1.total_distance < path2.total_distance) {
				cout << "Exit 201 is the best route with a distance of "
					<< path1.total_distance << " METERS" << endl;
			}
			else if (path2.total_distance < path1.total_distance) {
				cout << "Exit 202 is the best route with a distance of "
					<< path2.total_distance << " METERS" << endl;
			}
			else {
				cout << "Both exits are equally distant with a distance of "
					<< path1.total_distance << " METERS" << endl;
			}
		}
		else if (path1.found) {
			cout << "Exit 201 is the best available route with a distance of "
				<< path1.total_distance << " METERS" << endl;
		}
		else if (path2.found) {
			cout << "Exit 202 is the best available route with a distance of "
				<< path2.total_distance << " METERS" << endl;
		}
		else {
			cout << "No evacuation route is available." << endl;
		}
	}
};
