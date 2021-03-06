#ifndef OBSTACLEINTERFACE_H
#define OBSTACLEINTERFACE_H

#include <QGroupBox>
#include <QPushButton>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>

#include <string>
#include <thread>

#include "satelliteview.h"

#include "/opt/ros/kinetic/include/ros/ros.h"
#include "simulator_messages/obstacleCmd.h"
#include "simulator_messages/Gps.h"


class obstacleInterface : public QObject
{
	Q_OBJECT
public:
	obstacleInterface(){};
	obstacleInterface(ros::NodeHandle *nh, QGroupBox *interfaceWindow, satelliteView *Sv);
	~obstacleInterface(){};
	void requestNewObstacle(double x, double y, double psi);

private slots:
	void handleSpawnButton();

private:
	satelliteView *sv;
	ros::Publisher cmdPub;
	QPushButton *spawnObstacleButton;
};


#endif // OBSTACLEINTERFACE_H

