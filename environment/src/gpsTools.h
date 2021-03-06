#ifndef GPSTOOLS_H
#define GPSTOOLS_H

#include <QTime>

struct gpsPoint {
	gpsPoint(){};
	gpsPoint(double Longitude, double Latitude){
		this->longitude = Longitude;
		this->latitude = Latitude;
	}
	double longitude = 0;
	double latitude = 0;
};

struct gpsPoint3DOF : public gpsPoint {
	gpsPoint3DOF(){heading = 0;};
	gpsPoint3DOF(double Longitude, double Latitude, double Heading){
		this->longitude = Longitude;
		this->latitude = Latitude;
		this->heading = Heading;
	};
	double heading;
};

struct gpsPointStamped : public gpsPoint3DOF
{
	gpsPointStamped(){timeStamp = QTime::currentTime();}
	gpsPointStamped(double Longitude, double Latitude, double Heading){
		this->longitude = Longitude;
		this->latitude = Latitude;
		this->heading = Heading;
		this->timeStamp = QTime::currentTime();
	};

	gpsPointStamped(double Longitude, double Latitude, double Heading, QTime time){
		this->longitude = Longitude;
		this->latitude = Latitude;
		this->heading = Heading;
		this->timeStamp = time;
	};
	QTime timeStamp;
};

double deg2rad(double degrees);

double distance_m(gpsPoint a, gpsPoint b);

double compass_bearing(gpsPoint from, gpsPoint to);

bool is_within_bearing_range(double bearing, double min, double max);

double latitude_degs_pr_meter();

double longitude_degs_pr_meter(double currentLatitude);

#endif // GPSTOOLS_H