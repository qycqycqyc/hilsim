#ifndef SATELLITEVIEW_H
#define SATELLITEVIEW_H

#include "../environment/src/gpsTools.h"

#include "../qcustomplot.h"
#include <QTimer>
#include <QObject>
#include <QPushButton>
#include <QLabel>


#include <mutex>
#include <string>
#include <map>

using namespace std;

class simObject;
class target;
class USV;
class obstacle;


struct position {
	double x;
	double y;
	double psi;
};

class markedPosition
{
public:
	markedPosition(const markedPosition& other) : x(other.x), y(other.y) {
		positionMark = other.positionMark;
		owner = other.owner;
	}
	markedPosition(QCustomPlot *ownerPlot, double X, double Y) : x(X), y(Y), owner(ownerPlot) {
		positionMark = new QCPCurve(owner->xAxis, owner->yAxis);
		positionMark->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
		positionMark->addData(x,y);
	}
	~markedPosition(){
		// TODO: sørge for at bare en tråd aksesserer owner plot widget omgangen. Ooooh vanskelig...
		owner->removePlottable(positionMark);
	}
	position getPos(){
		return position{x,y,0};
	} 

private:
	double x;
	double y;
	QCPCurve *positionMark;
	QCustomPlot *owner;
};

class satelliteView : public QObject
{
	Q_OBJECT
public:
	satelliteView( QCustomPlot *satelliteViewWidget );
	~satelliteView();
	void addSimObject( string obstacleID, string objectDescriptor, double x, double y, double psi = 0 );
	void removeSimObject(string obstacleID);
	bool doesExist( string simObjectID );
	void setPosition( string obstacleID, gpsPointStamped newPos );
	void simTargetMoveTo( gpsPointStamped pos );
	void simTargetClearTrajectory();
	bool popMarkedPosition(position *pos);

private slots:
	void updatePlot();
	void markPosition( QMouseEvent *event );
	void zoomIn();
	void zoomOut();

private:
	QPushButton *zoomInButton;
	QPushButton *zoomOutButton;
	QFrame *rangeReference;
	int mapRangeInMeters = 800;
	QLabel *rangeDescriptor;
	void initialize_buttons();
	void update_range_descriptor();
	mutex m;
	vector<markedPosition*> MPs;
	USV *simTarget;
	map<string, simObject*> activeSimObjects;
	QCustomPlot *svWidget;
	QTimer plotTimer;
};


//--------------------------------------------------------




class simObject
{
public:
	simObject(string targetID, QCustomPlot *satelliteViewWidget);
	~simObject();
	void 			setNewPosition(gpsPointStamped Pos);
	void 			updateTrajectory();
	string 			getID(){return ID;};
	bool 			isPlottable(){ return readyToPlot; };
	virtual void 	makePlottable() = 0;
	gpsPointStamped getPosition();
	void			clearTrajectory();

protected:
	void trajectoryInit();
	bool readyToPlot = false;
	mutex m;
	string ID;
	gpsPointStamped pos;
	QCustomPlot *ownerSVWidget;
	QPixmap *targetIcon;
	QCPCurve *trajectory;
	QCPCurve *trajectoryHead;
};

class USV : public simObject
{
public:
	USV(string USVid, QCustomPlot *satelliteViewWidget, gpsPointStamped Pos);
	void makePlottable();
};

class obstacle : public simObject
{
public:
	obstacle(string obstID, QCustomPlot *satelliteViewWidget, gpsPointStamped Pos);
	void makePlottable();
};


class ship : public simObject
{
public:
	ship(string shipID, QCustomPlot *satelliteViewWidget, gpsPointStamped Pos);
	void makePlottable();
};

#endif // SATELLITEVIEW_H
